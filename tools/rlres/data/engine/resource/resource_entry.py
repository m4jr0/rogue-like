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

from rlres.data.engine.resource.resource_type import ResourceId, ResourceTypeId, res_id
from rlres.data.engine.resource.resource_utils import make_nid
from rlres.utils.hash import fnv1a32


@dataclass(slots=True)
class ResourceEntry:
    type_name: str
    name: str
    src_path: Path

    rid: ResourceId = field(init=False)
    type_id: ResourceTypeId = field(init=False)
    nid: str = field(init=False)

    def __post_init__(self) -> None:
        self.type_id = res_id(self.type_name)
        self.nid = make_nid(self.type_name, self.name)
        self.rid = fnv1a32(self.nid)

    def to_dict(self) -> dict[str, object]:
        return {
            "rid": self.rid,
            "nid": self.nid,
            "type": self.type_name,
            "type_id": int(self.type_id),
            "name": self.name,
            "src": str(self.src_path),
        }

    def to_json(self, indent: int = 2) -> str:
        return json.dumps(self.to_dict(), indent=indent)
