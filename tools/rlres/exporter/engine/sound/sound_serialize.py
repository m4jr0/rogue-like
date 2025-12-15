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

from rlres.data.engine.resource.resource_type import ResourceTypeId

from rlres.data.engine.sound.sound_type import (
    SoundBankResourceModel,
    SoundResourceModel,
)

from rlres.utils.serialize_utils import (
    write_file_header,
    write_u32,
    write_u64,
)


def write_sound_resource(f: BinaryIO, s: SoundResourceModel) -> None:
    write_u32(f, s.rid)
    write_u32(f, s.sample_rate)
    write_u32(f, s.channel_count)

    # Samples are 32-bit floats: 4 bytes per sample.
    sample_count = len(s.samples) // 4
    write_u64(f, sample_count)

    f.write(s.samples)


def write_sound_file(path: Path, model: SoundResourceModel) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)

    with path.open("wb") as f:
        write_file_header(f, ResourceTypeId.SOUND, SoundResourceModel.VERSION)
        write_sound_resource(f, model)


def write_sound_bank_resource(f: BinaryIO, b: SoundBankResourceModel) -> None:
    write_u32(f, b.rid)
    count = len(b.sounds)
    write_u64(f, count)

    for sid in b.sounds:
        write_u32(f, sid)


def write_sound_bank_file(path: Path, model: SoundBankResourceModel) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)

    with path.open("wb") as f:
        write_file_header(f, ResourceTypeId.SOUND_BANK, SoundBankResourceModel.VERSION)
        write_sound_bank_resource(f, model)
