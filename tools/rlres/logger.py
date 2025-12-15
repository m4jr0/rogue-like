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

import logging
import warnings

from typing import Any


class PipelineLogger:
    def __init__(self, name: str = "rl_pipeline") -> None:
        self._logger: logging.Logger = logging.getLogger(name)

    def debug(self, msg: str, *args: Any) -> None:
        try:
            self._logger.debug(msg, *args)
        except Exception:
            pass

    def info(self, msg: str, *args: Any) -> None:
        try:
            self._logger.info(msg, *args)
        except Exception:
            pass

    def warn(self, msg: str, *args: Any, stacklevel: int = 3) -> None:
        try:
            text = PipelineLogger._safe_format(msg, args)
            warnings.warn(text, RuntimeWarning, stacklevel=stacklevel)
            self._logger.warning(text)
        except Exception:
            pass

    def error(self, msg: str, *args: Any, stacklevel: int = 3) -> None:
        try:
            text = PipelineLogger._safe_format(msg, args)
            warnings.warn(text, RuntimeWarning, stacklevel=stacklevel)
            self._logger.error(text)
        except Exception:
            pass

    @staticmethod
    def _safe_format(msg: str, args: tuple[Any, ...]) -> str:
        if not args:
            return msg

        try:
            return msg % args
        except Exception:
            return f"{msg} | args={args!r}"


LOGGER = PipelineLogger()
