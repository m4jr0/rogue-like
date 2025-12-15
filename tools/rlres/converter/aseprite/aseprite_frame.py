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

from enum import StrEnum
from pathlib import Path
from typing import Any, Callable, Mapping, Sequence

from rlres.logger import LOGGER
from rlres.utils.safe_utils import safe_json_get
from rlres.utils.value_utils import parse_name_index


class AseLayerKind(StrEnum):
    UNKNOWN = "unknown"
    ATLAS = "atlas"
    PIVOT = "pivot"
    HITBOX = "hit"
    HURTBOX = "hurt"


from dataclasses import dataclass, field
from typing import Any, Mapping


@dataclass(slots=True)
class GroupedFrameData:
    atlas_indexed: list[tuple[int, str, Mapping[str, Any]]] = field(
        default_factory=list
    )

    atlas_pending: list[tuple[str, Mapping[str, Any]]] = field(default_factory=list)
    max_index: int = -1

    pivot: dict[str, Mapping[str, Any]] = field(default_factory=dict)
    hurt: dict[int, dict[int, Mapping[str, Any]]] = field(default_factory=dict)
    hit: dict[int, dict[int, Mapping[str, Any]]] = field(default_factory=dict)


def _handle_atlas(
    data: GroupedFrameData,
    filename: str,
    suffix: str | None,
    _: int | None,
    raw_frame: Mapping[str, Any],
) -> None:
    frame_index: int | None = None

    if suffix is not None:
        try:
            frame_index = int(suffix)
        except ValueError:
            LOGGER.warn(
                "Non-integer frame suffix %r in filename %r; treating as pending",
                suffix,
                filename,
            )

    if frame_index is not None:
        data.atlas_indexed.append((frame_index, filename, raw_frame))

        if frame_index > data.max_index:
            data.max_index = frame_index
    else:
        data.atlas_pending.append((filename, raw_frame))


def _handle_pivot(
    data: GroupedFrameData,
    filename: str,
    suffix: str | None,
    _: int | None,
    raw_frame: Mapping[str, Any],
) -> None:
    key = suffix or filename
    data.pivot[key] = raw_frame


def _handle_indexed_track(
    tracks: dict[int, dict[int, Mapping[str, Any]]],
    kind_name: str,
    filename: str,
    track_index: int | None,
    suffix: str | None,
    raw_frame: Mapping[str, Any],
) -> None:
    if track_index is None:
        LOGGER.warn(
            "%s frame %r does not follow '%s_<number>' pattern; ignoring",
            kind_name.capitalize(),
            filename,
            kind_name,
        )

        return

    if suffix is None:
        LOGGER.warn(
            "%s frame %r has no frame index suffix; ignoring",
            kind_name.capitalize(),
            filename,
        )

        return

    try:
        frame_index = int(suffix)
    except ValueError:
        LOGGER.warn(
            "%s frame %r has non-integer frame suffix %r; ignoring",
            kind_name.capitalize(),
            filename,
            suffix,
        )

        return

    track = tracks.setdefault(track_index, {})

    if frame_index in track:
        LOGGER.warn(
            "Duplicate %s frame for track %d, frame %d (filename=%r); overwriting",
            kind_name,
            track_index,
            frame_index,
            filename,
        )

    track[frame_index] = raw_frame


def _handle_hurt(
    data: GroupedFrameData,
    filename: str,
    suffix: str | None,
    track_index: int | None,
    raw_frame: Mapping[str, Any],
) -> None:
    _handle_indexed_track(
        data.hurt,
        AseLayerKind.HURTBOX,
        filename=filename,
        track_index=track_index,
        suffix=suffix,
        raw_frame=raw_frame,
    )


def _handle_hit(
    data: GroupedFrameData,
    filename: str,
    suffix: str | None,
    track_index: int | None,
    raw_frame: Mapping[str, Any],
) -> None:
    _handle_indexed_track(
        data.hit,
        AseLayerKind.HITBOX,
        filename=filename,
        track_index=track_index,
        suffix=suffix,
        raw_frame=raw_frame,
    )


FrameHandler = Callable[
    [GroupedFrameData],
    None,
]


FrameDispatch = Callable[
    [GroupedFrameData, str, str | None, int | None, Mapping[str, Any]],
    None,
]


_KIND_HANDLERS: dict[AseLayerKind, FrameDispatch] = {
    AseLayerKind.UNKNOWN: None,
    AseLayerKind.ATLAS: _handle_atlas,
    AseLayerKind.PIVOT: _handle_pivot,
    AseLayerKind.HURTBOX: _handle_hurt,
    AseLayerKind.HITBOX: _handle_hit,
}


def parse_ase_frame_name(
    filename: str,
) -> tuple[str, AseLayerKind, str | None, int | None]:
    stem = Path(filename).stem
    start = stem.rfind(" (")
    end = stem.rfind(")")

    if start == -1 or end == -1 or end < start:
        return stem, AseLayerKind.UNKNOWN, None, None

    base = stem[:start].rstrip()
    raw_kind = stem[start + 2 : end].strip()
    tail = stem[end + 1 :].strip()
    suffix = tail or None

    try:
        kind = AseLayerKind(raw_kind)
        return base, kind, suffix, None
    except ValueError:
        pass

    try:
        kind_prefix, track_index = parse_name_index(raw_kind)
    except ValueError:
        return base, AseLayerKind.UNKNOWN, suffix, None

    try:
        kind = AseLayerKind(kind_prefix)
    except ValueError:
        return base, AseLayerKind.UNKNOWN, suffix, None

    return base, kind, suffix, track_index


def build_frame_data(
    raw_frames: Sequence[tuple[str, Mapping[str, Any]]],
) -> GroupedFrameData:
    data = GroupedFrameData()

    for filename, raw_frame in raw_frames:
        if not isinstance(raw_frame, Mapping):
            LOGGER.warn(
                "Ignoring non-mapping frame data for %r: %r", filename, raw_frame
            )

            continue

        _, kind, suffix, track_index = parse_ase_frame_name(filename)
        handler = _KIND_HANDLERS.get(kind)

        if handler is None:
            continue

        handler(
            data,
            filename,
            suffix,
            track_index,
            raw_frame,
        )

    next_index = data.max_index + 1 if data.max_index >= 0 else 0

    for filename, raw_frame in data.atlas_pending:
        data.atlas_indexed.append((next_index, filename, raw_frame))
        next_index += 1

    data.atlas_indexed.sort(key=lambda t: t[0])
    return data


def normalize_ase_frames(
    sheet: Mapping[str, Any],
) -> list[tuple[str, Mapping[str, Any]]]:
    frames_field = safe_json_get(
        sheet,
        "frames",
        (Mapping, list),
        [],
        mandatory=True,
    )

    # Case: dict.
    if isinstance(frames_field, Mapping):
        result: list[tuple[str, Mapping[str, Any]]] = []

        for filename, frame in frames_field.items():
            if not isinstance(frame, Mapping):
                LOGGER.warn(
                    "Ignoring non-mapping frame entry for %r: %r",
                    filename,
                    frame,
                )

                continue

            result.append((str(filename), frame))
        return result

    # Case: list.
    result: list[tuple[str, Mapping[str, Any]]] = []

    for frame in frames_field:
        if not isinstance(frame, Mapping):
            LOGGER.warn("Ignoring non-mapping frame entry: %r", frame)
            continue

        filename = safe_json_get(frame, "filename", str, "", mandatory=True)

        if not filename:
            LOGGER.warn("Ignoring frame entry without valid filename: %r", frame)
            continue

        result.append((filename, frame))

    return result
