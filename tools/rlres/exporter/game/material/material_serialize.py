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
from typing import BinaryIO

from rlres.data.engine.resource.resource_type import ResourceTypeId
from rlres.data.game.material.material_type import MaterialResourceModel

from rlres.utils.serialize_utils import (
    write_file_header,
    write_u32,
    write_f32,
)


def write_material_resource(f: BinaryIO, mat: MaterialResourceModel) -> None:
    write_u32(f, mat.rid)
    write_f32(f, mat.move_scale)
    write_f32(f, mat.ground_damping)
    write_f32(f, mat.friction)
    write_f32(f, mat.restitution)
    write_f32(f, mat.air_drag)


def write_material_file(path: Path, model: MaterialResourceModel) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)

    with path.open("wb") as f:
        write_file_header(f, ResourceTypeId.MATERIAL, model.VERSION)
        write_material_resource(f, model)
