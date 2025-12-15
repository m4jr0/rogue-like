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

#ifndef ENGINE_PHYSICS_ANIM_COLLIDER_SYNC_H_
#define ENGINE_PHYSICS_ANIM_COLLIDER_SYNC_H_

#include "engine/anim/anim_resource.h"
#include "engine/anim/anim_runtime.h"
#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/core/spatial_ref.h"
#include "engine/physics/collider.h"
#include "engine/physics/collider_resource.h"
#include "engine/physics/hitbox.h"
#include "engine/resource/resource_type.h"
#include "engine/transform/transform.h"

namespace rl {
struct AnimColliderRigTag {};
using AnimColliderRigHandle = Handle<AnimColliderRigTag>;

struct AnimColliderRigDesc {
  SpatialRef ref{};
  AnimatorHandle animator{kInvalidHandle};
  std::vector<CollisionFilter> hurtFilters{};
  std::vector<CollisionFilter> hitFilters{};
};

struct AnimColliderRig {
  SpatialRef ref{};
  AnimColliderRigHandle handle{kInvalidHandle};
  AnimatorHandle animator{kInvalidHandle};
  usize animIdx{kInvalidIndex};
  const AnimSetResource* animSet{nullptr};
  const AnimColliderProfileResource* profile{nullptr};

  std::vector<HurtboxHandle> hurtHandles{};
  std::vector<HitboxHandle> hitHandles{};
};
}  // namespace rl

#endif  // ENGINE_PHYSICS_ANIM_COLLIDER_SYNC_H_
