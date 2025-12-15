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
from typing import Any, Mapping

from rlres.logger import LOGGER
from rlres.utils.json_utils import json_vec2
from rlres.utils.safe_utils import safe_json_get


class ColliderShape(IntEnum):
    UNKNOWN = 0
    AABB = 1
    CIRCLE = 2


class ColliderShapeName(StrEnum):
    UNKNOWN = "unknown"
    AABB = "aabb"
    CIRCLE = "circle"


_COLLIDER_SHAPE_TO_NAME: dict[ColliderShape, ColliderShapeName] = {
    ColliderShape.UNKNOWN: ColliderShapeName.UNKNOWN,
    ColliderShape.AABB: ColliderShapeName.AABB,
    ColliderShape.CIRCLE: ColliderShapeName.CIRCLE,
}

_COLLIDER_SHAPE_NAME_TO_SHAPE: dict[ColliderShapeName, ColliderShape] = {
    v: k for k, v in _COLLIDER_SHAPE_TO_NAME.items()
}


def collider_shape_name(shape: ColliderShape) -> ColliderShapeName:
    result = _COLLIDER_SHAPE_TO_NAME.get(shape)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            ColliderShape.__name__,
            shape,
            ColliderShapeName.__name__,
            ColliderShapeName.__name__,
            ColliderShapeName.UNKNOWN.name,
        )

        return ColliderShapeName.UNKNOWN

    return result


def collider_shape(name: ColliderShapeName | str) -> ColliderShape:
    if isinstance(name, ColliderShapeName):
        enum_name = name
    else:
        try:
            enum_name = ColliderShapeName(name.lower())
        except ValueError:
            LOGGER.warn(
                "Unknown %s string %r; using %s.%s",
                ColliderShapeName.__name__,
                name,
                ColliderShape.__name__,
                ColliderShape.UNKNOWN.name,
            )

            return ColliderShape.UNKNOWN

    result = _COLLIDER_SHAPE_NAME_TO_SHAPE.get(enum_name)

    if result is None:
        LOGGER.warn(
            "%s %r has no %s mapping; using %s.%s",
            ColliderShapeName.__name__,
            enum_name,
            ColliderShape.__name__,
            ColliderShape.__name__,
            ColliderShape.UNKNOWN.name,
        )

        return ColliderShape.UNKNOWN

    return result


@dataclass(slots=True)
class ColliderDescModel:
    shape: ColliderShape = ColliderShape.UNKNOWN
    center: tuple[float, float] = (0.0, 0.0)
    half_extents: tuple[float, float] = (0.0, 0.0)
    radius: float = 0.0


def parse_collider_desc(data: Mapping[str, Any]) -> ColliderDescModel:
    func_name = parse_collider_desc.__name__

    shape_name = safe_json_get(
        data,
        "shape",
        expected_type=str,
        default_value=ColliderShapeName.UNKNOWN.value,
        mandatory=False,
    )

    shape = collider_shape(shape_name)

    center = json_vec2(data, "center", (0.0, 0.0))
    half_extents = json_vec2(data, "half_extents", (0.0, 0.0))

    radius_val = safe_json_get(
        data,
        "radius",
        expected_type=(int, float),
        default_value=0.0,
        mandatory=False,
    )

    try:
        radius = float(radius_val)
    except (TypeError, ValueError):
        LOGGER.warn(
            "%s: invalid %r value %r; using 0.0",
            func_name,
            "radius",
            radius_val,
        )

        radius = 0.0

    return ColliderDescModel(
        shape=shape,
        center=center,
        half_extents=half_extents,
        radius=radius,
    )
