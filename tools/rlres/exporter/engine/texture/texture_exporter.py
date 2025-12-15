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
from typing import Sequence

from PIL import Image, UnidentifiedImageError

from rlres.data.engine.resource.index_builder import ResourceEntry, ResourceIndex
from rlres.data.engine.resource.resource_type import ResourceTypeName
from rlres.data.engine.resource.resource_utils import make_res_filepath
from rlres.data.engine.texture.texture_type import TexFormat, TexResourceModel
from rlres.exporter.asset_exporter import AssetExporter
from rlres.exporter.engine.texture.texture_serialize import write_texture_file
from rlres.logger import LOGGER


class TexExporter(AssetExporter):
    type_names: Sequence[ResourceTypeName] = (ResourceTypeName.TEX,)
    depends_on: Sequence[ResourceTypeName] = ()
    discover_patterns: Sequence[str] = ("*.png", "*.jpg")

    def _build_one(
        self,
        entry: ResourceEntry,
        _: Path,
        out_dir: Path,
        __: ResourceIndex,
    ) -> None:
        cls = self.__class__.__name__
        src_path = entry.src_path

        try:
            img = Image.open(src_path)
        except FileNotFoundError:
            LOGGER.error("%s: texture file not found: %s", cls, src_path)
            return
        except UnidentifiedImageError:
            LOGGER.error("%s: unsupported or corrupt image: %s", cls, src_path)
            return
        except OSError as e:
            LOGGER.error("%s: failed to open image %s: %s", cls, src_path, e)
            return

        try:
            img = img.convert("RGBA")
        except Exception as e:
            LOGGER.error("%s: failed to convert image %s to RGBA: %s", cls, src_path, e)

            return

        try:
            w, h = img.size
            data = img.tobytes()

        except Exception as e:
            LOGGER.error(
                "%s: failed to extract image bytes from %s: %s", cls, src_path, e
            )

            return

        finally:
            img.close()

        model = TexResourceModel(
            rid=entry.rid,
            fmt=TexFormat.RGBA8,
            width=w,
            height=h,
            data=data,
        )

        out_path = make_res_filepath(out_dir, entry.rid)

        try:
            write_texture_file(out_path, model)
        except OSError as e:
            LOGGER.error(
                "%s: failed to write texture %s to %s: %s",
                cls,
                entry.nid,
                out_path,
                e,
            )
