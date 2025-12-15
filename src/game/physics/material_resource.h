// Copyright 2025 m4jr0. All Rights Reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef GAME_PHYSICS_MATERIAL_H_
#define GAME_PHYSICS_MATERIAL_H_

#include "engine/common.h"
#include "engine/resource/resource_type.h"

namespace rl {
struct MaterialTag {};
using MaterialId = ResourceId<MaterialTag>;

struct MaterialResource {
  inline constexpr static ResourceVersion kVersion{1};

  MaterialId id{kInvalidResourceId};
  f32 moveScale{1.0f};
  f32 groundDamping{.0f};

  // External motion & collisions (applies to extVel).
  f32 friction{.1f};
  f32 restitution{.0f};
  f32 airDrag{.0f};
};
}  // namespace rl

#endif  // GAME_PHYSICS_MATERIAL_H_
