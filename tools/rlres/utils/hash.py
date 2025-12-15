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

FNV_PRIME_32 = 16777619
FNV_OFFSET_BASIS_32 = 2166136261
MASK32 = 0xFFFFFFFF


def fnv1a32_bytes(data: bytes) -> int:
    h = FNV_OFFSET_BASIS_32

    for b in data:
        h ^= b
        h = (h * FNV_PRIME_32) & MASK32

    return h


def fnv1a32(s: str, encoding: str = "utf-8") -> int:
    return fnv1a32_bytes(s.encode(encoding))
