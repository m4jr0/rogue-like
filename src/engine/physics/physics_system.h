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

#ifndef ENGINE_PHYSICS_PHYSICS_SYSTEM_H_
#define ENGINE_PHYSICS_PHYSICS_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/core/handle.h"
#include "engine/core/type.h"
#include "engine/physics/physics.h"
#include "engine/physics/physics_body.h"
#include "engine/transform/transform.h"

#ifdef RL_DEBUG
#include "engine/core/color.h"
#endif  // RL_DEBUG

// #define RL_DEBUG_PHYSICS

namespace rl {
using PhysicsSystemDebugFlags = u8;

enum PhysicsSystemDebugFlagBits : PhysicsSystemDebugFlags {
  kPhysicsSystemDebugFlagBitsNone = 0x0,
  kPhysicsSystemDebugFlagBitsCollider = 0x1,
  kPhysicsSystemDebugFlagBitsAll = static_cast<PhysicsSystemDebugFlags>(-1),
};

class PhysicsSystem {
 public:
  static PhysicsSystem& instance();

  void init();
  void shutdown();

  void update(FramePacket& f);

  [[nodiscard]] PhysicsBodyHandle generate(PhysicsBodyDesc desc);
  void destroy(PhysicsBodyHandle h);

  void pos(PhysicsBodyHandle h, Position p);
  void steer(PhysicsBodyHandle h, Steering s);

  void impulse(PhysicsBodyHandle h, Velocity dV);
  void knockback(PhysicsBodyHandle h, Dir dir, f32 strength);
  void dash(PhysicsBodyHandle h, f32 speed, f32 duration);
  void dash(PhysicsBodyHandle h, Dir dir, f32 speed, f32 duration);

  const PhysicsBody* body(PhysicsBodyHandle h) const;

#ifdef RL_DEBUG
  void debugOn(PhysicsSystemDebugFlags flags) { dFlags_ |= flags; }
  void debugOff(PhysicsSystemDebugFlags flags) { dFlags_ &= ~flags; }
  void updateDebug() const;
#endif  // RL_DEBUG

 private:
#ifdef RL_DEBUG
  inline static Rgba kDebugColliderColor_{kRgbaBlue.r, kRgbaBlue.g, kRgbaBlue.b,
                                          128};

  PhysicsSystemDebugFlags dFlags_{kPhysicsSystemDebugFlagBitsNone};
#endif  // RL_DEBUG

  f64 lag_{.0};
  HandlePool<PhysicsBodyTag> hBodyPool_{};
  std::vector<PhysicsBody> bodies_;

  PhysicsSystem() = default;

  void tick(const FramePacket& f);
  void resolveBodiesVsBodies();

  PhysicsBody* body(PhysicsBodyHandle h);

#ifdef RL_DEBUG
  void drawDebugCollider(const PhysicsBody& b) const;
#endif  // RL_DEBUG

#ifdef RL_DEBUG_PHYSICS
  u32 tickCount_{0};
  f64 debugTime_{0.0};

  void debugTick();
  void debugUpdate(const FramePacket& f);

#define RL_PHYSICS_DEBUG_TICK() debugTick()
#define RL_PHYSICS_DEBUG_UPDATE(f) debugUpdate(f)
#else
#define RL_PHYSICS_DEBUG_TICK() ((void)0)
#define RL_PHYSICS_DEBUG_UPDATE(f) ((void)0)
#endif  // RL_DEBUG_PHYSICS
};
}  // namespace rl

#define RL_PHYSICSSYS (::rl::PhysicsSystem::instance())
#define RL_CPHYSICSSYS \
  (static_cast<const ::rl::PhysicsSystem&>(::rl::PhysicsSystem::instance()))

#ifdef RL_DEBUG
#define RL_PHYSICSDEBON(flags) (::rl::PhysicsSystem::instance().debugOn(flags))
#define RL_PHYSICSDEBOFF(flags) \
  (::rl::PhysicsSystem::instance().debugOff(flags))
#define RL_PHYSICSDEB() (::rl::PhysicsSystem::instance().updateDebug())
#else
#define RL_PHYSICSDEBON(cond, ...) ((void)0)
#define RL_PHYSICSDEBOFF(cond, ...) ((void)0)
#define RL_PHYSICSDEB(cond, ...) ((void)0)
#endif  // RL_DEBUG

#endif  // ENGINE_PHYSICS_PHYSICS_SYSTEM_H_
