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
from typing import Any, Mapping, Sequence

from rlres.converter.aseprite.aseprite_pivot import pivot_from_layer

from rlres.converter.aseprite.aseprite_type import (
    AsepriteFrame,
    AsepriteFrameEntry,
    AsepriteRect,
    AsepriteSize,
    Ms,
    OffsetPair,
)

from rlres.data.engine.sprite.sprite_pivot import PivotDesc, resolve_offsets_norm
from rlres.data.engine.sprite.sprite_type import SpriteDesc, SpriteRect
from rlres.logger import LOGGER
from rlres.utils.safe_utils import safe_json_get


def build_events(
    meta_anim: Mapping[str, Any],
    frame_count: int,
) -> list[dict[str, Any]]:
    events: list[dict[str, Any]] = []
    raw_events = safe_json_get(meta_anim, "events", list, [], mandatory=False)

    for raw_event in raw_events:
        if not isinstance(raw_event, Mapping):
            LOGGER.warn("Ignoring non-mapping event entry %r", raw_event)
            continue

        frame = safe_json_get(raw_event, "frame", int, 0, mandatory=False)

        if frame < 0 or frame >= frame_count:
            LOGGER.warn(
                "Event frame %d out of range [0, %d); skipping event %r",
                frame,
                frame_count,
                raw_event,
            )

            continue

        name = safe_json_get(raw_event, "name", str, "", mandatory=True)

        if not name:
            LOGGER.warn("Event at frame %d has empty or missing name; skipping", frame)
            continue

        event_entry: dict[str, Any] = {
            "name": name,
            "frame": frame,
        }

        if "params" in raw_event:
            event_entry["params"] = raw_event["params"]

        events.append(event_entry)

    return events


def build_anim_frames(
    tag: Mapping[str, Any],
    frame_entries: Sequence[AsepriteFrameEntry],
    per_frame_offsets: Sequence[OffsetPair],
) -> list[dict[str, Any]]:
    start = safe_json_get(tag, "from", int, 0, mandatory=True)
    end = safe_json_get(tag, "to", int, len(frame_entries) - 1, mandatory=True)

    if end < start:
        LOGGER.warn(
            "Animation tag %r has end < start (from=%d, to=%d); swapping",
            tag,
            start,
            end,
        )

        start, end = end, start

    max_index = len(frame_entries) - 1

    if max_index < 0:
        LOGGER.warn(
            "No frame entries available when building anim frames for tag %r", tag
        )
        return []

    if start > max_index or end < 0:
        LOGGER.warn(
            "Tag range (from=%d, to=%d) is outside [0, %d]; nothing to build for %r",
            start,
            end,
            max_index,
            tag,
        )

        return []

    start = max(0, min(start, max_index))
    end = max(0, min(end, max_index))

    frames_list: list[dict[str, Any]] = []

    for i in range(start, end + 1):
        if i < 0 or i >= len(frame_entries):
            LOGGER.warn(
                "Frame index %d out of bounds when building anim frames; skipping",
                i,
            )

            continue

        entry = frame_entries[i]
        frame = entry.data
        offset_norm, offset_flipped_norm = per_frame_offsets[i]
        duration_sec = frame.duration_ms / 1000.0

        frames_list.append(
            {
                "rect": [
                    frame.frame.x,
                    frame.frame.y,
                    frame.frame.w,
                    frame.frame.h,
                ],
                "source_size": [
                    frame.source_size.w,
                    frame.source_size.h,
                ],
                "offset": [offset_norm[0], offset_norm[1]],
                "offset_flipped": [
                    offset_flipped_norm[0],
                    offset_flipped_norm[1],
                ],
                "duration": duration_sec,
            }
        )

    return frames_list


def build_frame_names(
    data: Mapping[str, Any],
    frames: Sequence[tuple[int, str, Mapping[str, Any]]],
) -> list[str]:
    count = len(frames)
    names: list[str | None] = [None] * count

    index_to_pos: dict[int, int] = {
        frame_index: pos for pos, (frame_index, _, _) in enumerate(frames)
    }

    meta = safe_json_get(data, "meta", Mapping, {}, mandatory=False)
    tags = safe_json_get(meta, "frameTags", list, [], mandatory=False)

    # Pass 1: tag-based names.
    for raw_tag in tags:
        if not isinstance(raw_tag, Mapping):
            LOGGER.warn("Ignoring non-mapping frame tag entry %r", raw_tag)
            continue

        tag_name = safe_json_get(raw_tag, "name", str, "", mandatory=True)

        if not tag_name:
            LOGGER.warn("Ignoring frame tag with empty or missing name: %r", raw_tag)
            continue

        start = safe_json_get(raw_tag, "from", int, 0, mandatory=True)
        end = safe_json_get(raw_tag, "to", int, start, mandatory=True)

        if end < start:
            LOGGER.warn(
                "frame tag %r has end < start (from=%d, to=%d); swapping",
                raw_tag,
                start,
                end,
            )

            start, end = end, start

        local_idx = 0

        for i in range(start, end + 1):
            pos = index_to_pos.get(i)

            if pos is None:
                # Case: tag refers to a frame index we don't have in frames.
                continue

            if names[pos] is None:
                names[pos] = f"{tag_name}_{local_idx:03d}"

            local_idx += 1

    # Pass 2: fallback to filename for any unnamed frame.
    for pos, (_, filename, _) in enumerate(frames):
        if names[pos] is None:
            base = Path(filename).stem.replace(" ", "_")
            names[pos] = base

    # No None left at this point.
    return [str(n) for n in names]


def build_per_frame_pivots(
    atlas_frames: Sequence[tuple[int, str, Mapping[str, Any]]],
    pivots: Mapping[str, Mapping[str, Any]],
    default_pivot: PivotDesc,
) -> dict[int, PivotDesc]:
    """Return {frame_index -> PivotDesc} for all atlas frames."""
    per_frame_pivot: dict[int, PivotDesc] = {}

    for frame_index, filename, _ in atlas_frames:
        layer_pivot = pivot_from_layer(filename, pivots)
        pivot = layer_pivot or default_pivot
        per_frame_pivot[frame_index] = pivot

    return per_frame_pivot


def build_entries_and_offsets(
    atlas_frames: Sequence[tuple[int, str, Mapping[str, Any]]],
    names: Sequence[str],
    pivots: Mapping[int, PivotDesc],
) -> tuple[
    list[AsepriteFrameEntry],
    list[OffsetPair],
]:
    entries: list[AsepriteFrameEntry] = []
    offsets: list[OffsetPair] = []

    for (frame_index, filename, raw_frame), sprite_name in zip(atlas_frames, names):
        frame_rect = safe_json_get(raw_frame, "frame", Mapping, {}, mandatory=True)
        sprite_size = safe_json_get(
            raw_frame, "spriteSourceSize", Mapping, {}, mandatory=True
        )

        source_size = safe_json_get(
            raw_frame, "sourceSize", Mapping, {}, mandatory=True
        )

        duration_ms = safe_json_get(raw_frame, "duration", Ms, 100, mandatory=False)

        fx = safe_json_get(frame_rect, "x", int, 0, mandatory=True)
        fy = safe_json_get(frame_rect, "y", int, 0, mandatory=True)
        fw = safe_json_get(frame_rect, "w", int, 0, mandatory=True)
        fh = safe_json_get(frame_rect, "h", int, 0, mandatory=True)

        sx = safe_json_get(sprite_size, "x", int, 0, mandatory=True)
        sy = safe_json_get(sprite_size, "y", int, 0, mandatory=True)
        sw = safe_json_get(sprite_size, "w", int, 0, mandatory=True)
        sh = safe_json_get(sprite_size, "h", int, 0, mandatory=True)

        ssw = safe_json_get(source_size, "w", int, 0, mandatory=True)
        ssh = safe_json_get(source_size, "h", int, 0, mandatory=True)

        frame = AsepriteFrame(
            frame=AsepriteRect(
                x=fx,
                y=fy,
                w=fw,
                h=fh,
            ),
            sprite_source_size=AsepriteRect(
                x=sx,
                y=sy,
                w=sw,
                h=sh,
            ),
            source_size=AsepriteSize(
                w=ssw,
                h=ssh,
            ),
            duration_ms=duration_ms,
        )

        entries.append(AsepriteFrameEntry(name=sprite_name, data=frame))

        sprite_desc = SpriteDesc(
            rect=SpriteRect(
                x=frame.frame.x,
                y=frame.frame.y,
                w=frame.frame.w,
                h=frame.frame.h,
            ),
            source_rect=SpriteRect(
                x=frame.sprite_source_size.x,
                y=frame.sprite_source_size.y,
                w=frame.sprite_source_size.w,
                h=frame.sprite_source_size.h,
            ),
            source_size=(frame.source_size.w, frame.source_size.h),
        )

        pivot = pivots.get(frame_index, {0, 0})
        offset_norm, offset_flipped_norm = resolve_offsets_norm(sprite_desc, pivot)
        offsets.append((offset_norm, offset_flipped_norm))

    return entries, offsets


def build_atlas_sprites(
    entries: Sequence[AsepriteFrameEntry],
    offsets: Sequence[OffsetPair],
) -> list[dict[str, Any]]:
    sprites: list[dict[str, Any]] = []

    for entry, (offset_norm, offset_flipped_norm) in zip(entries, offsets):
        frame = entry.data

        sprites.append(
            {
                "name": entry.name,
                "rect": [
                    frame.frame.x,
                    frame.frame.y,
                    frame.frame.w,
                    frame.frame.h,
                ],
                "source_size": [
                    frame.source_size.w,
                    frame.source_size.h,
                ],
                "offset": [offset_norm[0], offset_norm[1]],
                "offset_flipped": [
                    offset_flipped_norm[0],
                    offset_flipped_norm[1],
                ],
            }
        )

    return sprites
