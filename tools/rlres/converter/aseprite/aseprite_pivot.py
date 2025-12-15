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

from rlres.converter.aseprite.aseprite_frame import parse_ase_frame_name
from rlres.data.engine.sprite.sprite_pivot import PivotDesc, PivotPreset
from rlres.logger import LOGGER
from rlres.utils.safe_utils import safe_json_get

_DEFAULT_PIVOT = PivotDesc(preset=PivotPreset.BOTTOM_CENTER)


def pivot_from_meta(meta: Mapping[str, Any] | None) -> PivotDesc:
    if meta is None:
        return _DEFAULT_PIVOT

    raw_pivot = safe_json_get(meta, "pivot", Mapping, {}, mandatory=False)

    if not raw_pivot:
        return _DEFAULT_PIVOT

    preset_str = safe_json_get(raw_pivot, "preset", str, "", mandatory=True)

    if not preset_str:
        return _DEFAULT_PIVOT

    try:
        preset = PivotPreset(preset_str)
    except ValueError:
        LOGGER.warn("Invalid pivot preset %r in meta; using default pivot", preset_str)
        return _DEFAULT_PIVOT

    custom: tuple[int, int] | None = None
    norm: tuple[float, float] | None = None

    cx = safe_json_get(raw_pivot, "x", int, None, mandatory=False)
    cy = safe_json_get(raw_pivot, "y", int, None, mandatory=False)

    if cx is not None and cy is not None:
        custom = (cx, cy)

    nx = safe_json_get(raw_pivot, "nx", float, None, mandatory=False)
    ny = safe_json_get(raw_pivot, "ny", float, None, mandatory=False)

    if nx is not None and ny is not None:
        norm = (nx, ny)

    return PivotDesc(
        preset=preset,
        custom=custom,
        norm=norm,
    )


def pivot_from_layer(
    filename: str,
    pivots: Mapping[str, Mapping[str, Any]],
) -> PivotDesc | None:
    _, _, suffix, _ = parse_ase_frame_name(filename)
    key = suffix or filename
    raw_pivot = pivots.get(key)

    if raw_pivot is None:
        return None

    sprite_size = safe_json_get(
        raw_pivot,
        "spriteSourceSize",
        Mapping,
        {},
        mandatory=False,
    )

    if not sprite_size:
        return None

    px = safe_json_get(sprite_size, "x", int, None, mandatory=True)
    py = safe_json_get(sprite_size, "y", int, None, mandatory=True)
    pw = safe_json_get(sprite_size, "w", int, None, mandatory=True)
    ph = safe_json_get(sprite_size, "h", int, None, mandatory=True)

    if px is None or py is None or pw is None or ph is None:
        return None

    cx = px + pw / 2.0
    cy = py + ph / 2.0

    return PivotDesc(
        preset=PivotPreset.CUSTOM_PIXELS,
        custom=(int(round(cx)), int(round(cy))),
        norm=None,
    )
