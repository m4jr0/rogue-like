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
from rlres.data.engine.sprite.atlas_type import AtlasResourceModel
from rlres.exporter.engine.sprite.sprite_serialize import write_sprite
from rlres.utils.serialize_utils import write_file_header, write_u32, write_vector_len


def write_atlas_resource(f: BinaryIO, atlas: AtlasResourceModel) -> None:
    write_u32(f, atlas.rid)
    write_u32(f, atlas.tex)

    write_vector_len(f, len(atlas.sprites))

    for entry in atlas.sprites:
        name_bytes = entry.name.encode("utf-8")
        write_u32(f, len(name_bytes))
        f.write(name_bytes)

        write_sprite(f, entry.sprite)


def write_atlas_file(path: Path, atlas: AtlasResourceModel) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)

    with path.open("wb") as f:
        write_file_header(f, ResourceTypeId.ATLAS, AtlasResourceModel.VERSION)
        write_atlas_resource(f, atlas)
