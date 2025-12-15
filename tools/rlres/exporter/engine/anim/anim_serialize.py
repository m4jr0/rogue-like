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
from typing import BinaryIO, Sequence

from rlres.data.engine.anim.anim_type import (
    MAX_ANIM_KEY_PARAM_COUNT,
    AnimKeyFrameModel,
    AnimResourceModel,
    AnimSampleModel,
    AnimSetResourceModel,
    AnimSolvedEntryModel,
)

from rlres.data.engine.param_set.param_set_serialize import write_param_set
from rlres.data.engine.resource.resource_type import ResourceTypeId
from rlres.exporter.engine.sprite.sprite_serialize import write_sprite

from rlres.utils.serialize_utils import (
    write_f32,
    write_file_header,
    write_u32,
    write_u64,
    write_u8,
    write_usize,
    write_vector_len,
)


def write_anim_sample(f: BinaryIO, s: AnimSampleModel) -> None:
    write_sprite(f, s.sprite)
    write_f32(f, s.duration)


def write_keyframes(f: BinaryIO, keys: Sequence[AnimKeyFrameModel]) -> None:
    write_vector_len(f, len(keys))

    for key in keys:
        write_u32(f, key.key_tag)
        write_u32(f, key.frame)
        write_param_set(f, key.params, MAX_ANIM_KEY_PARAM_COUNT)


def write_anim_resource(f: BinaryIO, a: AnimResourceModel) -> None:
    write_u32(f, a.flags)
    write_f32(f, a.duration)
    write_u64(f, a.key)

    write_vector_len(f, len(a.samples))

    for s in a.samples:
        write_anim_sample(f, s)

    write_vector_len(f, len(a.acc_durations))

    for d in a.acc_durations:
        write_f32(f, d)

    write_keyframes(f, a.keys)


def write_anim_solved(f: BinaryIO, solver: list[AnimSolvedEntryModel]) -> None:
    write_vector_len(f, len(solver))

    for entry in solver:
        write_u64(f, int(entry.from_key))
        write_u64(f, int(entry.to_key))
        write_u8(f, int(entry.flip_policy))


def write_anim_set_resource(f: BinaryIO, aset: AnimSetResourceModel) -> None:
    write_u32(f, aset.rid)
    write_u32(f, aset.tex)
    write_u32(f, aset.collider_profile)
    write_usize(f, aset.default_anim_idx)

    write_vector_len(f, len(aset.anims))

    for a in aset.anims:
        write_anim_resource(f, a)

    write_anim_solved(f, aset.solved)


def write_anim_set_file(path: Path, aset: AnimSetResourceModel) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)

    with path.open("wb") as f:
        write_file_header(f, ResourceTypeId.ANIM_SET, AnimSetResourceModel.VERSION)
        write_anim_set_resource(f, aset)
