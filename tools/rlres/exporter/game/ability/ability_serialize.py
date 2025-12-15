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

from rlres.data.engine.resource.resource_type import ResourceTypeId

from rlres.data.game.ability.ability_type import (
    AbilityProgramModel,
    AbilityResourceModel,
)

from rlres.utils.serialize_utils import (
    write_f32,
    write_file_header,
    write_u32,
    write_u64,
)


MAX_ABILITY_PARAM_COUNT: int = 5


def write_param_set(f: BinaryIO, params: Sequence[int]) -> None:
    vals = list(params[:MAX_ABILITY_PARAM_COUNT])

    while len(vals) < MAX_ABILITY_PARAM_COUNT:
        vals.append(0)

    for v in vals:
        write_u64(f, v & 0xFFFFFFFF)


def write_program(f: BinaryIO, prog: AbilityProgramModel) -> None:
    write_u64(f, len(prog.code))

    for inst in prog.code:
        write_u32(f, inst.op)
        write_param_set(f, inst.params)


def write_ability_resource(f: BinaryIO, model: AbilityResourceModel) -> None:
    write_u32(f, model.rid)

    write_f32(f, model.stamina)
    write_f32(f, model.mana)

    write_u32(f, model.flags)

    write_program(f, model.on_begin)
    write_program(f, model.on_tick)
    write_program(f, model.on_end)


def write_ability_file(path: Path, model: AbilityResourceModel) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)

    with path.open("wb") as f:
        write_file_header(f, ResourceTypeId.ABILITY, AbilityResourceModel.VERSION)
        write_ability_resource(f, model)
