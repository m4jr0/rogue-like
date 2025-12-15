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

from dataclasses import dataclass

Offset = tuple[float, float]
OffsetPair = tuple[Offset, Offset]


@dataclass(slots=True)
class AsepriteRect:
    x: int
    y: int
    w: int
    h: int


@dataclass(slots=True)
class AsepriteSize:
    w: int
    h: int


Ms = int


@dataclass(slots=True)
class AsepriteFrame:
    frame: AsepriteRect
    sprite_source_size: AsepriteRect
    source_size: AsepriteSize
    duration_ms: Ms


@dataclass(slots=True)
class AsepriteFrameEntry:
    name: str
    data: AsepriteFrame
