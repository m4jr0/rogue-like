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
from typing import Final

from rlres.logger import LOGGER


class ResourceTypeId(IntEnum):
    UNKNOWN = 0
    RESOURCE_INDEX = 1
    TEX = 2
    ATLAS = 3
    ANIM_SET = 4
    COLLIDER_PROFILE = 5
    SOUND = 6
    SOUND_BANK = 7

    # High-range custom IDs.
    CHAR_ARCHETYPE = 1000
    ABILITY = 1001
    HURTBOXSET = 1002
    MATERIAL = 1003


class ResourceTypeName(StrEnum):
    UNKNOWN = "unknown"
    RESOURCE_INDEX = "residx"
    TEX = "tex"
    ATLAS = "atlas"
    ANIM_SET = "animset"
    ANIM_COLLIDER_PROFILE = "animcolprofile"
    SOUND = "sound"
    SOUND_BANK = "soundbank"
    CHAR_ARCHETYPE = "chararch"
    ABILITY = "ability"
    MATERIAL = "mat"


class ResourceSubTypeName(StrEnum):
    UNKNOWN = "unknown"
    ANIM = "anim"


_RES_ID_TO_NAME: dict[ResourceTypeId, ResourceTypeName] = {
    ResourceTypeId.UNKNOWN: ResourceTypeName.UNKNOWN,
    ResourceTypeId.RESOURCE_INDEX: ResourceTypeName.RESOURCE_INDEX,
    ResourceTypeId.TEX: ResourceTypeName.TEX,
    ResourceTypeId.ATLAS: ResourceTypeName.ATLAS,
    ResourceTypeId.ANIM_SET: ResourceTypeName.ANIM_SET,
    ResourceTypeId.COLLIDER_PROFILE: ResourceTypeName.ANIM_COLLIDER_PROFILE,
    ResourceTypeId.SOUND: ResourceTypeName.SOUND,
    ResourceTypeId.SOUND_BANK: ResourceTypeName.SOUND_BANK,
    ResourceTypeId.CHAR_ARCHETYPE: ResourceTypeName.CHAR_ARCHETYPE,
    ResourceTypeId.ABILITY: ResourceTypeName.ABILITY,
    ResourceTypeId.MATERIAL: ResourceTypeName.MATERIAL,
}


_RES_NAME_TO_ID: dict[ResourceTypeName, ResourceTypeId] = {
    v: k for k, v in _RES_ID_TO_NAME.items()
}

ResourceId = int
INVALID_RESOURCE_ID: Final[ResourceId] = (1 << 32) - 1


def res_name(rt: ResourceTypeId) -> ResourceTypeName:
    result = _RES_ID_TO_NAME.get(rt)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            ResourceTypeId.__name__,
            rt,
            ResourceTypeName.__name__,
            ResourceTypeName.__name__,
            ResourceTypeName.UNKNOWN.name,
        )

        return ResourceTypeName.UNKNOWN

    return result


def res_id(name: str | ResourceTypeName) -> ResourceTypeId:
    """Convert a resource type-name to a ResourceTypeId."""
    if isinstance(name, ResourceTypeName):
        key = name
    else:
        try:
            key = ResourceTypeName(name.lower())
        except Exception:
            LOGGER.warn(
                "Unknown %s string %r; using %s.%s",
                ResourceTypeName.__name__,
                name,
                ResourceTypeId.__name__,
                ResourceTypeId.UNKNOWN.name,
            )

            return ResourceTypeId.UNKNOWN

    result = _RES_NAME_TO_ID.get(key)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            ResourceTypeName.__name__,
            key,
            ResourceTypeId.__name__,
            ResourceTypeId.__name__,
            ResourceTypeId.UNKNOWN.name,
        )

        return ResourceTypeId.UNKNOWN

    return result
