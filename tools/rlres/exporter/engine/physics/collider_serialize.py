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
from pathlib import Path
from typing import BinaryIO

from rlres.data.engine.physics.collider import ColliderDescModel, ColliderShape

from rlres.data.engine.physics.collider_profile import (
    AnimColliderProfileResourceModel,
    AnimColliderSampleModel,
    AnimColliderSetModel,
    AnimColliderTrackModel,
)

from rlres.data.engine.resource.resource_type import ResourceTypeId

from rlres.utils.serialize_utils import (
    write_f32,
    write_file_header,
    write_u32,
    write_u8,
    write_usize,
)


def write_collider_desc(f: BinaryIO, collider: ColliderDescModel) -> None:
    shape_val = int(collider.shape)
    write_u8(f, shape_val)

    cx, cy = collider.center

    if collider.shape == ColliderShape.CIRCLE:
        write_f32(f, float(cx))
        write_f32(f, float(cy))
        write_f32(f, float(collider.radius))

    elif collider.shape == ColliderShape.AABB:
        hx, hy = collider.half_extents
        write_f32(f, float(cx))
        write_f32(f, float(cy))
        write_f32(f, float(hx))
        write_f32(f, float(hy))

    else:
        # Case: UNKNOWN or unsupported. Write zeros to keep layout stable.
        write_f32(f, 0.0)
        write_f32(f, 0.0)
        write_f32(f, 0.0)
        write_f32(f, 0.0)


def _write_collider_sample(f: BinaryIO, sample: AnimColliderSampleModel) -> None:
    write_collider_desc(f, sample.collider)


def _write_collider_track(f: BinaryIO, track: AnimColliderTrackModel) -> None:
    write_usize(f, len(track.samples))

    for s in track.samples:
        _write_collider_sample(f, s)


def _write_collider_set(f: BinaryIO, cs: AnimColliderSetModel) -> None:
    write_usize(f, len(cs.hurts))

    for t in cs.hurts:
        _write_collider_track(f, t)

    write_usize(f, len(cs.hits))

    for t in cs.hits:
        _write_collider_track(f, t)


def write_collider_profile_resource(
    f: BinaryIO,
    model: AnimColliderProfileResourceModel,
) -> None:
    write_u32(f, model.rid)
    write_usize(f, len(model.per_anim))

    for collider_set in model.per_anim:
        _write_collider_set(f, collider_set)


def write_collider_profile_file(
    path: Path,
    model: AnimColliderProfileResourceModel,
) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)

    with path.open("wb") as f:
        write_file_header(
            f, ResourceTypeId.COLLIDER_PROFILE, AnimColliderProfileResourceModel.VERSION
        )

        write_collider_profile_resource(f, model)
