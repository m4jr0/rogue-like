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

from typing import BinaryIO

from rlres.data.engine.physics.physics_body import PhysicsBodyDescModel
from rlres.exporter.engine.physics.collider_serialize import write_collider_desc
from rlres.utils.serialize_utils import write_bool, write_f32


def write_physics_body_desc(f: BinaryIO, phys: PhysicsBodyDescModel) -> None:
    write_bool(f, phys.dynamic)

    write_f32(f, float(phys.init_vel[0]))
    write_f32(f, float(phys.init_vel[1]))

    write_f32(f, float(phys.max_vel[0]))
    write_f32(f, float(phys.max_vel[1]))

    write_f32(f, float(phys.acc[0]))
    write_f32(f, float(phys.acc[1]))

    write_f32(f, float(phys.dec[0]))
    write_f32(f, float(phys.dec[1]))

    write_collider_desc(f, phys.collider)
