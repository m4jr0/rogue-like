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

from enum import IntEnum, StrEnum

from rlres.logger import LOGGER


class CharType(IntEnum):
    UNKNOWN = 0
    CLASS = 1
    SPECIES = 2


class CharTypeName(StrEnum):
    UNKNOWN = "unknown"
    CLASS = "class"
    SPECIES = "species"


_CHAR_TYPE_NAME_TO_TYPE = {
    CharTypeName.UNKNOWN: CharType.UNKNOWN,
    CharTypeName.CLASS: CharType.CLASS,
    CharTypeName.SPECIES: CharType.SPECIES,
}


_CHAR_TYPE_TO_NAME: dict[CharType, CharTypeName] = {
    v: k for k, v in _CHAR_TYPE_NAME_TO_TYPE.items()
}


def char_type(name: CharTypeName | str) -> CharType:
    if isinstance(name, CharTypeName):
        result = _CHAR_TYPE_NAME_TO_TYPE.get(name)

        if result is None:
            LOGGER.warn(
                "%s %r has no %s mapping; using %s.%s",
                CharTypeName.__name__,
                name,
                CharType.__name__,
                CharType.__name__,
                CharType.UNKNOWN.name,
            )

            return CharType.UNKNOWN

        return result

    try:
        enum_name = CharTypeName(name.lower())
    except ValueError:
        LOGGER.warn(
            "Unknown %s string %r; using %s.%s",
            CharTypeName.__name__,
            name,
            CharType.__name__,
            CharType.UNKNOWN.name,
        )

        return CharType.UNKNOWN

    result = _CHAR_TYPE_NAME_TO_TYPE.get(enum_name)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            CharTypeName.__name__,
            enum_name,
            CharType.__name__,
            CharType.__name__,
            CharType.UNKNOWN.name,
        )

        return CharType.UNKNOWN

    return result


def char_type_name(ctype: CharType) -> CharTypeName:
    result = _CHAR_TYPE_TO_NAME.get(ctype)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            CharType.__name__,
            ctype,
            CharTypeName.__name__,
            CharTypeName.__name__,
            CharTypeName.UNKNOWN.name,
        )

        return CharTypeName.UNKNOWN

    return result


class CharFaction(IntEnum):
    NEUTRAL = 0
    GOOD = 1
    EVIL = 2


class CharFactionName(StrEnum):
    NEUTRAL = "neutral"
    GOOD = "good"
    EVIL = "evil"


_CHAR_FACTION_NAME_TO_FACTION = {
    CharFactionName.NEUTRAL: CharFaction.NEUTRAL,
    CharFactionName.GOOD: CharFaction.GOOD,
    CharFactionName.EVIL: CharFaction.EVIL,
}

_CHAR_FACTION_TO_NAME: dict[CharFaction, CharFactionName] = {
    v: k for k, v in _CHAR_FACTION_NAME_TO_FACTION.items()
}


def char_faction(name: CharFactionName | str) -> CharFaction:
    if isinstance(name, CharFactionName):
        result = _CHAR_FACTION_NAME_TO_FACTION.get(name)

        if result is None:
            LOGGER.warn(
                "%s %r has no %s mapping; using %s.%s",
                CharFactionName.__name__,
                name,
                CharFaction.__name__,
                CharFaction.__name__,
                CharFaction.NEUTRAL.name,
            )

            return CharFaction.NEUTRAL

        return result

    try:
        enum_name = CharFactionName(name.lower())
    except ValueError:
        LOGGER.warn(
            "Unknown %s string %r; using %s.%s",
            CharFactionName.__name__,
            name,
            CharFaction.__name__,
            CharFaction.NEUTRAL.name,
        )

        return CharFaction.NEUTRAL

    result = _CHAR_FACTION_NAME_TO_FACTION.get(enum_name)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            CharFactionName.__name__,
            enum_name,
            CharFaction.__name__,
            CharFaction.__name__,
            CharFaction.NEUTRAL.name,
        )

        return CharFaction.NEUTRAL

    return result


def char_faction_name(faction: CharFaction) -> CharFactionName:
    result = _CHAR_FACTION_TO_NAME.get(faction)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            CharFaction.__name__,
            faction,
            CharFactionName.__name__,
            CharFactionName.__name__,
            CharFactionName.NEUTRAL.name,
        )

        return CharFactionName.NEUTRAL

    return result


class CharClass(IntEnum):
    NONE = 0
    ADVENTURER = 1
    SWORDSMAN = 2


class CharClassName(StrEnum):
    NONE = "none"
    ADVENTURER = "adventurer"
    SWORDSMAN = "swordsman"


_CHAR_CLASS_NAME_TO_CLASS = {
    CharClassName.NONE: CharClass.NONE,
    CharClassName.ADVENTURER: CharClass.ADVENTURER,
    CharClassName.SWORDSMAN: CharClass.SWORDSMAN,
}


_CHAR_CLASS_TO_NAME: dict[CharClass, CharClassName] = {
    v: k for k, v in _CHAR_CLASS_NAME_TO_CLASS.items()
}


def char_class(name: CharClassName | str) -> CharClass:
    if isinstance(name, CharClassName):
        result = _CHAR_CLASS_NAME_TO_CLASS.get(name)

        if result is None:
            LOGGER.warn(
                "%s %r has no %s mapping; using %s.%s",
                CharClassName.__name__,
                name,
                CharClass.__name__,
                CharClass.__name__,
                CharClass.NONE.name,
            )

            return CharClass.NONE

        return result

    try:
        enum_name = CharClassName(name.lower())
    except ValueError:
        LOGGER.warn(
            "Unknown %s string %r; using %s.%s",
            CharClassName.__name__,
            name,
            CharClass.__name__,
            CharClass.NONE.name,
        )

        return CharClass.NONE

    result = _CHAR_CLASS_NAME_TO_CLASS.get(enum_name)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            CharClassName.__name__,
            enum_name,
            CharClass.__name__,
            CharClass.__name__,
            CharClass.NONE.name,
        )

        return CharClass.NONE

    return result


def char_class_name(cclass: CharClass) -> CharClassName:
    result = _CHAR_CLASS_TO_NAME.get(cclass)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            CharClass.__name__,
            cclass,
            CharClassName.__name__,
            CharClassName.__name__,
            CharClassName.NONE.name,
        )

        return CharClassName.NONE

    return result
