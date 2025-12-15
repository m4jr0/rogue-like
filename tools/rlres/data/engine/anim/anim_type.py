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
from enum import IntEnum, IntFlag, StrEnum
from typing import ClassVar, Iterable

from rlres.data.engine.anim.anim_id import AnimKey
from rlres.data.engine.sprite.sprite_type import Sprite
from rlres.logger import LOGGER


class AnimFlipPolicy(IntEnum):
    UNKNOWN = 0
    KEEP = 1
    FLIP = 2
    DONT_FLIP = 3


class AnimFlipPolicyName(StrEnum):
    UNKNOWN = "unknown"
    KEEP = "keep"
    FLIP = "flip"
    DONT_FLIP = "dont_flip"


_ANIM_FLIP_NAME_TO_POLICY: dict[AnimFlipPolicyName, AnimFlipPolicy] = {
    AnimFlipPolicyName.UNKNOWN: AnimFlipPolicy.UNKNOWN,
    AnimFlipPolicyName.KEEP: AnimFlipPolicy.KEEP,
    AnimFlipPolicyName.FLIP: AnimFlipPolicy.FLIP,
    AnimFlipPolicyName.DONT_FLIP: AnimFlipPolicy.DONT_FLIP,
}


_ANIM_FLIP_POLICY_TO_NAME: dict[AnimFlipPolicy, AnimFlipPolicyName] = {
    v: k for k, v in _ANIM_FLIP_NAME_TO_POLICY.items()
}


def anim_flip_policy_name_to_policy(
    name: str | AnimFlipPolicyName,
) -> AnimFlipPolicy:
    if isinstance(name, AnimFlipPolicyName):
        enum_name = name
    else:
        try:
            enum_name = AnimFlipPolicyName(name.lower())
        except ValueError:
            LOGGER.warn(
                "Unknown %s string %r; using %s.%s",
                AnimFlipPolicyName.__name__,
                name,
                AnimFlipPolicy.__name__,
                AnimFlipPolicy.UNKNOWN.name,
            )

            return AnimFlipPolicy.UNKNOWN

    result = _ANIM_FLIP_NAME_TO_POLICY.get(enum_name)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            AnimFlipPolicyName.__name__,
            enum_name,
            AnimFlipPolicy.__name__,
            AnimFlipPolicy.__name__,
            AnimFlipPolicy.UNKNOWN.name,
        )

        return AnimFlipPolicy.UNKNOWN

    return result


def anim_flip_policy_to_name(policy: AnimFlipPolicy) -> AnimFlipPolicyName:
    result = _ANIM_FLIP_POLICY_TO_NAME.get(policy)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            AnimFlipPolicy.__name__,
            policy,
            AnimFlipPolicyName.__name__,
            AnimFlipPolicyName.__name__,
            AnimFlipPolicyName.UNKNOWN.name,
        )

        return AnimFlipPolicyName.UNKNOWN

    return result


class AnimFlag(IntFlag):
    NONE = 0x0
    LOOP = 0x1
    PING_PONG = 0x2


MAX_ANIM_KEY_PARAM_COUNT: int = 3


class AnimFlagName(StrEnum):
    NONE = "none"
    LOOP = "loop"
    PING_PONG = "ping_pong"


_ANIM_FLAG_NAME_TO_FLAG: dict[AnimFlagName, AnimFlag] = {
    AnimFlagName.NONE: AnimFlag.NONE,
    AnimFlagName.LOOP: AnimFlag.LOOP,
    AnimFlagName.PING_PONG: AnimFlag.PING_PONG,
}

_ANIM_FLAG_TO_NAME: dict[AnimFlag, AnimFlagName] = {
    v: k for k, v in _ANIM_FLAG_NAME_TO_FLAG.items()
}


def anim_flag_name_to_flag(name: AnimFlagName | str) -> AnimFlag:
    if isinstance(name, AnimFlagName):
        enum_name = name
    else:
        try:
            enum_name = AnimFlagName(name.lower())
        except ValueError:
            LOGGER.warn(
                "Unknown %s string %r; using %s.%s",
                AnimFlagName.__name__,
                name,
                AnimFlag.__name__,
                AnimFlag.NONE.name,
            )

            return AnimFlag.NONE

    result = _ANIM_FLAG_NAME_TO_FLAG.get(enum_name)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            AnimFlagName.__name__,
            enum_name,
            AnimFlag.__name__,
            AnimFlag.__name__,
            AnimFlag.NONE.name,
        )

        return AnimFlag.NONE

    return result


def anim_flag_to_name(flag: AnimFlag) -> AnimFlagName:
    result = _ANIM_FLAG_TO_NAME.get(flag)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            AnimFlag.__name__,
            flag,
            AnimFlagName.__name__,
            AnimFlagName.__name__,
            AnimFlagName.NONE.name,
        )

        return AnimFlagName.NONE

    return result


class AnimKeyTag(IntEnum):
    UNKNOWN = 0
    DEFAULT_HURTBOX_SET = 1000
    CUSTOM_HURTBOX_SET = 1001
    HIT_WALK = 1002


class AnimKeyTagName(StrEnum):
    UNKNOWN = "unknown"
    DEFAULT_HURTBOX_SET = "default_hurtbox_set"
    CUSTOM_HURTBOX_SET = "custom_hurtbox_set"
    HIT_WALK = "hit_walk"


_ANIM_KEY_TAG_NAME_TO_TAG: dict[AnimKeyTagName, AnimKeyTag] = {
    AnimKeyTagName.DEFAULT_HURTBOX_SET: AnimKeyTag.DEFAULT_HURTBOX_SET,
    AnimKeyTagName.CUSTOM_HURTBOX_SET: AnimKeyTag.CUSTOM_HURTBOX_SET,
    AnimKeyTagName.HIT_WALK: AnimKeyTag.HIT_WALK,
}

_ANIM_KEY_TAG_TO_NAME: dict[AnimKeyTag, AnimKeyTagName] = {
    v: k for k, v in _ANIM_KEY_TAG_NAME_TO_TAG.items()
}


def anim_key_tag_to_name(tag: AnimKeyTag) -> AnimKeyTagName:
    result = _ANIM_KEY_TAG_TO_NAME.get(tag)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            AnimKeyTag.__name__,
            tag,
            AnimKeyTagName.__name__,
            AnimKeyTagName.__name__,
            AnimKeyTagName.UNKNOWN.name,
        )

        return AnimKeyTagName.UNKNOWN

    return result


def anim_key_tag_name_to_tag(name: str | AnimKeyTagName) -> AnimKeyTag:
    if isinstance(name, AnimKeyTagName):
        enum_name = name
    else:
        try:
            enum_name = AnimKeyTagName(name.lower())
        except ValueError:
            LOGGER.warn(
                "Unknown %s string %r; using %s.%s",
                AnimKeyTagName.__name__,
                name,
                AnimKeyTag.__name__,
                AnimKeyTag.UNKNOWN.name,
            )

            return AnimKeyTag.UNKNOWN

    result = _ANIM_KEY_TAG_NAME_TO_TAG.get(enum_name)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            AnimKeyTagName.__name__,
            enum_name,
            AnimKeyTag.__name__,
            AnimKeyTag.__name__,
            AnimKeyTag.UNKNOWN.name,
        )

        return AnimKeyTag.UNKNOWN

    return result


@dataclass(slots=True)
class AnimSampleModel:
    sprite: Sprite
    duration: float


@dataclass(slots=True)
class AnimKeyFrameModel:
    key_tag: AnimKeyTag
    frame: int
    params: list[int]


@dataclass(slots=True)
class AnimResourceModel:
    flags: AnimFlag
    duration: float
    key: AnimKey
    samples: list[AnimSampleModel]
    acc_durations: list[float]
    keys: list[AnimKeyFrameModel]


@dataclass(slots=True)
class AnimSolvedEntryModel:
    from_key: AnimKey
    to_key: AnimKey
    flip_policy: AnimFlipPolicy


@dataclass(slots=True)
class AnimSetResourceModel:
    VERSION: ClassVar[int] = 1

    rid: int
    tex: int
    collider_profile: int
    default_anim_idx: int
    anims: list[AnimResourceModel]
    solved: list[AnimSolvedEntryModel] = field(default_factory=list)
