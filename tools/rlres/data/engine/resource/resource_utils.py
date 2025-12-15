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

from rlres.data.engine.resource.resource_type import (
    ResourceId,
    ResourceTypeId,
    ResourceTypeName,
    res_name,
    res_id,
)


def parse_nid(nid: str) -> tuple[str, str]:
    parts = nid.split(".", 1)

    if len(parts) != 2:
        raise ValueError(f"NID '{nid}' is not in 'type.name' form.")

    return parts[0].lower(), parts[1].lower()


def make_nid(rt: ResourceTypeName | str, name: str) -> str:
    if isinstance(rt, ResourceTypeName):
        type_part = rt.value
    else:
        type_part = rt

    return f"{type_part}.{name}"


def make_res_filename(rid: ResourceId) -> str:
    return f"{rid}.res"


def make_res_filepath(out_root: Path, rid: ResourceId) -> Path:
    return out_root / make_res_filename(rid)


def ensure_res_type_name(
    value: str | ResourceTypeName | ResourceTypeId,
) -> ResourceTypeName:
    if isinstance(value, ResourceTypeName):
        return value

    if isinstance(value, ResourceTypeId):
        return res_name(value)

    if isinstance(value, str):
        rt_id = res_id(value)
        return res_name(rt_id)

    LOGGER.warn(
        "Unsupported value type %s in ensure_res_type_name; using %s.%s",
        type(value).__name__,
        ResourceTypeName.__name__,
        ResourceTypeName.UNKNOWN.name,
    )

    return ResourceTypeName.UNKNOWN
