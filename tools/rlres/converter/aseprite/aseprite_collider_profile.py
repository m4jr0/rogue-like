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

from typing import Any, Mapping, Sequence

from rlres.data.engine.physics.collider import ColliderShapeName
from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.data.engine.resource.resource_utils import make_nid
from rlres.logger import LOGGER
from rlres.utils.safe_utils import safe_json_get


def _frame_center_and_half_extents(
    raw: Mapping[str, Any],
) -> tuple[float, float, float, float]:
    frame_rect = safe_json_get(raw, "frame", Mapping, {}, mandatory=False)
    sprite_src = safe_json_get(raw, "spriteSourceSize", Mapping, {}, mandatory=False)
    source_size = safe_json_get(raw, "sourceSize", Mapping, {}, mandatory=False)

    fw = float(safe_json_get(frame_rect, "w", (int, float), 0.0, mandatory=False))
    fh = float(safe_json_get(frame_rect, "h", (int, float), 0.0, mandatory=False))

    sx = float(safe_json_get(sprite_src, "x", (int, float), 0.0, mandatory=False))
    sy = float(safe_json_get(sprite_src, "y", (int, float), 0.0, mandatory=False))

    sw = float(safe_json_get(source_size, "w", (int, float), 0.0, mandatory=False))
    sh = float(safe_json_get(source_size, "h", (int, float), 0.0, mandatory=False))

    hw = fw * 0.5
    hh = fh * 0.5

    cx_px = sx + hw
    cy_px = sy + hh

    return cx_px, cy_px, hw, hh, sw, sh


def _build_tracks(
    anim_frame_indices: list[int],
    src_tracks: Mapping[int, Mapping[int, Mapping[str, Any]]],
    pivots: Mapping[int, tuple[float, float]],
) -> list[list[dict[str, Any]]]:
    result: list[list[dict[str, Any]]] = []

    for track_index in sorted(src_tracks.keys()):
        per_frame = src_tracks[track_index]
        samples: list[dict[str, Any]] = []

        for i in anim_frame_indices:
            raw = per_frame.get(i)

            if raw is None:
                samples.append({"collider": {"shape": ColliderShapeName.UNKNOWN}})
                continue

            cx_px, cy_px, hw, hh, sw, sh = _frame_center_and_half_extents(raw)
            pivot_px = pivots.get(i, (sw * 0.5, sh * 0.5))
            px, py = pivot_px.custom

            cx = cx_px - px
            cy = py - cy_px

            samples.append(
                {
                    "collider": {
                        "shape": ColliderShapeName.AABB,
                        "center": [cx, cy],
                        "half_extents": [hw, hh],
                    },
                }
            )

        result.append(samples)

    return result


def build_collider_profile_json(
    atlas_id: str,
    sheet: Mapping[str, Any],
    atlas_frames: Sequence[tuple[int, str, Mapping[str, Any]]],
    hurt_tracks: Mapping[int, Mapping[int, Mapping[str, Any]]],
    hit_tracks: Mapping[int, Mapping[int, Mapping[str, Any]]],
    pivots: Mapping[int, tuple[float, float]],
) -> dict[str, Any]:
    meta = safe_json_get(sheet, "meta", Mapping, {}, mandatory=False)
    tags = safe_json_get(meta, "frameTags", list, [], mandatory=False)

    per_anim: list[dict[str, Any]] = []

    if atlas_frames:
        min_frame_idx = min(fi for fi, _, _ in atlas_frames)
        max_frame_idx = max(fi for fi, _, _ in atlas_frames)
    else:
        min_frame_idx = 0
        max_frame_idx = -1

    for raw_tag in tags:
        if not isinstance(raw_tag, Mapping):
            LOGGER.warn(
                "Collider profile: ignoring non-mapping frameTag entry %r", raw_tag
            )

            continue

        name = safe_json_get(raw_tag, "name", str, "", mandatory=True)

        if not name:
            LOGGER.warn(
                "Collider profile: frameTag with empty/missing name; skipping: %r",
                raw_tag,
            )

            continue

        start = safe_json_get(raw_tag, "from", int, 0, mandatory=True)
        end = safe_json_get(raw_tag, "to", int, start, mandatory=True)

        if end < start:
            LOGGER.warn(
                "Collider profile: tag %r has end < start (from=%d, to=%d); swapping",
                raw_tag,
                start,
                end,
            )

            start, end = end, start

        if max_frame_idx < min_frame_idx:
            # Case: no frames at all.
            LOGGER.warn(
                "Collider profile: no atlas frames available when processing tag %r",
                raw_tag,
            )

            continue

        start = max(min_frame_idx, min(start, max_frame_idx))
        end = max(min_frame_idx, min(end, max_frame_idx))

        anim_frame_indices = list(range(start, end + 1))

        hurts_json = _build_tracks(
            anim_frame_indices,
            hurt_tracks,
            pivots,
        )

        hits_json = _build_tracks(
            anim_frame_indices,
            hit_tracks,
            pivots,
        )

        per_anim.append(
            {
                "name": name,
                "hurts": hurts_json,
                "hits": hits_json,
            }
        )

    collider_profile = {
        "anim_set": make_nid(ResourceTypeName.ATLAS, atlas_id),
        "per_anim": per_anim,
    }

    return collider_profile
