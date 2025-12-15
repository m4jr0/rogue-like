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


class CardinalDir(IntEnum):
    UNSET = 0
    FRONT = 1
    LEFT = 2
    RIGHT = 3
    BACK = 4


class CardinalDirName(StrEnum):
    UNSET = "unset"
    FRONT = "front"
    LEFT = "left"
    RIGHT = "right"
    BACK = "back"


_CARDINAL_DIR_TO_NAME: dict[CardinalDir, CardinalDirName] = {
    CardinalDir.UNSET: CardinalDirName.UNSET,
    CardinalDir.FRONT: CardinalDirName.FRONT,
    CardinalDir.LEFT: CardinalDirName.LEFT,
    CardinalDir.RIGHT: CardinalDirName.RIGHT,
    CardinalDir.BACK: CardinalDirName.BACK,
}

_CARDINAL_NAME_TO_DIR: dict[CardinalDirName, CardinalDir] = {
    v: k for k, v in _CARDINAL_DIR_TO_NAME.items()
}


def cardinal_dir_name(card_dir: CardinalDir) -> CardinalDirName:
    result = _CARDINAL_DIR_TO_NAME.get(card_dir)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            CardinalDir.__name__,
            card_dir,
            CardinalDirName.__name__,
            CardinalDirName.__name__,
            CardinalDirName.UNSET.name,
        )

        return CardinalDirName.UNSET

    return result


def cardinal_dir(name: CardinalDirName | str) -> CardinalDir:
    if isinstance(name, CardinalDirName):
        enum_name = name
    else:
        try:
            enum_name = CardinalDirName(name.lower())
        except ValueError:
            LOGGER.warn(
                "Unknown %s string %r; using %s.%s",
                CardinalDirName.__name__,
                name,
                CardinalDir.__name__,
                CardinalDir.UNSET.name,
            )

            return CardinalDir.UNSET

    result = _CARDINAL_NAME_TO_DIR.get(enum_name)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            CardinalDirName.__name__,
            enum_name,
            CardinalDir.__name__,
            CardinalDir.__name__,
            CardinalDir.UNSET.name,
        )

        return CardinalDir.UNSET

    return result
