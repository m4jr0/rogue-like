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

import miniaudio as ma

from pathlib import Path
from typing import Sequence


from rlres.data.engine.resource.index_builder import ResourceEntry, ResourceIndex
from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.data.engine.resource.resource_utils import make_res_filepath
from rlres.data.engine.sound.sound_type import SoundResourceModel
from rlres.exporter.asset_exporter import AssetExporter
from rlres.exporter.engine.sound.sound_serialize import write_sound_file
from rlres.logger import LOGGER


class SoundExporter(AssetExporter):
    type_names: Sequence[ResourceTypeName] = (ResourceTypeName.SOUND,)
    depends_on: Sequence[ResourceTypeName] = ()
    discover_patterns: Sequence[str] = ("*.wav", "*.mp3", "*.ogg", "*.flac")

    def _build_one(
        self,
        entry: ResourceEntry,
        _: Path,
        out_dir: Path,
        __: ResourceIndex,
    ) -> None:
        src_path = entry.src_path

        try:
            decoded = ma.decode_file(
                str(src_path),
                output_format=ma.SampleFormat.FLOAT32,
            )

        except Exception as e:
            LOGGER.error(
                "%s: failed to decode sound file %s: %s",
                self.__class__.__name__,
                src_path,
                e,
            )

            return

        sample_rate = int(decoded.sample_rate)
        channel_count = int(decoded.nchannels)

        if sample_rate <= 0 or channel_count <= 0:
            LOGGER.error(
                "%s: invalid decoded sound format from %s "
                "(sample_rate=%d, channels=%d); skipping",
                self.__class__.__name__,
                src_path,
                sample_rate,
                channel_count,
            )

            return

        samples_bytes = decoded.samples.tobytes()

        model = SoundResourceModel(
            rid=entry.rid,
            sample_rate=sample_rate,
            channel_count=channel_count,
            samples=samples_bytes,
        )

        out_path = make_res_filepath(out_dir, entry.rid)
        write_sound_file(out_path, model)
