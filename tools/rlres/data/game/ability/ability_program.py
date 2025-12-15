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


class AbilityOp(IntEnum):
    NO_OP = 0
    SLEEP = 1
    PLAY_ANIM = 2
    WAIT_ANIM = 3
    SET_ANIM_SPEED = 4
    SET_GCD = 5
    SET_CD = 6
    DAMAGE = 7
    SPAWN_HITBOX = 8
    SPAWN_STICKY_HITBOX = 9
    DASH = 10
    END = 11


class AbilityOpName(StrEnum):
    NO_OP = "no_op"
    SLEEP = "sleep"
    PLAY_ANIM = "play_anim"
    WAIT_ANIM = "wait_anim"
    SET_ANIM_SPEED = "set_anim_speed"
    SET_GCD = "set_gcd"
    SET_CD = "set_cd"
    DAMAGE = "damage"
    SPAWN_HITBOX = "spawn_hitbox"
    SPAWN_STICKY_HITBOX = "spawn_sticky_hitbox"
    DASH = "dash"
    END = "end"


_ABILITY_OP_TO_NAME: dict[AbilityOp, AbilityOpName] = {
    AbilityOp.NO_OP: AbilityOpName.NO_OP,
    AbilityOp.SLEEP: AbilityOpName.SLEEP,
    AbilityOp.PLAY_ANIM: AbilityOpName.PLAY_ANIM,
    AbilityOp.WAIT_ANIM: AbilityOpName.WAIT_ANIM,
    AbilityOp.SET_ANIM_SPEED: AbilityOpName.SET_ANIM_SPEED,
    AbilityOp.SET_GCD: AbilityOpName.SET_GCD,
    AbilityOp.SET_CD: AbilityOpName.SET_CD,
    AbilityOp.DAMAGE: AbilityOpName.DAMAGE,
    AbilityOp.SPAWN_HITBOX: AbilityOpName.SPAWN_HITBOX,
    AbilityOp.SPAWN_STICKY_HITBOX: AbilityOpName.SPAWN_STICKY_HITBOX,
    AbilityOp.DASH: AbilityOpName.DASH,
    AbilityOp.END: AbilityOpName.END,
}

_ABILITY_NAME_TO_OP: dict[AbilityOpName, AbilityOp] = {
    v: k for k, v in _ABILITY_OP_TO_NAME.items()
}


def ability_op_name(op: AbilityOp) -> AbilityOpName:
    result = _ABILITY_OP_TO_NAME.get(op)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            AbilityOp.__name__,
            op,
            AbilityOpName.__name__,
            AbilityOpName.__name__,
            AbilityOpName.NO_OP.name,
        )

        return AbilityOpName.NO_OP

    return result


def ability_op(name: AbilityOpName | str) -> AbilityOp:
    if isinstance(name, AbilityOpName):
        enum_name = name
    else:
        try:
            enum_name = AbilityOpName(name.lower())
        except ValueError:
            LOGGER.warn(
                "Unknown %s string %r; using %s.%s",
                AbilityOpName.__name__,
                name,
                AbilityOp.__name__,
                AbilityOp.NO_OP.name,
            )

            return AbilityOp.NO_OP

    result = _ABILITY_NAME_TO_OP.get(enum_name)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            AbilityOpName.__name__,
            enum_name,
            AbilityOp.__name__,
            AbilityOp.__name__,
            AbilityOp.NO_OP.name,
        )

        return AbilityOp.NO_OP

    return result
