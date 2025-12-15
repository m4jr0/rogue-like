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

import argparse

from pathlib import Path
from typing import Iterable

from rlres.converter.aseprite.aseprite_converter import AsepriteConverter
from rlres.converter.asset_converter import AssetConverter
from rlres.data.engine.resource.index_builder import ResourceIndex, ResourceIndexBuilder
from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.exporter.asset_exporter import AssetExporter
from rlres.exporter.engine.anim.anim_exporter import AnimExporter

from rlres.exporter.engine.physics.collider_profile_exporter import (
    AnimColliderProfileExporter,
)

from rlres.exporter.engine.sound.sound_bank_exporter import SoundBankExporter
from rlres.exporter.engine.sound.sound_exporter import SoundExporter
from rlres.exporter.engine.sprite.atlas_exporter import AtlasExporter
from rlres.exporter.engine.texture.texture_exporter import TexExporter
from rlres.exporter.game.ability.ability_exporter import AbilityExporter

from rlres.exporter.game.character.character_archetype_exporter import (
    CharArchetypeExporter,
)

from rlres.exporter.game.material.material_exporter import MaterialExporter
from rlres.logger import LOGGER


class AssetPipeline:
    def __init__(self, src_root: Path, out_root: Path) -> None:
        self.src_root = src_root
        self.out_root = out_root
        self._converters: list[AssetConverter] = []
        self._exporters: list[AssetExporter] = []

    def register_converter(self, converter: AssetConverter) -> None:
        self._converters.append(converter)

    def register_converters(self, converters: Iterable[AssetConverter]) -> None:
        self._converters.extend(converters)

    def register_exporter(self, exporter: AssetExporter) -> None:
        self._exporters.append(exporter)

    def register_exporters(self, exporters: Iterable[AssetExporter]) -> None:
        self._exporters.extend(exporters)

    def _sort_exporters(self) -> None:
        ordered: list[AssetExporter] = []
        remaining = list(self._exporters)
        ready_types: set[ResourceTypeName] = set()
        cls_name = self.__class__.__name__

        while remaining:
            progress = False

            for exporter in list(remaining):
                if all(dep in ready_types for dep in exporter.depends_on):
                    ordered.append(exporter)
                    remaining.remove(exporter)
                    ready_types.update(exporter.type_names)
                    progress = True
                    LOGGER.debug(
                        "%s: scheduled exporter %s (types=%r)",
                        cls_name,
                        exporter.__class__.__name__,
                        [t.value for t in exporter.type_names],
                    )

            if not progress:
                cycle_exporters = ", ".join(e.__class__.__name__ for e in remaining)
                raise RuntimeError(
                    f"{cls_name}: cyclic or unsatisfiable exporter dependencies "
                    f"among: {cycle_exporters}"
                )

        self._exporters = ordered

    def _run_converters(self) -> None:
        cls_name = self.__class__.__name__

        for converter in self._converters:
            conv_name = converter.__class__.__name__
            LOGGER.info("%s: running converter %s", cls_name, conv_name)
            converter.run(self.src_root, self.src_root)

    def _discover(self) -> ResourceIndex:
        cls_name = self.__class__.__name__
        builder = ResourceIndexBuilder()

        for exporter in self._exporters:
            LOGGER.info(
                "%s: discovering resources for exporter %s (types=%r)",
                cls_name,
                exporter.__class__.__name__,
                [t.value for t in exporter.type_names],
            )

            exporter.discover(self.src_root, builder)

        index = builder.to_index()

        LOGGER.info(
            "%s: discovery complete; %d resources registered",
            cls_name,
            len(index.entries),
        )

        return index

    def _export(self, index: ResourceIndex) -> ResourceIndex:
        cls_name = self.__class__.__name__

        for exporter in self._exporters:
            exp_name = exporter.__class__.__name__

            LOGGER.info(
                "%s: exporting resources with %s (types=%r)",
                cls_name,
                exp_name,
                [t.value for t in exporter.type_names],
            )

            exporter.run(self.src_root, self.out_root, index)

        json_index_path = self.out_root / "resources_index.json"
        bin_index_path = self.out_root / "resources.idx"

        index.write_index_json(json_index_path)
        index.write_index_file(bin_index_path)

        LOGGER.info(
            "%s: exported %d resources to %s (index JSON: %s, binary index: %s)",
            cls_name,
            len(index.entries),
            self.out_root,
            json_index_path,
            bin_index_path,
        )

        return index

    def run(self) -> ResourceIndex:
        cls_name = self.__class__.__name__

        LOGGER.info(
            "%s: starting asset pipeline (src=%s, out=%s)",
            cls_name,
            self.src_root,
            self.out_root,
        )

        self.out_root.mkdir(parents=True, exist_ok=True)

        self._run_converters()
        self._sort_exporters()
        index = self._discover()
        return self._export(index)


def main() -> None:
    parser = argparse.ArgumentParser(description="Run the RL resource export pipeline.")

    parser.add_argument(
        "--src",
        type=Path,
        default=Path("assets"),
        help="Source asset root",
    )

    parser.add_argument(
        "--out",
        type=Path,
        default=Path("resources"),
        help="Output resource root",
    )

    args = parser.parse_args()

    pipeline = AssetPipeline(args.src, args.out)

    pipeline.register_converters(
        [
            AsepriteConverter(),
        ]
    )

    pipeline.register_exporters(
        [
            AbilityExporter(),
            AnimExporter(),
            AtlasExporter(),
            CharArchetypeExporter(),
            AnimColliderProfileExporter(),
            MaterialExporter(),
            SoundBankExporter(),
            SoundExporter(),
            TexExporter(),
        ]
    )

    pipeline.run()


if __name__ == "__main__":
    main()
