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

from pathlib import Path
from typing import Any, Callable, Mapping, TypeVar, Union, Tuple, cast

from rlres.logger import LOGGER

T = TypeVar("T")


def safe_json_get(
    obj: Any,
    key: str,
    expected_type: Union[type, Tuple[type, ...]],
    default_value: T,
    *,
    mandatory: bool = True,
    allow_null: bool = False,
) -> T:
    func_name = safe_json_get.__name__

    # Case: obj is not a mapping.
    if not isinstance(obj, Mapping):
        LOGGER.warn(
            "%s: called with non-mapping object %r for key %r; using default %r",
            func_name,
            obj,
            key,
            default_value,
        )
        return default_value

    # Case: missing key.
    if key not in obj:
        if mandatory:
            LOGGER.warn(
                "%s: mandatory key %r is missing in JSON object %r; using default %r",
                func_name,
                key,
                obj,
                default_value,
            )
        return default_value

    value = obj[key]

    # Case: explicit null.
    if value is None:
        if allow_null:
            return default_value

    # Case: type mismatch.
    if not isinstance(value, expected_type):
        LOGGER.warn(
            "%s: unexpected type for key %r: got %s with value %r, expected %r; "
            "using default %r",
            func_name,
            key,
            type(value).__name__,
            value,
            expected_type,
            default_value,
        )

        return default_value

    return cast(T, value)


def safe_load_json_object(sheet_path: Path) -> dict[str, Any]:
    func_name = safe_load_json_object.__name__

    try:
        text = sheet_path.read_text(encoding="utf-8")
    except OSError as exc:
        LOGGER.error(
            "%s: failed to read JSON %r: %s",
            func_name,
            sheet_path,
            exc,
        )

        return {}

    try:
        data = json.loads(text)
    except json.JSONDecodeError as exc:
        LOGGER.error(
            "%s: failed to parse %r as JSON: %s",
            func_name,
            sheet_path,
            exc,
        )

        return {}

    if not isinstance(data, dict):
        LOGGER.error(
            "%s: JSON %r root is not an object: %r",
            func_name,
            sheet_path,
            data,
        )

        return {}

    return data


def safe_json_get_flags(
    obj: Any,
    key: str,
    *,
    name_to_flag: Callable[[str], int],
    default: int = 0,
    mandatory: bool = False,
    allow_null: bool = False,
) -> int:
    func_name = safe_load_json_object.__name__

    raw = safe_json_get(
        obj,
        key,
        expected_type=(int, float, list),
        default_value=default,
        mandatory=mandatory,
        allow_null=allow_null,
    )

    if raw is None:
        return default

    if isinstance(raw, (int, float)):
        return int(raw)

    if isinstance(raw, list):
        flags = 0

        for item in raw:
            if not isinstance(item, str):
                LOGGER.warn(
                    "%s: flag list for %r contains non-string %r; skipping",
                    func_name,
                    key,
                    item,
                )

                continue

            try:
                flags |= int(name_to_flag(item))
            except Exception:
                LOGGER.warn(
                    "%s: unknown flag %r for key %r; skipping",
                    func_name,
                    item,
                    key,
                )

        return flags

    LOGGER.warn(
        "%s: value for key %r has invalid type %r; using default %r",
        func_name,
        key,
        type(raw).__name__,
        default,
    )

    return default
