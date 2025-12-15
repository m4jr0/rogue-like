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
from typing import Any, ClassVar, Mapping, Sequence

from rlres.converter.aseprite.aseprite_build import (
    build_atlas_sprites,
    build_entries_and_offsets,
    build_frame_names,
    build_per_frame_pivots,
)

from rlres.converter.aseprite.aseprite_collider_profile import (
    build_collider_profile_json,
)

from rlres.converter.aseprite.aseprite_pivot import pivot_from_meta
from rlres.converter.aseprite.aseprite_texture import process_texture

from rlres.converter.aseprite.aseprite_write import (
    write_animset,
    write_atlas,
    write_collider_profile,
)

from rlres.converter.asset_converter import AssetConverter

from rlres.converter.aseprite.aseprite_frame import (
    GroupedFrameData,
    build_frame_data,
    normalize_ase_frames,
)

from rlres.logger import LOGGER
from rlres.utils.safe_utils import safe_json_get, safe_load_json_object


class AsepriteConverter(AssetConverter):
    subdir: ClassVar[str] = "aseprite"
    discover_patterns: ClassVar[Sequence[str]] = ("*.json",)

    def _convert_one(self, src_path: Path, _: Path, out_root: Path) -> None:
        # Case: Meta-only JSON files. Those will be used for additional data.
        if src_path.name.endswith(".meta.json"):
            return

        atlas_id = src_path.stem
        tex, dst_texture_path = process_texture(src_path, out_root)

        sheet = self._load_sheet(src_path)
        meta = self._load_meta(src_path)

        (
            meta_anims_by_name,
            default_anim,
            solver_entries,
        ) = self._parse_meta_anims(meta)

        raw_frames = normalize_ase_frames(sheet)

        frame_data: GroupedFrameData = build_frame_data(raw_frames)
        frame_names = build_frame_names(sheet, frame_data.atlas_indexed)

        meta_section = safe_json_get(sheet, "meta", Mapping, {}, mandatory=False)
        tags = safe_json_get(meta_section, "frameTags", list, [], mandatory=False)

        default_pivot = pivot_from_meta(meta)

        pivots = build_per_frame_pivots(
            atlas_frames=frame_data.atlas_indexed,
            pivots=frame_data.pivot,
            default_pivot=default_pivot,
        )

        entries, offsets = build_entries_and_offsets(
            atlas_frames=frame_data.atlas_indexed,
            names=frame_names,
            pivots=pivots,
        )

        sprites = build_atlas_sprites(entries, offsets)

        collider_profile = build_collider_profile_json(
            atlas_id=atlas_id,
            sheet=sheet,
            atlas_frames=frame_data.atlas_indexed,
            hurt_tracks=frame_data.hurt,
            hit_tracks=frame_data.hit,
            pivots=pivots,
        )

        atlas_path = write_atlas(
            out_root=out_root,
            atlas_id=atlas_id,
            tex=tex,
            sprites=sprites,
        )

        anim_path = write_animset(
            out_root=out_root,
            atlas_id=atlas_id,
            tex=tex,
            tags=tags,
            entries=entries,
            offsets=offsets,
            meta_anims_by_name=meta_anims_by_name,
            default_anim=default_anim,
            solver_entries=solver_entries,
        )

        collider_path = write_collider_profile(
            out_root=out_root,
            atlas_id=atlas_id,
            collider_profile=collider_profile,
        )

        LOGGER.info(
            "[Aseprite] %s -> %s, %s, %s (and %s)",
            src_path.name,
            atlas_path.relative_to(out_root),
            anim_path.relative_to(out_root),
            collider_path.relative_to(out_root),
            dst_texture_path.relative_to(out_root),
        )

    @staticmethod
    def _load_sheet(sheet_path: Path) -> dict[str, Any]:
        return safe_load_json_object(sheet_path)

    @staticmethod
    def _load_meta(sheet_path: Path) -> dict[str, Any] | None:
        meta_path = sheet_path.with_suffix(".meta.json")

        if not meta_path.exists():
            return None

        return safe_load_json_object(meta_path)

    @staticmethod
    def _parse_meta_anims(
        meta: dict[str, Any] | None,
    ) -> tuple[dict[str, dict[str, Any]], str | None, list[dict[str, Any]]]:
        meta_anims_by_name: dict[str, dict[str, Any]] = {}
        default_anim: str | None = None
        solver_entries: list[dict[str, Any]] = []

        if meta is None:
            return meta_anims_by_name, default_anim, solver_entries

        raw_anims = safe_json_get(meta, "anims", list, [], mandatory=False)

        for raw_anim in raw_anims:
            if not isinstance(raw_anim, dict):
                LOGGER.warn("Ignoring non-dict animation entry in meta: %r", raw_anim)
                continue

            name = safe_json_get(raw_anim, "name", str, "", mandatory=True)

            if not name:
                LOGGER.warn(
                    "Ignoring animation entry with empty or missing name: %r", raw_anim
                )

                continue

            meta_anims_by_name[name] = raw_anim

        default_anim_name = safe_json_get(
            meta, "default_anim", str, "", mandatory=False
        )

        if default_anim_name:
            default_anim = default_anim_name

        raw_solved = safe_json_get(meta, "solved", list, [], mandatory=False)

        for s in raw_solved:
            if isinstance(s, dict):
                solver_entries.append(s)
            else:
                LOGGER.warn("Ignoring non-dict solved entry in meta: %r", s)

        return meta_anims_by_name, default_anim, solver_entries
