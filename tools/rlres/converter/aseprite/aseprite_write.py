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

from rlres.converter.aseprite.aseprite_build import build_anim_frames, build_events
from rlres.converter.aseprite.aseprite_type import AsepriteFrameEntry, OffsetPair
from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.data.engine.resource.resource_utils import make_nid
from rlres.logger import LOGGER
from rlres.utils.safe_utils import safe_json_get


def write_atlas(
    out_root: Path,
    atlas_id: str,
    tex: str,
    sprites: Sequence[Mapping[str, Any]],
) -> Path:
    atlas_dir = out_root / ResourceTypeName.ATLAS
    atlas_dir.mkdir(parents=True, exist_ok=True)
    path = atlas_dir / f"{atlas_id}.json"

    atlas = {
        "id": atlas_id,
        "texture": tex,
        "sprites": list(sprites),
    }

    path.write_text(
        json.dumps(atlas, indent=2),
        encoding="utf-8",
    )

    LOGGER.info("Wrote atlas: %s", path)

    return path


def write_animset(
    out_root: Path,
    atlas_id: str,
    tex: str,
    tags: Sequence[Mapping[str, Any]],
    entries: Sequence[AsepriteFrameEntry],
    offsets: Sequence[OffsetPair],
    meta_anims_by_name: Mapping[str, Mapping[str, Any]],
    default_anim: str | None,
    solver_entries: Sequence[Mapping[str, Any]] | None,
) -> Path:
    anims: list[dict[str, Any]] = []

    for raw_tag in tags:
        if not isinstance(raw_tag, Mapping):
            LOGGER.warn("Ignoring non-mapping tag: %r", raw_tag)
            continue

        tag_name = safe_json_get(raw_tag, "name", str, "", mandatory=True)

        if not tag_name:
            LOGGER.warn("Tag missing valid 'name': %r", raw_tag)
            continue

        frames_list = build_anim_frames(
            raw_tag,
            entries,
            offsets,
        )

        if not frames_list:
            LOGGER.warn("Tag %r produced no frames; skipping", tag_name)
            continue

        meta_anim = meta_anims_by_name.get(tag_name, {})
        anim_tag = safe_json_get(meta_anim, "anim_tag", str, None, mandatory=False)
        dir_name = safe_json_get(meta_anim, "dir", str, None, mandatory=False)
        flags = safe_json_get(meta_anim, "flags", list, [], mandatory=False)

        events = build_events(
            meta_anim,
            frame_count=len(frames_list),
        )

        anims.append(
            {
                "name": tag_name,
                "anim_tag": anim_tag,
                "dir": dir_name,
                "flags": flags,
                "frames": frames_list,
                "events": events,
            }
        )

    anim_dir = out_root / ResourceTypeName.ANIM_SET
    anim_dir.mkdir(parents=True, exist_ok=True)
    path = anim_dir / f"{atlas_id}.json"

    anim: dict[str, Any] = {
        "id": atlas_id,
        "texture": tex,
        "collider_profile": make_nid(ResourceTypeName.ANIM_COLLIDER_PROFILE, atlas_id),
        "default_anim": default_anim,
        "animations": anims,
    }

    if solver_entries:
        solved: list[dict[str, Any]] = []

        for s in solver_entries:
            if isinstance(s, Mapping):
                solved.append(dict(s))
            else:
                LOGGER.warn("Ignoring non-dict entry in solver_entries: %r", s)

        if solved:
            anim["solved"] = solved

    path.write_text(
        json.dumps(anim, indent=2),
        encoding="utf-8",
    )

    LOGGER.info("Wrote animset: %s", path)
    return path


def write_collider_profile(
    out_root: Path,
    atlas_id: str,
    collider_profile: Mapping[str, Any],
) -> Path:
    out_dir = out_root / ResourceTypeName.ANIM_COLLIDER_PROFILE
    out_dir.mkdir(parents=True, exist_ok=True)

    path = out_dir / f"{atlas_id}.json"
    path.write_text(json.dumps(collider_profile, indent=2), encoding="utf-8")
    return path
