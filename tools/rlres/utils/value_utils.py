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


def clamp(x: float, lo: float, hi: float) -> float:
    return max(lo, min(x, hi))


def parse_name_index(s: str) -> tuple[str, int]:
    i = s.rfind("_")
    if i <= 0 or i == len(s) - 1:
        # Case: no underscore, or underscore at start/end: invalid.
        raise ValueError(f"Not a name_index pattern: {s!r}")

    num = 0

    for ch in s[i + 1 :]:
        o = ord(ch)

        if o < 48 or o > 57:  # Not '0'..'9'.
            raise ValueError(f"Non-digit in index part: {s!r}")

        num = num * 10 + (o - 48)

    prefix = s[:i]
    return prefix, num
