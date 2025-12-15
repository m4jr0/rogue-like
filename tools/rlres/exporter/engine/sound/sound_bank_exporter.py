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
from typing import Any, Mapping, Sequence

from rlres.data.engine.resource.index_builder import ResourceEntry, ResourceIndex
from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.data.engine.resource.resource_utils import make_res_filepath
from rlres.data.engine.sound.sound_type import SoundBankResourceModel
from rlres.exporter.asset_exporter import AssetExporter
from rlres.exporter.engine.sound.sound_serialize import write_sound_bank_file
from rlres.logger import LOGGER
from rlres.utils.safe_utils import safe_json_get, safe_load_json_object


class SoundBankExporter(AssetExporter):
    type_names: Sequence[ResourceTypeName] = (ResourceTypeName.SOUND_BANK,)
    depends_on: Sequence[ResourceTypeName] = (ResourceTypeName.SOUND,)
    discover_patterns: Sequence[str] = ("*.json",)

    def _build_one(
        self,
        entry: ResourceEntry,
        _: Path,
        out_dir: Path,
        index: ResourceIndex,
    ) -> None:
        src_path = entry.src_path
        data: Mapping[str, Any] = safe_load_json_object(src_path)

        if not data:
            LOGGER.error(
                "%s: empty or invalid JSON for sound bank %r from %s; skipping",
                self.__class__.__name__,
                entry.name,
                src_path,
            )

            return

        sound_names = safe_json_get(
            data,
            "sounds",
            expected_type=list,
            default_value=[],
            mandatory=False,
        )

        sound_ids_by_name = {
            e.nid: e.rid for e in index.by_type(ResourceTypeName.SOUND)
        }

        sound_rids: list[int] = []

        for raw_name in sound_names:
            if not isinstance(raw_name, str) or not raw_name:
                LOGGER.warn(
                    "%s: SoundBank %r has non-string or empty sound entry %r; skipping",
                    self.__class__.__name__,
                    entry.name,
                    raw_name,
                )

                continue

            rid = sound_ids_by_name.get(raw_name)

            if rid is None:
                LOGGER.error(
                    "%s: SoundBank %r references unknown sound %r; skipping",
                    self.__class__.__name__,
                    entry.name,
                    raw_name,
                )

                continue

            sound_rids.append(rid)

        model = SoundBankResourceModel(
            rid=entry.rid,
            sounds=sound_rids,
        )

        out_path = make_res_filepath(out_dir, entry.rid)
        write_sound_bank_file(out_path, model)
