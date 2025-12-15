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
from rlres.data.game.material.material_type import MaterialResourceModel
from rlres.exporter.asset_exporter import AssetExporter
from rlres.exporter.game.material.material_serialize import write_material_file
from rlres.logger import LOGGER
from rlres.utils.safe_utils import safe_json_get, safe_load_json_object


class MaterialExporter(AssetExporter):
    type_names: Sequence[ResourceTypeName] = (ResourceTypeName.MATERIAL,)
    depends_on: Sequence[ResourceTypeName] = ()
    discover_patterns: Sequence[str] = ("*.json",)

    def _build_one(
        self,
        entry: ResourceEntry,
        _: Path,
        out_dir: Path,
        __: ResourceIndex,
    ) -> None:
        cls_name = self.__class__.__name__
        src_path = entry.src_path
        data: Mapping[str, Any] = safe_load_json_object(src_path)

        if not data:
            LOGGER.error(
                "%s: empty or invalid JSON for material %r from %s; skipping",
                cls_name,
                entry.name,
                src_path,
            )

            return

        move_scale = safe_json_get(
            data, "move_scale", expected_type=(int, float), default_value=1.0
        )

        ground_damping = safe_json_get(
            data, "ground_damping", expected_type=(int, float), default_value=0.0
        )

        friction = safe_json_get(
            data, "friction", expected_type=(int, float), default_value=0.1
        )

        restitution = safe_json_get(
            data, "restitution", expected_type=(int, float), default_value=0.0
        )

        air_drag = safe_json_get(
            data, "air_drag", expected_type=(int, float), default_value=0.0
        )

        try:
            model = MaterialResourceModel(
                rid=entry.rid,
                move_scale=float(move_scale),
                ground_damping=float(ground_damping),
                friction=float(friction),
                restitution=float(restitution),
                air_drag=float(air_drag),
            )

        except Exception as e:
            LOGGER.error(
                "%s: failed constructing %s for %r: %s",
                cls_name,
                MaterialResourceModel.__name__,
                entry.name,
                e,
            )
            return

        out_path = make_res_filepath(out_dir, entry.rid)
        write_material_file(out_path, model)
