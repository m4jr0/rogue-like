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

from dataclasses import dataclass, field
from typing import ClassVar, Sequence

from rlres.data.engine.physics.collider import ColliderDescModel


@dataclass(slots=True)
class AnimColliderSampleModel:
    collider: ColliderDescModel


@dataclass(slots=True)
class AnimColliderTrackModel:
    samples: list[AnimColliderSampleModel] = field(default_factory=list)


@dataclass(slots=True)
class AnimColliderSetModel:
    hurts: list[AnimColliderTrackModel] = field(default_factory=list)
    hits: list[AnimColliderTrackModel] = field(default_factory=list)


@dataclass(slots=True)
class AnimColliderProfileResourceModel:
    VERSION: ClassVar[int] = 1

    rid: int
    per_anim: Sequence[AnimColliderSetModel]
