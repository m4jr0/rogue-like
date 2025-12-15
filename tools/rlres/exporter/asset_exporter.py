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

from abc import ABC, abstractmethod
from pathlib import Path
from typing import ClassVar, Iterable, Sequence

from rlres.data.engine.resource.index_builder import (
    ResourceEntry,
    ResourceIndex,
    ResourceIndexBuilder,
)

from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.logger import LOGGER


class AssetExporter(ABC):
    type_names: ClassVar[Sequence[ResourceTypeName]]
    depends_on: ClassVar[Sequence[ResourceTypeName]] = ()
    discover_patterns: ClassVar[Sequence[str]] = ()
    exclude_suffixes: ClassVar[Sequence[str]] = ()

    def discover(self, src_root: Path, index_builder: ResourceIndexBuilder) -> None:
        cls_name = self.__class__.__name__

        for type_name in self.type_names:
            base_dir = src_root / type_name

            if not base_dir.is_dir():
                LOGGER.debug(
                    "%s: base directory %s does not exist for type %s; skipping",
                    cls_name,
                    base_dir,
                    type_name.value,
                )

                continue

            for path in self._iter_source_files(type_name, base_dir):
                if not self._should_include(type_name, path):
                    LOGGER.debug(
                        "%s: excluding %s for type %s based on suffix filter",
                        cls_name,
                        path,
                        type_name.value,
                    )

                    continue

                name = self._make_resource_name(type_name, path)

                try:
                    index_builder.add(type_name.value, name, path)
                except ValueError as e:
                    LOGGER.error(
                        "%s: duplicate or invalid resource %s.%s at %s: %s",
                        cls_name,
                        type_name.value,
                        name,
                        path,
                        e,
                    )

                    raise

    def run(self, src_root: Path, out_root: Path, index: ResourceIndex) -> None:
        cls_name = self.__class__.__name__

        for type_name in self.type_names:
            src_dir = src_root / type_name
            out_dir = out_root / type_name
            out_dir.mkdir(parents=True, exist_ok=True)

            for entry in index.by_type(type_name.value):
                try:
                    self._build_one(entry, src_dir, out_dir, index)
                except Exception as e:
                    LOGGER.error(
                        "%s: failed exporting resource %r of type %s from %s: %s",
                        cls_name,
                        entry.nid,
                        type_name.value,
                        entry.src_path,
                        e,
                    )

    def _patterns_for_type(
        self,
        _: ResourceTypeName,
    ) -> Sequence[str]:
        return self.discover_patterns

    def _iter_source_files(
        self,
        type_name: ResourceTypeName,
        base_dir: Path,
    ) -> Iterable[Path]:
        patterns = self._patterns_for_type(type_name)

        if not patterns:
            yield from sorted(p for p in base_dir.iterdir() if p.is_file())
            return

        for pattern in patterns:
            for path in sorted(base_dir.glob(pattern)):
                yield path

    def _make_resource_name(
        self,
        _: ResourceTypeName,
        path: Path,
    ) -> str:
        return path.stem

    def _should_include(
        self,
        _: ResourceTypeName,
        path: Path,
    ) -> bool:
        name = path.name

        for suf in self.exclude_suffixes:
            if name.endswith(suf):
                return False

        return True

    @abstractmethod
    def _build_one(
        self,
        entry: ResourceEntry,
        src_dir: Path,
        out_dir: Path,
        index: ResourceIndex,
    ) -> None: ...
