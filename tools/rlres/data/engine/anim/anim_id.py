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

from rlres.data.engine.physics.physics import CardinalDir
from rlres.logger import LOGGER


class AnimId(IntEnum):
    IDLE = 0
    WALK = 1
    RUN = 2
    DASH = 3
    HURT = 4
    HEAL = 5
    DEAD = 6
    ATTACK0 = 7
    ATTACK1 = 8
    ATTACK2 = 9
    INVALID = (1 << 32) - 1


class AnimName(StrEnum):
    IDLE = "idle"
    WALK = "walk"
    RUN = "run"
    DASH = "dash"
    HURT = "hurt"
    HEAL = "heal"
    DEAD = "dead"
    ATTACK0 = "attack0"
    ATTACK1 = "attack1"
    ATTACK2 = "attack2"
    INVALID = "invalid"


_ANIM_NAME_TO_ID: dict[AnimName, AnimId] = {
    AnimName.IDLE: AnimId.IDLE,
    AnimName.WALK: AnimId.WALK,
    AnimName.RUN: AnimId.RUN,
    AnimName.DASH: AnimId.DASH,
    AnimName.HURT: AnimId.HURT,
    AnimName.HEAL: AnimId.HEAL,
    AnimName.DEAD: AnimId.DEAD,
    AnimName.ATTACK0: AnimId.ATTACK0,
    AnimName.ATTACK1: AnimId.ATTACK1,
    AnimName.ATTACK2: AnimId.ATTACK2,
}

_ANIM_ID_TO_NAME: dict[AnimId, AnimName] = {v: k for k, v in _ANIM_NAME_TO_ID.items()}


def anim_id_from_name(name: AnimName | str) -> AnimId:
    if isinstance(name, AnimName):
        result = _ANIM_NAME_TO_ID.get(name)

        if result is None:
            LOGGER.warn(
                "%s %r has no %s mapping; using %s.%s",
                AnimName.__name__,
                name,
                AnimId.__name__,
                AnimId.__name__,
                AnimId.INVALID.name,
            )

            return AnimId.INVALID

        return result

    try:
        enum_name = AnimName(name.lower())
    except ValueError:
        LOGGER.warn(
            "Unknown %s string %r; using %s.%s",
            AnimName.__name__,
            name,
            AnimId.__name__,
            AnimId.INVALID.name,
        )

        return AnimId.INVALID

    result = _ANIM_NAME_TO_ID.get(enum_name)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            AnimName.__name__,
            AnimId.__name__,
            enum_name,
            AnimId.__name__,
            AnimId.INVALID.name,
        )

        return AnimId.INVALID

    return result


def anim_name_from_id(aid: AnimId) -> AnimName:
    result = _ANIM_ID_TO_NAME.get(aid)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            AnimId.__name__,
            aid,
            AnimName.__name__,
            AnimName.__name__,
            AnimName.INVALID.name,
        )

        return AnimName.INVALID

    return result


AnimKey = int
INVALID_ANIM_KEY: AnimKey = (1 << 64) - 1


def make_anim_key(nid: AnimId, direction: CardinalDir) -> AnimKey:
    return (int(nid) << 32) | int(direction)


def split_anim_key(key: AnimKey) -> tuple[AnimId, CardinalDir]:
    nid = AnimId((key >> 32) & 0xFFFFFFFF)
    direction = CardinalDir(key & 0xFFFFFFFF)
    return nid, direction
