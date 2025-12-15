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

import json

from dataclasses import dataclass, field
from pathlib import Path
from typing import ClassVar

from rlres.data.engine.resource.resource_entry import ResourceEntry
from rlres.logger import LOGGER
from rlres.utils.serialize_utils import write_file_header, write_u32, write_usize

from rlres.data.engine.resource.resource_type import (
    INVALID_RESOURCE_ID,
    ResourceId,
    ResourceTypeId,
)

from rlres.data.engine.resource.resource_utils import make_nid, parse_nid


@dataclass(slots=True)
class ResourceIndex:
    VERSION: ClassVar[int] = 1

    entries: list[ResourceEntry]
    by_type_name: dict[str, dict[str, ResourceEntry]]

    def by_type(self, type_name: str) -> list[ResourceEntry]:
        return list(self.by_type_name.get(type_name, {}).values())

    def get(self, type_name: str, name: str) -> ResourceEntry | None:
        return self.by_type_name.get(type_name, {}).get(name)

    def get_rid(self, type_name: str, name: str) -> ResourceId:
        entry = self.get(type_name, name)

        if entry is None:
            LOGGER.warn(
                "Invalid or unknown resource ID requested: %s; returning INVALID_RESOURCE_ID",
                make_nid(type_name, name),
            )

            return INVALID_RESOURCE_ID

        return entry.rid

    def rid_from_nid(self, nid: str | None) -> int:
        if not nid:
            return INVALID_RESOURCE_ID

        try:
            t, n = parse_nid(nid)
        except ValueError as e:
            LOGGER.warn(
                "Invalid NID string %r; returning INVALID_RESOURCE_ID (%s)",
                nid,
                e,
            )

            return INVALID_RESOURCE_ID

        return self.get_rid(t, n)

    def write_index_json(self, out_path: Path, indent: int = 2) -> None:
        data = {"resources": [e.to_dict() for e in self.entries]}
        out_path.write_text(json.dumps(data, indent=indent), encoding="utf-8")

        LOGGER.info(
            "Wrote resource index JSON with %d entries to %s",
            len(self.entries),
            out_path,
        )

    def write_index_file(self, out_path: Path) -> None:
        out_path.parent.mkdir(parents=True, exist_ok=True)

        with out_path.open("wb") as f:
            write_file_header(f, ResourceTypeId.RESOURCE_INDEX, self.VERSION)
            write_usize(f, len(self.entries))

            for e in self.entries:
                nid_bytes = e.nid.encode("utf-8")
                write_u32(f, e.rid)
                write_u32(f, int(e.type_id))
                write_u32(f, len(nid_bytes))
                f.write(nid_bytes)

        LOGGER.info(
            "Wrote resource index binary with %d entries to %s",
            len(self.entries),
            out_path,
        )


@dataclass(slots=True)
class ResourceIndexBuilder:
    _entries: list[ResourceEntry] = field(default_factory=list)
    _by_type_name: dict[str, dict[str, ResourceEntry]] = field(default_factory=dict)

    def add(self, type_name: str, name: str, src_path: Path) -> ResourceEntry:
        bucket = self._by_type_name.setdefault(type_name, {})

        if name in bucket:
            LOGGER.error(
                "Duplicate resource %s.%s (existing src=%r, new src=%r)",
                type_name,
                name,
                bucket[name].src_path if name in bucket else None,
                src_path,
            )

            raise ValueError(f"Duplicate resource: {type_name}.{name}")

        entry = ResourceEntry(
            type_name=type_name,
            name=name,
            src_path=src_path,
        )

        self._entries.append(entry)
        bucket[name] = entry
        return entry

    def to_index(self) -> ResourceIndex:
        return ResourceIndex(
            entries=list(self._entries),
            by_type_name={t: dict(m) for t, m in self._by_type_name.items()},
        )
