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
from rlres.data.engine.texture.texture_type import TexResourceModel

from rlres.utils.serialize_utils import (
    write_file_header,
    write_u8,
    write_u16,
    write_u32,
    write_u64,
)


def write_texture_resource(f: BinaryIO, tex: TexResourceModel) -> None:
    write_u32(f, tex.rid)
    write_u8(f, int(tex.fmt))

    write_u16(f, tex.width)
    write_u16(f, tex.height)

    write_u64(f, len(tex.data))
    f.write(tex.data)


def write_texture_file(path: Path, model: TexResourceModel) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)

    with path.open("wb") as f:
        write_file_header(f, ResourceTypeId.TEX, TexResourceModel.VERSION)
        write_texture_resource(f, model)
