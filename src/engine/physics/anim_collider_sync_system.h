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

#ifndef ENGINE_PHYSICS_ANIM_COLLIDER_SYNC_SYSTEM_H_
#define ENGINE_PHYSICS_ANIM_COLLIDER_SYNC_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/core/handle.h"
#include "engine/physics/anim_collider_sync.h"

namespace rl {
class AnimColliderSyncSystem {
 public:
  static AnimColliderSyncSystem& instance();

  void init();
  void shutdown();

  void tick(const FramePacket&);

  [[nodiscard]] AnimColliderRigHandle generate(const AnimColliderRigDesc& desc);
  void destroy(AnimColliderRigHandle h);

 private:
  HandlePool<AnimColliderRigTag> hRigPool_{};
  std::vector<AnimColliderRig> rigs_{};

  AnimColliderSyncSystem() = default;

  void update(AnimFrame frame, const AnimColliderSet& set, AnimColliderRig& r);
  void deactivate(AnimColliderRig& r);

  AnimColliderRig* rig(AnimColliderRigHandle h);
  const AnimColliderRig* rig(AnimColliderRigHandle h) const;
};
}  // namespace rl

#define RL_ANIMCOLSYNCSYS (::rl::AnimColliderSyncSystem::instance())
#define RL_CHITBOXSYNCSYS                            \
  (static_cast<const ::rl::AnimColliderSyncSystem&>( \
      ::rl::AnimColliderSyncSystem::instance()))

#endif  // ENGINE_PHYSICS_ANIM_COLLIDER_SYNC_SYSTEM_H_
