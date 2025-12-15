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

from typing import BinaryIO, SupportsInt

RESOURCE_FILE_MAGIC: int = int.from_bytes(b"RLRS", byteorder="big")

_S8 = struct.Struct("<b")
_U8 = struct.Struct("<B")
_S16 = struct.Struct("<h")
_U16 = struct.Struct("<H")
_S32 = struct.Struct("<i")
_U32 = struct.Struct("<I")
_S64 = struct.Struct("<q")
_U64 = struct.Struct("<Q")
_F32 = struct.Struct("<f")
_F64 = struct.Struct("<d")


def write_bool(f: BinaryIO, value: bool) -> None:
    write_u8(f, 1 if value else 0)


def write_s8(f: BinaryIO, v: int) -> None:
    f.write(_S8.pack(v))


def write_u8(f: BinaryIO, v: int) -> None:
    f.write(_U8.pack(v))


def write_s16(f: BinaryIO, v: int) -> None:
    f.write(_S16.pack(v))


def write_u16(f: BinaryIO, v: int) -> None:
    f.write(_U16.pack(v))


def write_s32(f: BinaryIO, v: int) -> None:
    f.write(_S32.pack(v))


def write_u32(f: BinaryIO, v: int) -> None:
    f.write(_U32.pack(v))


def write_s64(f: BinaryIO, v: int) -> None:
    f.write(_S64.pack(v))


def write_u64(f: BinaryIO, v: int) -> None:
    f.write(_U64.pack(v))


def write_f32(f: BinaryIO, v: float) -> None:
    f.write(_F32.pack(v))


def write_f64(f: BinaryIO, v: float) -> None:
    f.write(_F64.pack(v))


def write_usize(f: BinaryIO, v: int) -> None:
    f.write(_U64.pack(v))


def write_ssize(f: BinaryIO, v: int) -> None:
    f.write(_S64.pack(v))


def write_vector_len(f: BinaryIO, n: int) -> None:
    write_u64(f, n)


def write_file_header(
    f: BinaryIO,
    resource_type_id: SupportsInt,
    version: SupportsInt,
) -> None:
    write_u32(f, RESOURCE_FILE_MAGIC)
    write_u32(f, int(resource_type_id))
    write_u32(f, int(version))


def read_bool(f: BinaryIO) -> bool:
    return read_u8(f) != 0


def read_s8(f: BinaryIO) -> int:
    return _S8.unpack(f.read(1))[0]


def read_u8(f: BinaryIO) -> int:
    return _U8.unpack(f.read(1))[0]


def read_s16(f: BinaryIO) -> int:
    return _S16.unpack(f.read(2))[0]


def read_u16(f: BinaryIO) -> int:
    return _U16.unpack(f.read(2))[0]


def read_s32(f: BinaryIO) -> int:
    return _S32.unpack(f.read(4))[0]


def read_u32(f: BinaryIO) -> int:
    return _U32.unpack(f.read(4))[0]


def read_s64(f: BinaryIO) -> int:
    return _S64.unpack(f.read(8))[0]


def read_u64(f: BinaryIO) -> int:
    return _U64.unpack(f.read(8))[0]


def read_f32(f: BinaryIO) -> float:
    return _F32.unpack(f.read(4))[0]


def read_f64(f: BinaryIO) -> float:
    return _F64.unpack(f.read(8))[0]


def read_usize(f: BinaryIO) -> int:
    return _U64.unpack(f.read(8))[0]


def read_ssize(f: BinaryIO) -> int:
    return _S64.unpack(f.read(8))[0]


def read_vector_len(f: BinaryIO) -> int:
    return read_u64(f)


def read_file_header(f: BinaryIO) -> tuple[int, int, int]:
    magic = read_u32(f)
    res_type = read_u32(f)
    version = read_u32(f)
    return magic, res_type, version


def f32_to_bits(value: float) -> int:
    return _U32.unpack(_F32.pack(float(value)))[0]
