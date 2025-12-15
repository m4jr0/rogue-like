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

from rlres.logger import LOGGER


class AssetConverter(ABC):
    subdir: ClassVar[str] = ""
    discover_patterns: ClassVar[Sequence[str]] = ("*.json",)

    def run(self, src_root: Path, out_root: Path) -> None:
        base_dir = src_root / self.subdir

        if not base_dir.is_dir():
            LOGGER.info(
                "%s: base directory %r does not exist; skipping",
                self.__class__.__name__,
                base_dir,
            )

            return

        LOGGER.info(
            "%s: scanning %r",
            self.__class__.__name__,
            base_dir,
        )

        for path in self._iter_source_files(base_dir):
            try:
                LOGGER.debug(
                    "%s: converting %r",
                    self.__class__.__name__,
                    path,
                )

                self._convert_one(path, src_root, out_root)

            except Exception as e:
                LOGGER.error(
                    "%s: error while converting %r: %s",
                    self.__class__.__name__,
                    path,
                    e,
                )

    def _iter_source_files(self, base_dir: Path) -> Iterable[Path]:
        if not self.discover_patterns:
            yield from sorted(p for p in base_dir.iterdir() if p.is_file())
            return

        for pattern in self.discover_patterns:
            for path in sorted(base_dir.glob(pattern)):
                yield path

    @abstractmethod
    def _convert_one(self, src_path: Path, src_root: Path, out_root: Path) -> None: ...
