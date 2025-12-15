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

import shutil

from pathlib import Path

from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.logger import LOGGER

_SUPPORTED_IMAGE_EXTS: tuple[str, ...] = (".png", ".jpg", ".jpeg")


def find_texture(tex_dir: Path, tex: str) -> Path:
    for ext in _SUPPORTED_IMAGE_EXTS:
        candidate = tex_dir / f"{tex}{ext}"

        if candidate.exists():
            return candidate

    tried = ", ".join(f"{tex}{ext}" for ext in _SUPPORTED_IMAGE_EXTS)

    LOGGER.error(
        "Missing image for Aseprite sheet %r in %r (tried: %s)",
        tex,
        tex_dir,
        tried,
    )

    raise FileNotFoundError(
        f"Missing image for Aseprite sheet '{tex}' (tried: {tried})"
    )


def copy_texture(src_img: Path, out_root: Path, tex_name: str) -> Path:
    tex_dir = out_root / ResourceTypeName.TEX
    tex_dir.mkdir(parents=True, exist_ok=True)

    ext = src_img.suffix.lower()
    dst_path = tex_dir / f"{tex_name}{ext}"

    shutil.copy2(src_img, dst_path)

    LOGGER.debug(
        "Copied texture %r -> %r",
        src_img,
        dst_path,
    )

    return dst_path


def process_texture(sheet_path: Path, out_root: Path) -> tuple[str, Path]:
    tex_dir = sheet_path.parent
    tex = sheet_path.stem

    src_img = find_texture(tex_dir, tex)
    dst_path = copy_texture(src_img, out_root, tex)

    return tex, dst_path
