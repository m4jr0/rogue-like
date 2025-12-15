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

from rlres.data.engine.resource.resource_type import INVALID_RESOURCE_ID, ResourceTypeId

from rlres.data.game.character.character_archetype_type import (
    CharArchetypeResourceModel,
)

from rlres.exporter.engine.physics.physics_body_serialize import write_physics_body_desc

from rlres.utils.serialize_utils import (
    write_file_header,
    write_u16,
    write_u32,
    write_u8,
)


MAX_ABILITY_SLOT_COUNT: int = 8


def write_char_arch_abilities(f: BinaryIO, ability_ids: list[int]) -> None:
    slot_count = min(len(ability_ids), MAX_ABILITY_SLOT_COUNT)
    write_u8(f, slot_count)
    padded = ability_ids[:MAX_ABILITY_SLOT_COUNT]

    while len(padded) < MAX_ABILITY_SLOT_COUNT:
        padded.append(INVALID_RESOURCE_ID)

    for rid in padded:
        write_u32(f, rid)


def write_char_archetype_resource(
    f: BinaryIO, model: CharArchetypeResourceModel
) -> None:
    write_u32(f, model.rid)

    write_u32(f, int(model.type))
    write_u32(f, int(model.faction))
    write_u32(f, model.flags)
    write_u16(f, model.type_id)

    write_char_arch_abilities(f, list(model.abilities))

    write_physics_body_desc(f, model.physics)

    write_u32(f, model.anim_set)
    write_u32(f, model.start_anim)
    write_u32(f, model.sound_bank)


def write_char_archetype_file(path: Path, model: CharArchetypeResourceModel) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)

    with path.open("wb") as f:
        write_file_header(f, ResourceTypeId.CHAR_ARCHETYPE, model.VERSION)
        write_char_archetype_resource(f, model)
