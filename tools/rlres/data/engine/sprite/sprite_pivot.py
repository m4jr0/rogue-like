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

from dataclasses import dataclass
from enum import StrEnum

from rlres.converter.aseprite.aseprite_type import OffsetPair
from rlres.data.engine.sprite.sprite_type import SpriteDesc
from rlres.logger import LOGGER
from rlres.utils.value_utils import clamp


class PivotPreset(StrEnum):
    TOP_LEFT = "top_left"
    TOP_CENTER = "top_center"
    TOP_RIGHT = "top_right"
    CENTER_LEFT = "center_left"
    CENTER = "center"
    CENTER_RIGHT = "center_right"
    BOTTOM_LEFT = "bottom_left"
    BOTTOM_CENTER = "bottom_center"
    BOTTOM_RIGHT = "bottom_right"
    CUSTOM_PIXELS = "custom_pixels"
    CUSTOM_NORMALIZED = "custom_normalized"


@dataclass(slots=True)
class PivotDesc:
    preset: str
    custom: tuple[int, int] | None = None
    norm: tuple[float, float] | None = None


def resolve_pivot(sprite_desc: SpriteDesc, pivot_desc: PivotDesc) -> tuple[int, int]:
    ssz_w, ssz_h = sprite_desc.source_size
    px = 0
    py = 0

    if pivot_desc.preset == PivotPreset.TOP_LEFT:
        px = 0
        py = 0

    elif pivot_desc.preset == PivotPreset.TOP_CENTER:
        px = ssz_w // 2
        py = 0

    elif pivot_desc.preset == PivotPreset.TOP_RIGHT:
        px = ssz_w
        py = 0

    elif pivot_desc.preset == PivotPreset.CENTER_LEFT:
        px = 0
        py = ssz_h // 2

    elif pivot_desc.preset == PivotPreset.CENTER:
        px = ssz_w // 2
        py = ssz_h // 2

    elif pivot_desc.preset == PivotPreset.CENTER_RIGHT:
        px = ssz_w
        py = ssz_h // 2

    elif pivot_desc.preset == PivotPreset.BOTTOM_LEFT:
        px = 0
        py = ssz_h

    elif pivot_desc.preset == PivotPreset.BOTTOM_CENTER:
        px = ssz_w // 2
        py = ssz_h

    elif pivot_desc.preset == PivotPreset.BOTTOM_RIGHT:
        px = ssz_w
        py = ssz_h

    elif pivot_desc.preset == PivotPreset.CUSTOM_PIXELS:
        if pivot_desc.custom is not None:
            px, py = pivot_desc.custom
        else:
            LOGGER.warn(
                "%s preset %s used without custom pixels; defaulting to (0, 0)",
                PivotPreset.__name__,
                PivotPreset.CUSTOM_PIXELS.value,
            )

    elif pivot_desc.preset == PivotPreset.CUSTOM_NORMALIZED:
        if pivot_desc.norm is not None:
            nx, ny = pivot_desc.norm
            nx = clamp(nx, 0.0, 1.0)
            ny = clamp(ny, 0.0, 1.0)
            px = int(round(nx * ssz_w))
            py = int(round(ny * ssz_h))
        else:
            LOGGER.warn(
                "%s preset %s used without normalized coords; defaulting to (0, 0)",
                PivotPreset.__name__,
                PivotPreset.CUSTOM_NORMALIZED.value,
            )

    else:
        LOGGER.warn(
            "Unknown %s value %r in resolve_pivot; defaulting to (0, 0)",
            PivotPreset.__name__,
            pivot_desc.preset,
        )

    return px, py


def resolve_offset(sprite_desc: SpriteDesc, pivot_desc: PivotDesc) -> tuple[int, int]:
    sr = sprite_desc.source_rect
    px, py = resolve_pivot(sprite_desc, pivot_desc)
    return sr.x - px, sr.y - py


def resolve_offsets_norm(
    sprite_desc: SpriteDesc,
    pivot_desc: PivotDesc,
) -> OffsetPair:
    off_x, off_y = resolve_offset(sprite_desc, pivot_desc)

    rw = max(1, sprite_desc.rect.w)
    rh = max(1, sprite_desc.rect.h)

    off_norm_x = clamp(-off_x / rw, 0.0, 1.0)
    off_norm_y = clamp(1.0 + (off_y / rh), 0.0, 1.0)

    off_flipped_x = 1.0 - off_norm_x
    off_flipped_y = 1.0 - off_norm_y

    return (off_norm_x, off_norm_y), (off_flipped_x, off_flipped_y)
