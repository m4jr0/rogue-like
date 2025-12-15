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

from rlres.data.engine.param_set.param_set import encode_param_set
from rlres.data.engine.resource.index_builder import ResourceEntry, ResourceIndex
from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.data.engine.resource.resource_utils import make_res_filepath

from rlres.data.game.ability.ability_program import ability_op

from rlres.data.game.ability.ability_type import (
    ability_flag,
    AbilityInstructionModel,
    AbilityProgramModel,
    AbilityResourceModel,
)

from rlres.exporter.asset_exporter import AssetExporter
from rlres.exporter.game.ability.ability_serialize import write_ability_file
from rlres.logger import LOGGER

from rlres.utils.safe_utils import (
    safe_json_get,
    safe_json_get_flags,
    safe_load_json_object,
)


class AbilityExporter(AssetExporter):
    type_names: Sequence[ResourceTypeName] = (ResourceTypeName.ABILITY,)
    depends_on: Sequence[ResourceTypeName] = ()
    discover_patterns: Sequence[str] = ("*.json",)

    def _build_one(
        self,
        entry: ResourceEntry,
        _: Path,
        out_dir: Path,
        index: ResourceIndex,
    ) -> None:
        cls_name = self.__class__.__name__
        src_path = entry.src_path

        data: Mapping[str, Any] = safe_load_json_object(src_path)

        if not data:
            LOGGER.error(
                "%s: empty or invalid JSON for ability %r from %s; skipping",
                cls_name,
                entry.name,
                src_path,
            )

            return

        cost_data = safe_json_get(
            data, "cost", expected_type=Mapping, default_value={}, mandatory=False
        )

        stamina_val = safe_json_get(
            cost_data,
            "stamina",
            expected_type=(int, float),
            default_value=0.0,
            mandatory=False,
        )

        mana_val = safe_json_get(
            cost_data,
            "mana",
            expected_type=(int, float),
            default_value=0.0,
            mandatory=False,
        )

        try:
            stamina = float(stamina_val)
        except Exception:
            LOGGER.warn(
                "%s: ability %r invalid stamina %r; using 0.0",
                cls_name,
                entry.name,
                stamina_val,
            )

            stamina = 0.0

        try:
            mana = float(mana_val)
        except Exception:
            LOGGER.warn(
                "%s: ability %r invalid mana %r; using 0.0",
                cls_name,
                entry.name,
                mana_val,
            )

            mana = 0.0

        flags = safe_json_get_flags(
            data,
            "flags",
            name_to_flag=ability_flag,
            default=0,
        )

        on_begin = self._build_program(data, "onBegin", index, entry)
        on_tick = self._build_program(data, "onTick", index, entry)
        on_end = self._build_program(data, "onEnd", index, entry)

        model = AbilityResourceModel(
            rid=entry.rid,
            stamina=stamina,
            mana=mana,
            flags=flags,
            on_begin=on_begin,
            on_tick=on_tick,
            on_end=on_end,
        )

        out_path = make_res_filepath(out_dir, entry.rid)
        write_ability_file(out_path, model)

    def _build_program(
        self,
        data: Mapping[str, Any],
        key: str,
        index: ResourceIndex,
        entry: ResourceEntry,
    ) -> AbilityProgramModel:
        cls_name = self.__class__.__name__

        inst_datas = safe_json_get(
            data,
            key,
            expected_type=list,
            default_value=[],
            mandatory=False,
        )

        code: list[AbilityInstructionModel] = []

        for data in inst_datas:
            if not isinstance(data, Mapping):
                LOGGER.warn(
                    "%s: ability %r program %s has non-mapping instruction; skipping. Raw=%r",
                    cls_name,
                    entry.name,
                    key,
                    data,
                )

                continue

            op_name = safe_json_get(
                data,
                "op",
                expected_type=str,
                default_value="",
            )

            if not op_name:
                LOGGER.warn(
                    "%s: ability %r program %s missing 'op'; skipping. Raw=%r",
                    cls_name,
                    entry.name,
                    key,
                    data,
                )

                continue

            op_enum = ability_op(op_name)

            if op_enum is None:
                LOGGER.warn(
                    "%s: ability %r program %s unknown op %r; skipping.",
                    cls_name,
                    entry.name,
                    key,
                    op_name,
                )

                continue

            raw_params = safe_json_get(
                data,
                "params",
                expected_type=list,
                default_value=[],
                mandatory=False,
            )

            params = encode_param_set(raw_params, index)

            code.append(
                AbilityInstructionModel(
                    op=int(op_enum),
                    params=params,
                )
            )

        return AbilityProgramModel(code=code)
