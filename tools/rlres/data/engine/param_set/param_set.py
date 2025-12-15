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

from enum import IntEnum
from typing import Any, Callable, Iterable

from rlres.data.engine.anim.anim_id import anim_id_from_name
from rlres.data.engine.resource.index_builder import ResourceIndex
from rlres.data.engine.resource.resource_type import ResourceSubTypeName
from rlres.data.engine.resource.resource_utils import parse_nid
from rlres.logger import LOGGER
from rlres.utils.serialize_utils import f32_to_bits

_INTERNAL_NID_ENCODERS: dict[str, Callable[[str], int]] = {
    ResourceSubTypeName.ANIM.value: lambda name: int(anim_id_from_name(name)),
}


def _encode_nid(nid: str, index: ResourceIndex) -> int:
    try:
        type_name, name = parse_nid(nid)
    except ValueError as e:
        LOGGER.error("Failed to parse NID %r: %s", nid, e)
        raise

    internal_encoder = _INTERNAL_NID_ENCODERS.get(type_name)

    if internal_encoder is not None:
        return internal_encoder(name)

    return index.get_rid(type_name, name)


def encode_param_set_value(raw: Any, index: ResourceIndex) -> int:
    # Case: bools first (as a subclass of int).
    if isinstance(raw, bool):
        return 1 if raw else 0

    if isinstance(raw, IntEnum):
        return int(raw)

    # Case: numbers. Encode as f32 bit-pattern stored in u32/u64.
    if isinstance(raw, (int, float)):
        return f32_to_bits(float(raw))

    # Case: NID-style resource reference: "type.name".
    if isinstance(raw, str):
        return _encode_nid(raw, index)

    LOGGER.warn(
        "Unsupported param value of type %s: %r",
        type(raw).__name__,
        raw,
    )

    raise TypeError(f"Unsupported param value: {raw!r}")


def encode_param_set(raw_params: Iterable[Any], index: ResourceIndex) -> list[int]:
    encoded: list[int] = []

    for raw in raw_params:
        try:
            encoded.append(encode_param_set_value(raw, index))
        except Exception as e:
            LOGGER.error(
                "Failed to encode param value %r: %s",
                raw,
                e,
            )

            raise

    return encoded
