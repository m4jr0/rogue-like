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
from typing import Any, Mapping, Sequence

from rlres.data.engine.physics.collider import (
    ColliderDescModel,
    parse_collider_desc,
)

from rlres.utils.json_utils import json_vec2
from rlres.utils.safe_utils import safe_json_get


@dataclass(slots=True)
class PhysicsBodyDescModel:
    dynamic: bool = False

    init_vel: Sequence[float] = (0.0, 0.0)
    max_vel: Sequence[float] = (100.0, 100.0)
    acc: Sequence[float] = (60.0, 60.0)
    dec: Sequence[float] = (50.0, 50.0)

    collider: ColliderDescModel = field(default_factory=ColliderDescModel)


def parse_physics_body_desc(data: Mapping[str, Any]) -> PhysicsBodyDescModel:
    dynamic = safe_json_get(
        data,
        "dynamic",
        expected_type=bool,
        default_value=True,
        mandatory=False,
    )

    init_vel = json_vec2(data, "init_vel", (0.0, 0.0))
    max_vel = json_vec2(data, "max_vel", (100.0, 100.0))
    acc = json_vec2(data, "acc", (60.0, 60.0))
    dec = json_vec2(data, "dec", (50.0, 50.0))

    collider_data = safe_json_get(
        data,
        "collider",
        expected_type=Mapping,
        default_value={},
        mandatory=False,
    )

    collider = parse_collider_desc(collider_data)

    return PhysicsBodyDescModel(
        dynamic=dynamic,
        init_vel=init_vel,
        max_vel=max_vel,
        acc=acc,
        dec=dec,
        collider=collider,
    )
