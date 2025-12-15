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

from typing import Any, Mapping

from rlres.data.engine.resource.index_builder import ResourceIndex

from rlres.data.engine.resource.resource_type import (
    INVALID_RESOURCE_ID,
    ResourceId,
    ResourceTypeName,
)

from rlres.logger import LOGGER
from rlres.utils.safe_utils import safe_json_get


def rid_from_key(
    index: ResourceIndex,
    data: Mapping[str, Any],
    type_name: ResourceTypeName,
    json_key: str,
    silent: bool = False,
) -> ResourceId:
    name = safe_json_get(
        data,
        json_key,
        expected_type=str,
        default_value="",
        mandatory=False,
    )

    if not name:
        if not silent:
            LOGGER.warn(
                "%s: RID requested from missing or empty JSON key %r for type %s; "
                "using %s",
                rid_from_key.__name__,
                json_key,
                type_name.value,
                "INVALID_RESOURCE_ID",
            )

        return INVALID_RESOURCE_ID

    return index.get_rid(type_name.value, name)


def json_vec2(
    parent: Mapping[str, Any],
    key: str,
    default: tuple[float, float],
) -> tuple[float, float]:
    default_list = [float(default[0]), float(default[1])]

    raw = safe_json_get(
        parent,
        key,
        expected_type=list,
        default_value=default_list,
        mandatory=False,
    )

    if not isinstance(raw, list) or len(raw) != 2:
        LOGGER.warn(
            "%s: '%s' is not a 2-element list; using %r",
            json_vec2.__name__,
            key,
            default,
        )

        return default

    try:
        x, y = float(raw[0]), float(raw[1])
    except (TypeError, ValueError) as exc:
        LOGGER.warn(
            "%s: '%s' has invalid values %r; using %r (%s)",
            json_vec2.__name__,
            key,
            raw,
            default,
            exc,
        )

        return default

    return (x, y)
