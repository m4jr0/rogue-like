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

from pathlib import Path
from typing import Any, Mapping, Sequence

from rlres.data.engine.physics.collider import parse_collider_desc

from rlres.data.engine.physics.collider_profile import (
    AnimColliderProfileResourceModel,
    AnimColliderSampleModel,
    AnimColliderSetModel,
    AnimColliderTrackModel,
)

from rlres.data.engine.resource.index_builder import ResourceIndex
from rlres.data.engine.resource.resource_entry import ResourceEntry
from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.data.engine.resource.resource_utils import make_res_filepath
from rlres.exporter.asset_exporter import AssetExporter
from rlres.exporter.engine.physics.collider_serialize import write_collider_profile_file
from rlres.logger import LOGGER
from rlres.utils.safe_utils import safe_json_get, safe_load_json_object


class AnimColliderProfileExporter(AssetExporter):
    type_names: Sequence[ResourceTypeName] = (ResourceTypeName.ANIM_COLLIDER_PROFILE,)
    depends_on: Sequence[ResourceTypeName] = (ResourceTypeName.ANIM_SET,)
    discover_patterns: Sequence[str] = ("*.json",)

    def _build_one(
        self,
        entry: ResourceEntry,
        _: Path,
        out_dir: Path,
        index: ResourceIndex,
    ) -> None:
        cls_name = self.__class__.__name__
        src_path = entry.src_path

        data: Mapping[str, Any] = safe_load_json_object(src_path)

        if not data:
            LOGGER.error(
                "%s: empty or invalid JSON for collider profile %r from %s; skipping",
                cls_name,
                entry.name,
                src_path,
            )

            return

        raw_per_anim = safe_json_get(
            data,
            "per_anim",
            expected_type=list,
            default_value=[],
            mandatory=False,
        )

        per_anim: list[AnimColliderSetModel] = []

        for raw_entry in raw_per_anim:
            if not isinstance(raw_entry, Mapping):
                LOGGER.warn(
                    "%s: collider profile %r has non-mapping per_anim entry; skipping. Raw=%r",
                    cls_name,
                    entry.name,
                    raw_entry,
                )

                continue

            set_model = self._build_collider_set(raw_entry)
            per_anim.append(set_model)

        model = AnimColliderProfileResourceModel(
            rid=entry.rid,
            per_anim=per_anim,
        )

        out_path = make_res_filepath(out_dir, entry.rid)
        write_collider_profile_file(out_path, model)

    def _build_collider_set(self, data: Mapping[str, Any]) -> AnimColliderSetModel:
        hurts_raw = safe_json_get(
            data,
            "hurts",
            expected_type=list,
            default_value=[],
            mandatory=False,
        )

        hits_raw = safe_json_get(
            data,
            "hits",
            expected_type=list,
            default_value=[],
            mandatory=False,
        )

        hurts = [self._build_track(raw_track) for raw_track in hurts_raw]
        hits = [self._build_track(raw_track) for raw_track in hits_raw]

        return AnimColliderSetModel(hurts=hurts, hits=hits)

    def _build_track(
        self,
        raw_track: Any,
    ) -> AnimColliderTrackModel:
        if not isinstance(raw_track, list):
            LOGGER.warn(
                "Collider track is not a list; using empty track. Raw=%r", raw_track
            )

            return AnimColliderTrackModel()

        samples: list[AnimColliderSampleModel] = []

        for raw_sample in raw_track:
            if not isinstance(raw_sample, Mapping):
                LOGGER.warn(
                    "Collider sample is not a mapping; skipping. Raw=%r", raw_sample
                )

                continue

            collider_data = safe_json_get(
                raw_sample,
                "collider",
                expected_type=Mapping,
                default_value={},
                mandatory=False,
            )

            collider = parse_collider_desc(collider_data)

            samples.append(
                AnimColliderSampleModel(
                    collider=collider,
                )
            )

        return AnimColliderTrackModel(samples=samples)
