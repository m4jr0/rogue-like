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

from rlres.data.engine.anim.anim_id import AnimName, anim_id_from_name
from rlres.data.engine.physics.physics_body import parse_physics_body_desc
from rlres.data.engine.resource.index_builder import ResourceEntry, ResourceIndex
from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.data.engine.resource.resource_utils import make_res_filepath

from rlres.data.game.character.character_archetype_type import (
    CharArchetypeResourceModel,
    char_arch_flag,
)

from rlres.data.game.character.character_type import (
    CharClassName,
    CharFactionName,
    CharTypeName,
    char_class,
    char_faction,
    char_type,
)

from rlres.exporter.asset_exporter import AssetExporter

from rlres.exporter.game.character.character_archetype_serialize import (
    write_char_archetype_file,
)

from rlres.logger import LOGGER

from rlres.utils.safe_utils import (
    safe_json_get,
    safe_json_get_flags,
    safe_load_json_object,
)


class CharArchetypeExporter(AssetExporter):
    type_names: Sequence[ResourceTypeName] = (ResourceTypeName.CHAR_ARCHETYPE,)

    depends_on: Sequence[ResourceTypeName] = (
        ResourceTypeName.ABILITY,
        ResourceTypeName.ANIM_SET,
        ResourceTypeName.SOUND_BANK,
    )

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
                "%s: empty or invalid JSON for char archetype %r from %s; skipping",
                cls_name,
                entry.name,
                src_path,
            )

            return

        type_str = safe_json_get(
            data, "type", expected_type=str, default_value=CharTypeName.UNKNOWN.value
        )

        type_enum = char_type(type_str)

        faction_str = safe_json_get(
            data,
            "faction",
            expected_type=str,
            default_value=CharFactionName.NEUTRAL.value,
            mandatory=False,
        )

        faction_enum = char_faction(faction_str)

        flags = safe_json_get_flags(
            data,
            "flags",
            name_to_flag=char_arch_flag,
            default=0,
        )

        class_name = safe_json_get(
            data,
            "class",
            expected_type=str,
            default_value=CharClassName.NONE.value,
            mandatory=False,
        )

        class_enum = char_class(class_name)
        type_id = int(class_enum)

        raw_ability_nids = safe_json_get(
            data,
            "abilities",
            expected_type=list,
            default_value=[],
            mandatory=False,
        )

        ability_rids: list[int] = []

        for nid in raw_ability_nids:
            if not isinstance(nid, str):
                LOGGER.warn(
                    "%s: char archetype %r ability nid is not a string; skipping. Raw=%r",
                    cls_name,
                    entry.name,
                    nid,
                )

                continue

            rid = index.rid_from_nid(nid)
            ability_rids.append(rid)

        phys_data = safe_json_get(
            data,
            "physics",
            expected_type=Mapping,
            default_value={},
            mandatory=False,
        )

        physics = parse_physics_body_desc(phys_data)

        anim_set_nid = safe_json_get(
            data,
            "anim_set",
            expected_type=str,
            default_value="",
            mandatory=False,
        )

        anim_set = index.rid_from_nid(anim_set_nid)

        start_anim_name = safe_json_get(
            data,
            "start_anim",
            expected_type=str,
            default_value=AnimName.IDLE.value,
            mandatory=False,
            allow_null=True,
        )

        start_anim = int(anim_id_from_name(start_anim_name))

        sound_bank_nid = safe_json_get(
            data,
            "sound_bank",
            expected_type=str,
            default_value="",
            mandatory=False,
            allow_null=True,
        )

        sound_bank = index.rid_from_nid(sound_bank_nid)

        model = CharArchetypeResourceModel(
            rid=entry.rid,
            type=type_enum,
            faction=faction_enum,
            flags=flags,
            type_id=type_id,
            abilities=ability_rids,
            physics=physics,
            anim_set=anim_set,
            start_anim=start_anim,
            sound_bank=sound_bank,
        )

        out_path = make_res_filepath(out_dir, entry.rid)
        write_char_archetype_file(out_path, model)
