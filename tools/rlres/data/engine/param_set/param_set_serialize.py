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

from typing import BinaryIO, Sequence

from rlres.logger import LOGGER
from rlres.utils.serialize_utils import write_u64


def write_param_set(
    f: BinaryIO,
    params: Sequence[int],
    max_count: int,
) -> None:
    if max_count < 0:
        raise ValueError(f"max_count must be non-negative, got {max_count}")

    count = len(params)

    if count > max_count:
        LOGGER.warn(
            "ParamSet received %d params but max_count is %d; truncating extras",
            count,
            max_count,
        )

        params = params[:max_count]

    for p in params:
        write_u64(f, int(p))

    for _ in range(max_count - len(params)):
        write_u64(f, 0)
