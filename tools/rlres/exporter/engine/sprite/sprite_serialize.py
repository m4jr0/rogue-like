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

import struct

from typing import BinaryIO

from rlres.data.engine.sprite.sprite_type import Sprite


_RECT_PACK = struct.Struct("<iiii")
_OFFSET_PACK = struct.Struct("<ff")
_SOURCE_SIZE_PACK = struct.Struct("<ii")


def write_sprite(f: BinaryIO, s: Sprite) -> None:
    f.write(_RECT_PACK.pack(s.rect.x, s.rect.y, s.rect.w, s.rect.h))
    f.write(_OFFSET_PACK.pack(*s.offset))
    f.write(_OFFSET_PACK.pack(*s.offset_flipped))
    f.write(_SOURCE_SIZE_PACK.pack(*s.source_size))
