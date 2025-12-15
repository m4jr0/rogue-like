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

from dataclasses import dataclass, field
from enum import IntEnum, StrEnum
from typing import ClassVar, Sequence

from rlres.logger import LOGGER


class AbilityFlagBits(IntEnum):
    NONE = 0x0
    LOCK_MOVEMENT = 0x1
    UNINTERRUPT = 0x2
    ALL = 0xFFFFFFFF


class AbilityFlagName(StrEnum):
    NONE = "none"
    LOCK_MOVEMENT = "lock_movement"
    UNINTERRUPT = "uninterrupt"


_ABILITY_FLAG_TO_NAME: dict[AbilityFlagBits, AbilityFlagName] = {
    AbilityFlagBits.NONE: AbilityFlagName.NONE,
    AbilityFlagBits.LOCK_MOVEMENT: AbilityFlagName.LOCK_MOVEMENT,
    AbilityFlagBits.UNINTERRUPT: AbilityFlagName.UNINTERRUPT,
}

_ABILITY_NAME_TO_FLAG: dict[AbilityFlagName, AbilityFlagBits] = {
    v: k for k, v in _ABILITY_FLAG_TO_NAME.items()
}


def ability_flag_name(flag: AbilityFlagBits) -> AbilityFlagName:
    result = _ABILITY_FLAG_TO_NAME.get(flag)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            AbilityFlagBits.__name__,
            flag,
            AbilityFlagName.__name__,
            AbilityFlagName.__name__,
            AbilityFlagName.NONE.name,
        )

        return AbilityFlagName.NONE

    return result


def ability_flag(name: AbilityFlagName | str) -> AbilityFlagBits:
    if isinstance(name, AbilityFlagName):
        enum_name = name
    else:
        try:
            enum_name = AbilityFlagName(name.lower())
        except ValueError:
            LOGGER.warn(
                "Unknown %s string %r; using %s.%s",
                AbilityFlagName.__name__,
                name,
                AbilityFlagBits.__name__,
                AbilityFlagBits.NONE.name,
            )

            return AbilityFlagBits.NONE

    result = _ABILITY_NAME_TO_FLAG.get(enum_name)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            AbilityFlagName.__name__,
            enum_name,
            AbilityFlagBits.__name__,
            AbilityFlagBits.__name__,
            AbilityFlagBits.NONE.name,
        )

        return AbilityFlagBits.NONE

    return result


@dataclass(slots=True)
class AbilityInstructionModel:
    op: int
    params: Sequence[int]


@dataclass(slots=True)
class AbilityProgramModel:
    code: list[AbilityInstructionModel] = field(default_factory=list)


@dataclass(slots=True)
class AbilityResourceModel:
    VERSION: ClassVar[int] = 1

    rid: int

    stamina: float
    mana: float
    flags: int

    on_begin: AbilityProgramModel
    on_tick: AbilityProgramModel
    on_end: AbilityProgramModel
