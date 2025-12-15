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

#ifndef ENGINE_PHYSICS_HITBOX_SYSTEM_H_
#define ENGINE_PHYSICS_HITBOX_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/core/handle.h"
#include "engine/physics/grid.h"
#include "engine/physics/hitbox.h"

#ifdef RL_DEBUG
#include "engine/core/color.h"
#endif  // RL_DEBUG

namespace rl {
using HitboxSystemDebugFlags = u8;

enum HitboxSystemDebugFlagBits : HitboxSystemDebugFlags {
  kHitboxSystemDebugFlagBitsNone = 0x0,
  kHitboxSystemDebugFlagBitsHitbox = 0x1,
  kHitboxSystemDebugFlagBitsHurtbox = 0x2,
  kHitboxSystemDebugFlagBitsAll = static_cast<HitboxSystemDebugFlags>(-1),
};

class HitboxSystem {
 public:
  static HitboxSystem& instance();

  void init();
  void shutdown();

  void tick(const FramePacket& f);

  [[nodiscard]] HitboxHandle generate(const HitboxDesc& desc);
  void destroy(HitboxHandle h);

  [[nodiscard]] HurtboxHandle onHurt(const HurtboxDesc& desc);
  void offHurt(HurtboxHandle h);

  Hitbox* hitbox(HitboxHandle h);
  const Hitbox* hitbox(HitboxHandle h) const;

  Hurtbox* hurtbox(HurtboxHandle h);
  const Hurtbox* hurtbox(HurtboxHandle h) const;

  std::span<const HitContact> contacts() const noexcept { return contacts_; }

#ifdef RL_DEBUG
  void debugOn(HitboxSystemDebugFlags flags) { dFlags_ |= flags; }
  void debugOff(HitboxSystemDebugFlags flags) { dFlags_ &= ~flags; }
  void updateDebug();
#endif  // RL_DEBUG

 private:
#ifdef RL_DEBUG
  inline static Rgba kDebugHitboxColor_{kRgbaRed.r, kRgbaRed.g, kRgbaRed.b,
                                        128};
  inline static Rgba kDebugHurtboxColor_{kRgbaGreen.r, kRgbaGreen.g,
                                         kRgbaGreen.b, 128};

  HitboxSystemDebugFlags dFlags_{kHitboxSystemDebugFlagBitsNone};
#endif  // RL_DEBUG
  HurtboxStamp seenStamp_{1};

  HandlePool<HitboxTag> hHitboxPool_{};
  std::vector<Hitbox> hitboxes_{};

  HandlePool<HurtboxTag> hHurtboxPool_{};
  std::vector<Hurtbox> hurtboxes_{};

  SpatialGrid grid_{};

  std::vector<HitContact> contacts_{};

  HitboxSystem() = default;

  void prepareSeenStamp();
  void rebuildWorldCollider(Hitbox& h);
  void rebuildWorldCollider(Hurtbox& h);
  void handleHit(Hitbox& hit, const Hurtbox* hurt, f64 time);
  void cleanupHitboxes(f64 time);

#ifdef RL_DEBUG
  void drawDebugHitbox(const Hitbox& h) const;
  void drawDebugHurtbox(const Hurtbox& h) const;
  void drawDebugCollider(const Collider& c, const Rgba& color) const;
#endif  // RL_DEBUG
};
}  // namespace rl

#define RL_HITBOXSYS (::rl::HitboxSystem::instance())
#define RL_CHITBOXSYS \
  (static_cast<const ::rl::HitboxSystem&>(::rl::HitboxSystem::instance()))

#ifdef RL_DEBUG
#define RL_HITBOXDEBON(flags) (::rl::HitboxSystem::instance().debugOn(flags))
#define RL_HITBOXDEBOFF(flags) (::rl::HitboxSystem::instance().debugOff(flags))
#define RL_HITBOXDEB() (::rl::HitboxSystem::instance().updateDebug())
#else
#define RL_HITBOXDEBON(cond, ...) ((void)0)
#define RL_HITBOXDEBOFF(cond, ...) ((void)0)
#define RL_HITBOXDEB(cond, ...) ((void)0)
#endif  // RL_DEBUG

#endif  // ENGINE_PHYSICS_HITBOX_SYSTEM_H_
