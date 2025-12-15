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
from enum import IntEnum, StrEnum
from typing import ClassVar, Sequence

from rlres.data.engine.physics.physics_body import PhysicsBodyDescModel
from rlres.data.game.character.character_type import CharType, CharFaction
from rlres.logger import LOGGER


class CharArchFlagBits(IntEnum):
    NONE = 0x0
    ALL = 0xFFFFFFFF


class CharArchFlagName(StrEnum):
    NONE = "none"


_CHAR_ARCH_FLAG_TO_NAME: dict[CharArchFlagBits, CharArchFlagName] = {
    CharArchFlagBits.NONE: CharArchFlagName.NONE,
}

_CHAR_ARCH_NAME_TO_FLAG: dict[CharArchFlagName, CharArchFlagBits] = {
    v: k for k, v in _CHAR_ARCH_FLAG_TO_NAME.items()
}


def char_arch_flag_name(flag: CharArchFlagBits) -> CharArchFlagName:
    result = _CHAR_ARCH_FLAG_TO_NAME.get(flag)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            CharArchFlagBits.__name__,
            flag,
            CharArchFlagName.__name__,
            CharArchFlagName.__name__,
            CharArchFlagName.NONE.name,
        )

        return CharArchFlagName.NONE

    return result


def char_arch_flag(name: CharArchFlagName | str) -> CharArchFlagBits:
    if isinstance(name, CharArchFlagName):
        enum_name = name
    else:
        try:
            enum_name = CharArchFlagName(name.lower())
        except ValueError:
            LOGGER.warn(
                "Unknown %s string %r; using %s.%s",
                CharArchFlagName.__name__,
                name,
                CharArchFlagBits.__name__,
                CharArchFlagBits.NONE.name,
            )

            return CharArchFlagBits.NONE

    result = _CHAR_ARCH_NAME_TO_FLAG.get(enum_name)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            CharArchFlagName.__name__,
            enum_name,
            CharArchFlagBits.__name__,
            CharArchFlagBits.__name__,
            CharArchFlagBits.NONE.name,
        )

        return CharArchFlagBits.NONE

    return result


@dataclass(slots=True)
class CharArchetypeResourceModel:
    VERSION: ClassVar[int] = 1

    rid: int

    type: CharType
    faction: CharFaction
    flags: int
    type_id: int

    abilities: Sequence[int]

    physics: PhysicsBodyDescModel

    anim_set: int
    start_anim: int
    sound_bank: int
