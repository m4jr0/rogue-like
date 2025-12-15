# Copyright 2025 m4jr0. All Rights Reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

from __future__ import annotations

import json

from pathlib import Path
from typing import Any, Mapping, Sequence

from rlres.data.engine.anim.anim_id import (
    INVALID_ANIM_KEY,
    AnimKey,
    anim_id_from_name,
    make_anim_key,
)


from rlres.data.engine.anim.anim_type import (
    AnimKeyFrameModel,
    AnimResourceModel,
    AnimSampleModel,
    AnimSetResourceModel,
    AnimSolvedEntryModel,
    anim_flag_name_to_flag,
    anim_flip_policy_name_to_policy,
    anim_key_tag_name_to_tag,
)

from rlres.data.engine.param_set.param_set import encode_param_set
from rlres.data.engine.physics.physics import cardinal_dir

from rlres.data.engine.resource.index_builder import (
    ResourceEntry,
    ResourceIndex,
)

from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.data.engine.resource.resource_utils import make_res_filepath
from rlres.data.engine.sprite.sprite_type import SpriteRect, Sprite
from rlres.exporter.asset_exporter import AssetExporter
from rlres.exporter.engine.anim.anim_serialize import write_anim_set_file
from rlres.logger import LOGGER
from rlres.utils.json_utils import rid_from_key
from rlres.utils.safe_utils import safe_json_get, safe_json_get_flags


class AnimExporter(AssetExporter):
    type_names: Sequence[ResourceTypeName] = (ResourceTypeName.ANIM_SET,)
    depends_on: Sequence[ResourceTypeName] = (ResourceTypeName.TEX,)
    discover_patterns: Sequence[str] = ("*.json",)

    def _build_one(
        self,
        entry: ResourceEntry,
        _: Path,
        out_dir: Path,
        index: ResourceIndex,
    ) -> None:
        cls_name = self.__class__.__name__

        try:
            text = entry.src_path.read_text(encoding="utf-8")
        except OSError as e:
            LOGGER.error(
                "%s: failed to read anim JSON %s: %s",
                cls_name,
                entry.src_path,
                e,
            )

            return

        try:
            data = json.loads(text)
        except json.JSONDecodeError as e:
            LOGGER.error(
                "%s: failed to parse anim JSON %s: %s",
                cls_name,
                entry.src_path,
                e,
            )

            return

        if not isinstance(data, Mapping):
            LOGGER.error(
                "%s: source JSON root is not an object: %r (from %s)",
                cls_name,
                data,
                entry.src_path,
            )

            return

        tex_rid = rid_from_key(index, data, ResourceTypeName.TEX, "texture")

        raw_anims = safe_json_get(
            data,
            "animations",
            expected_type=list,
            default_value=[],
            mandatory=False,
        )

        anims = self._build_anims(index, raw_anims)

        default_spec = safe_json_get(
            data,
            "default_anim",
            expected_type=(int, str),
            default_value=None,
            mandatory=False,
        )

        collider_profile_nid = safe_json_get(
            data,
            "collider_profile",
            expected_type=str,
            default_value="",
            mandatory=False,
            allow_null=True,
        )

        collider_profile_rid = index.rid_from_nid(collider_profile_nid)

        default_idx = self._extract_default_anim_idx(
            default_spec=default_spec,
            raw_anims=raw_anims,
        )

        solved_raw = safe_json_get(
            data,
            "solved",
            expected_type=list,
            default_value=[],
            mandatory=False,
        )

        solved = self._build_solved(solved_raw)

        anim_set = AnimSetResourceModel(
            rid=entry.rid,
            tex=tex_rid,
            collider_profile=collider_profile_rid,
            default_anim_idx=default_idx,
            anims=anims,
            solved=solved,
        )

        out_path = make_res_filepath(out_dir, entry.rid)
        write_anim_set_file(out_path, anim_set)

    def _build_anims(
        self,
        index: ResourceIndex,
        raw_anims: Sequence[dict[str, Any]] | None,
    ) -> list[AnimResourceModel]:
        cls_name = self.__class__.__name__
        anims: list[AnimResourceModel] = []

        if raw_anims is None:
            return anims

        for raw_anim in raw_anims:
            if not isinstance(raw_anim, Mapping):
                LOGGER.warn(
                    "%s: animation entry is not a mapping; skipping. Raw=%r",
                    cls_name,
                    raw_anim,
                )

                continue

            flags = safe_json_get_flags(
                raw_anim,
                "flags",
                name_to_flag=anim_flag_name_to_flag,
                default=0,
            )

            anim_key = self._extract_anim_key(raw_anim)

            raw_frames = safe_json_get(
                raw_anim,
                "frames",
                expected_type=list,
                default_value=[],
                mandatory=False,
            )

            samples, acc_durs, total_duration = self._build_samples(raw_frames)

            raw_events = safe_json_get(
                raw_anim,
                "events",
                expected_type=list,
                default_value=[],
                mandatory=False,
            )

            events = self._build_events(index, raw_events)

            anims.append(
                AnimResourceModel(
                    flags=flags,
                    duration=total_duration,
                    key=anim_key,
                    samples=samples,
                    acc_durations=acc_durs,
                    keys=events,
                )
            )

        return anims

    def _build_samples(
        self,
        raw_frames: Sequence[dict[str, Any]] | None,
    ) -> tuple[list[AnimSampleModel], list[float], float]:
        cls_name = self.__class__.__name__
        samples: list[AnimSampleModel] = []
        acc_durs: list[float] = []
        acc = 0.0

        if raw_frames is None:
            return samples, acc_durs, acc

        for raw_frame in raw_frames:
            if not isinstance(raw_frame, Mapping):
                LOGGER.warn(
                    "%s: frame entry is not a mapping; skipping. Raw=%r",
                    cls_name,
                    raw_frame,
                )

                continue

            rect = safe_json_get(
                raw_frame,
                "rect",
                expected_type=list,
                default_value=[0, 0, 0, 0],
            )

            source_size = safe_json_get(
                raw_frame,
                "source_size",
                expected_type=list,
                default_value=[0, 0],
            )

            offset = safe_json_get(
                raw_frame,
                "offset",
                expected_type=list,
                default_value=[0.0, 0.0],
            )

            offset_flipped = safe_json_get(
                raw_frame,
                "offset_flipped",
                expected_type=list,
                default_value=[0.0, 0.0],
            )

            if (
                len(rect) != 4
                or len(source_size) != 2
                or len(offset) != 2
                or len(offset_flipped) != 2
            ):
                LOGGER.warn(
                    "%s: frame has invalid rect/source_size/offset structure; skipping. Raw=%r",
                    cls_name,
                    raw_frame,
                )

                continue

            try:
                rx, ry, rw, rh = rect
                sw, sh = source_size
                ox, oy = offset
                ofx, ofy = offset_flipped

            except ValueError as e:
                LOGGER.warn(
                    "%s: failed to unpack frame arrays; skipping. Raw=%r (%s)",
                    cls_name,
                    raw_frame,
                    e,
                )

                continue

            dur_value = safe_json_get(
                raw_frame,
                "duration",
                expected_type=(int, float),
                default_value=0.0,
                mandatory=False,
            )

            try:
                dur = float(dur_value)
            except (TypeError, ValueError) as e:
                LOGGER.warn(
                    "%s: invalid 'duration' %r; using 0.0 (%s)",
                    cls_name,
                    dur_value,
                    e,
                )

                dur = 0.0

            srect = SpriteRect(x=int(rx), y=int(ry), w=int(rw), h=int(rh))

            sprite = Sprite(
                rect=srect,
                offset=(float(ox), float(oy)),
                offset_flipped=(float(ofx), float(ofy)),
                source_size=(int(sw), int(sh)),
            )

            samples.append(AnimSampleModel(sprite=sprite, duration=dur))

            acc += dur
            acc_durs.append(acc)

        total_duration = acc
        return samples, acc_durs, total_duration

    def _build_events(
        self,
        index: ResourceIndex,
        raw_events: Sequence[dict[str, Any]] | None,
    ) -> list[AnimKeyFrameModel]:
        cls_name = self.__class__.__name__
        events: list[AnimKeyFrameModel] = []

        if raw_events is None:
            return events

        for raw_event in raw_events:
            if not isinstance(raw_event, Mapping):
                LOGGER.warn(
                    "%s: event entry is not a mapping; skipping. Raw=%r",
                    cls_name,
                    raw_event,
                )

                continue

            frame_val = safe_json_get(
                raw_event,
                "frame",
                expected_type=(int, float),
                default_value=0,
            )

            name = safe_json_get(
                raw_event,
                "name",
                expected_type=str,
                default_value="",
            )

            try:
                frame_idx = int(frame_val)
            except (TypeError, ValueError):
                LOGGER.warn(
                    "%s: event has invalid 'frame' %r; skipping. Raw=%r",
                    cls_name,
                    frame_val,
                    raw_event,
                )

                continue

            if not name:
                LOGGER.warn(
                    "%s: event missing or empty 'name'; skipping. Raw=%r",
                    cls_name,
                    raw_event,
                )

                continue

            raw_params = safe_json_get(
                raw_event,
                "params",
                expected_type=list,
                default_value=[],
                mandatory=False,
            )

            params: list[int] = encode_param_set(raw_params, index)
            key_tag = anim_key_tag_name_to_tag(name)

            frame = AnimKeyFrameModel(
                key_tag=key_tag,
                frame=frame_idx,
                params=params,
            )

            events.append(frame)

        return events

    def _build_solved(
        self,
        raw_solved: Sequence[dict[str, Any]] | None,
    ) -> list[AnimSolvedEntryModel]:
        cls_name = self.__class__.__name__
        entries: list[AnimSolvedEntryModel] = []

        if raw_solved is None:
            return entries

        for raw in raw_solved:
            if not isinstance(raw, Mapping):
                LOGGER.warn(
                    "%s: solved entry is not a mapping; skipping. Raw=%r",
                    cls_name,
                    raw,
                )

                continue

            raw_from = safe_json_get(
                raw,
                "from",
                expected_type=Mapping,
                default_value={},
            )

            raw_to = safe_json_get(
                raw,
                "to",
                expected_type=Mapping,
                default_value={},
            )

            from_tag = safe_json_get(
                raw_from,
                "anim_tag",
                expected_type=str,
                default_value="",
            )

            from_dir = safe_json_get(
                raw_from,
                "dir",
                expected_type=str,
                default_value="",
            )

            to_tag = safe_json_get(
                raw_to,
                "anim_tag",
                expected_type=str,
                default_value="",
            )

            to_dir = safe_json_get(
                raw_to,
                "dir",
                expected_type=str,
                default_value="",
            )

            if not from_tag or not from_dir or not to_tag or not to_dir:
                LOGGER.warn(
                    "%s: solved entry missing anim_tag/dir fields; skipping. Raw=%r",
                    cls_name,
                    raw,
                )

                continue

            from_key = make_anim_key(
                anim_id_from_name(from_tag),
                cardinal_dir(from_dir),
            )

            to_key = make_anim_key(
                anim_id_from_name(to_tag),
                cardinal_dir(to_dir),
            )

            if from_key == INVALID_ANIM_KEY or to_key == INVALID_ANIM_KEY:
                LOGGER.warn(
                    "%s: solved entry has invalid from/to key; skipping. Raw=%r",
                    cls_name,
                    raw,
                )

                continue

            flip_name = safe_json_get(
                raw,
                "flip_policy",
                expected_type=str,
                default_value="keep",
                mandatory=False,
            )

            flip_policy = anim_flip_policy_name_to_policy(flip_name)

            entries.append(
                AnimSolvedEntryModel(
                    from_key=from_key,
                    to_key=to_key,
                    flip_policy=flip_policy,
                )
            )

        return entries

    @staticmethod
    def _extract_default_anim_idx(
        default_spec: Any,
        raw_anims: Sequence[Mapping[str, Any]] | None,
    ) -> int:
        if not raw_anims:
            return 0

        if default_spec is None:
            return 0

        if isinstance(default_spec, int):
            if 0 <= default_spec < len(raw_anims):
                return default_spec

            LOGGER.warn(
                "%s: default_anim index %d out of range (0..%d); using 0",
                AnimExporter.__name__,
                default_spec,
                len(raw_anims) - 1,
            )

            return 0

        if isinstance(default_spec, str):
            for i, raw_anim in enumerate(raw_anims):
                if not isinstance(raw_anim, Mapping):
                    continue

                name = safe_json_get(
                    raw_anim,
                    "name",
                    expected_type=str,
                    default_value="",
                    mandatory=False,
                )

                anim_tag = safe_json_get(
                    raw_anim,
                    "anim_tag",
                    expected_type=str,
                    default_value="",
                    mandatory=False,
                )

                if default_spec == name or default_spec == anim_tag:
                    return i

            LOGGER.warn(
                "%s: default_anim %r did not match any animation (by 'name' or 'anim_tag'); using 0",
                AnimExporter.__name__,
                default_spec,
            )

            return 0

        LOGGER.warn(
            "%s: default_anim has unsupported type %s; using 0",
            AnimExporter.__name__,
            type(default_spec).__name__,
        )

        return 0

    @staticmethod
    def _extract_anim_key(
        raw_anim: Mapping[str, Any],
    ) -> AnimKey:
        anim_tag_name = safe_json_get(
            raw_anim,
            "anim_tag",
            expected_type=str,
            default_value="",
        )

        dir_name = safe_json_get(
            raw_anim,
            "dir",
            expected_type=str,
            default_value="",
        )

        if not anim_tag_name or not dir_name:
            LOGGER.warn(
                "%s: anim missing 'anim_tag' or 'dir'; using INVALID_ANIM_KEY. Raw=%r",
                AnimExporter.__name__,
                raw_anim,
            )

            return INVALID_ANIM_KEY

        aid = anim_id_from_name(anim_tag_name)
        direction = cardinal_dir(dir_name)
        key = make_anim_key(aid, direction)
        return key
