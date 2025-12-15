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

#ifndef ENGINE_PHYSICS_ANIM_COLLIDER_LIBRARY_H_
#define ENGINE_PHYSICS_ANIM_COLLIDER_LIBRARY_H_

#include "engine/common.h"
#include "engine/physics/collider_resource.h"
#include "engine/resource/resource_slot.h"

namespace rl {
class AnimColliderLibrary {
 public:
  static AnimColliderLibrary& instance();

  void init();
  void shutdown();

  const AnimColliderProfileResource* load(AnimColliderProfileId id);
  void unload(AnimColliderProfileId id);
  const AnimColliderProfileResource* get(AnimColliderProfileId id) const;

 private:
  ResourceSlots<AnimColliderProfileTag, AnimColliderProfileResource> slots_{};

  AnimColliderLibrary() = default;
};
}  // namespace rl

#define RL_ANIMCOLLIB (::rl::AnimColliderLibrary::instance())
#define RL_CANIMCOLLIB                            \
  (static_cast<const ::rl::AnimColliderLibrary&>( \
      ::rl::AnimColliderLibrary::instance()))

#endif  // ENGINE_PHYSICS_ANIM_COLLIDER_LIBRARY_H_
