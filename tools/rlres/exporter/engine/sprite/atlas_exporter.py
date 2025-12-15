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

from rlres.data.engine.resource.index_builder import ResourceEntry, ResourceIndex
from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.data.engine.resource.resource_utils import make_res_filepath
from rlres.data.engine.sprite.atlas_type import AtlasResourceModel, AtlasSpriteEntry
from rlres.data.engine.sprite.sprite_type import Sprite, SpriteRect
from rlres.exporter.asset_exporter import AssetExporter
from rlres.exporter.engine.sprite.atlas_serialize import write_atlas_file
from rlres.logger import LOGGER
from rlres.utils.json_utils import rid_from_key
from rlres.utils.safe_utils import safe_json_get, safe_load_json_object


class AtlasExporter(AssetExporter):
    type_names: Sequence[ResourceTypeName] = (ResourceTypeName.ATLAS,)
    depends_on: Sequence[ResourceTypeName] = (ResourceTypeName.TEX,)
    discover_patterns: Sequence[str] = ("*.json",)

    def _build_one(
        self,
        entry: ResourceEntry,
        _: Path,
        out_dir: Path,
        index: ResourceIndex,
    ) -> None:
        src_path = entry.src_path

        data: Mapping[str, Any] = safe_load_json_object(src_path)

        if not data:
            LOGGER.error(
                "%s: empty or invalid JSON for atlas %r from %s; skipping",
                self.__class__.__name__,
                entry.nid,
                src_path,
            )

            return

        tex_rid = rid_from_key(index, data, ResourceTypeName.TEX, "texture")

        raw_sprites = safe_json_get(
            data,
            "sprites",
            expected_type=list,
            default_value=[],
            mandatory=False,
        )

        if not raw_sprites:
            LOGGER.warn(
                "%s: atlas JSON for %r has no 'sprites' field or it is empty; "
                "producing an empty atlas",
                self.__class__.__name__,
                entry.nid,
            )

        sprite_entries: list[AtlasSpriteEntry] = []

        for raw_sprite in raw_sprites:
            if not isinstance(raw_sprite, Mapping):
                LOGGER.warn(
                    "%s: atlas sprite entry is not a mapping; skipping. Raw=%r",
                    self.__class__.__name__,
                    raw_sprite,
                )

                continue

            name = safe_json_get(
                raw_sprite,
                "name",
                expected_type=str,
                default_value="",
            )

            rect = safe_json_get(
                raw_sprite,
                "rect",
                expected_type=list,
                default_value=[0, 0, 0, 0],
            )

            source_size = safe_json_get(
                raw_sprite,
                "source_size",
                expected_type=list,
                default_value=[0, 0],
            )

            offset = safe_json_get(
                raw_sprite,
                "offset",
                expected_type=list,
                default_value=[0.0, 0.0],
            )

            offset_flipped = safe_json_get(
                raw_sprite,
                "offset_flipped",
                expected_type=list,
                default_value=[0.0, 0.0],
            )

            if (
                not name
                or len(rect) != 4
                or len(source_size) != 2
                or len(offset) != 2
                or len(offset_flipped) != 2
            ):
                LOGGER.warn(
                    "%s: atlas sprite entry has invalid name/rect/source_size/offset; "
                    "skipping. Raw=%r",
                    self.__class__.__name__,
                    raw_sprite,
                )

                continue

            try:
                rx, ry, rw, rh = rect
                sw, sh = source_size
                ox, oy = offset
                ofx, ofy = offset_flipped

            except ValueError as e:
                LOGGER.warn(
                    "%s: failed to unpack atlas sprite arrays; skipping. Raw=%r (%s)",
                    self.__class__.__name__,
                    raw_sprite,
                    e,
                )

                continue

            sprite_rect = SpriteRect(
                x=int(rx),
                y=int(ry),
                w=int(rw),
                h=int(rh),
            )

            sprite = Sprite(
                rect=sprite_rect,
                offset=(float(ox), float(oy)),
                offset_flipped=(float(ofx), float(ofy)),
                source_size=(int(sw), int(sh)),
            )

            sprite_entries.append(AtlasSpriteEntry(name=name, sprite=sprite))

        model = AtlasResourceModel(
            rid=entry.rid,
            tex=tex_rid,
            sprites=sprite_entries,
        )

        out_path = make_res_filepath(out_dir, entry.rid)
        write_atlas_file(out_path, model)
