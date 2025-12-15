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
from enum import IntEnum
from typing import ClassVar

from rlres.data.engine.resource.resource_type import ResourceId


class TexFormat(IntEnum):
    UNKNOWN = 0
    RGBA8 = 1


@dataclass(slots=True)
class TexResourceModel:
    VERSION: ClassVar[int] = 1

    rid: ResourceId
    fmt: TexFormat
    width: int
    height: int
    data: bytes
