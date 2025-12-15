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

// Precompiled. ////////////////////////////////////////////////////////////////
#include "precompiled.h"
////////////////////////////////////////////////////////////////////////////////

// Header. /////////////////////////////////////////////////////////////////////
#include "physics_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/anim/anim_system.h"
#include "engine/core/phase_bus.h"
#include "engine/core/vector.h"
#include "engine/event/event_system.h"
#include "engine/physics/anim_collider_sync_system.h"
#include "engine/physics/hitbox_system.h"
#include "engine/physics/physics_utils.h"
#include "engine/sound/sound_system.h"
#include "engine/transform/transform_system.h"

#ifdef RL_DEBUG
#include "engine/render/render_gizmo.h"
#endif  // RL_DEBUG

namespace rl {
PhysicsSystem& PhysicsSystem::instance() {
  static PhysicsSystem inst;
  return inst;
}

void PhysicsSystem::init() {
  RL_LOG_DEBUG("PhysicsSystem::init");
  static constexpr usize kDefaultBodyCap = 256;
  hBodyPool_.clear();
  hBodyPool_.reserve(kDefaultBodyCap);
  bodies_.reserve(kDefaultBodyCap);
}

void PhysicsSystem::shutdown() {
  RL_LOG_DEBUG("PhysicsSystem::shutdown");
  hBodyPool_.clear();
  bodies_.clear();
}

void PhysicsSystem::update(FramePacket& f) {
  lag_ += f.delta;

  constexpr u32 kMaxStepCount = 5;
  u32 stepCount = 0;

  while (lag_ >= f.step && stepCount < kMaxStepCount) {
    tick(f);
    RL_ANIMSYS.tick(f);
    RL_ANIMCOLSYNCSYS.tick(f);
    RL_HITBOXSYS.tick(f);
    RL_EVENTSYS.tick();
    RL_SOUNDSYS.tick(f);
    lag_ -= f.step;
    ++stepCount;
  }

  f.lag = lag_;
  RL_PHYSICS_DEBUG_UPDATE(f);
}

PhysicsBodyHandle PhysicsSystem::generate(PhysicsBodyDesc desc) {
  auto h = hBodyPool_.generate();
  ensureCapacity(bodies_, h.index);
  auto& b = bodies_[h.index];
  b.handle = h;
  b.trans = desc.trans;
  b.vel = desc.initVel;

  b.maxVel = desc.maxVel;
  b.dec = desc.dec;
  b.acc = desc.acc;

  b.dynamic = desc.dynamic;
  b.collider = desc.collider;
  return h;
}

void PhysicsSystem::destroy(PhysicsBodyHandle h) {
  auto* b = body(h);
  if (!b) return;
  *b = {};
  hBodyPool_.destroy(h);
}

void PhysicsSystem::pos(PhysicsBodyHandle h, Position p) {
  auto* b = body(h);
  if (!b) return;
  RL_TRANSSYS.translation(b->trans, p);
}

void PhysicsSystem::steer(PhysicsBodyHandle h, Steering s) {
  auto* b = body(h);
  if (!b) return;
  b->steer = s.clampMag(1.0f);
}

void PhysicsSystem::impulse(PhysicsBodyHandle h, Velocity dV) {
  auto* b = body(h);
  if (!b) return;
  b->extVel.x += dV.x;
  b->extVel.y += dV.y;
}

void PhysicsSystem::knockback(PhysicsBodyHandle h, Dir dir, f32 strength) {
  auto* b = body(h);
  if (!b) return;
  auto len2 = dir.magSqrd();

  if (len2 > 1e-12f) {
    auto inv = 1.0f / std::sqrt(len2);
    dir.x *= inv;
    dir.y *= inv;
  }

  impulse(h, {dir.x * strength, dir.y * strength});
}

void PhysicsSystem::dash(PhysicsBodyHandle h, f32 speed, f32 duration) {
  auto* b = body(h);
  if (!b) return;

  auto dir = b->dir;
  auto len2 = dir.magSqrd();

  if (len2 > 1e-12f) {
    auto inv = 1.0f / std::sqrt(len2);
    dir.x *= inv;
    dir.y *= inv;
  }

  b->extVel = {dir.x * speed, dir.y * speed};
  auto dec = (duration > .0f) ? (speed / duration) : speed * 10.0f;
  b->extDec = {dec, dec};
}

void PhysicsSystem::dash(PhysicsBodyHandle h, Dir dir, f32 speed,
                         f32 duration) {
  auto* b = body(h);
  if (!b) return;

  auto len2 = dir.magSqrd();

  if (len2 > 1e-12f) {
    auto inv = 1.0f / std::sqrt(len2);
    dir.x *= inv;
    dir.y *= inv;
  }

  b->extVel = {dir.x * speed, dir.y * speed};
  auto dec = (duration > .0f) ? (speed / duration) : speed * 10.0f;
  b->extDec = {dec, dec};
}

const PhysicsBody* PhysicsSystem::body(PhysicsBodyHandle h) const {
  RL_ASSERT(h && hBodyPool_.alive(h),
            "PhysicsSystem::body: Invalid physics body handle provided!");
  if (!h || !hBodyPool_.alive(h)) return nullptr;
  return &bodies_[h.index];
}

PhysicsBody* PhysicsSystem::body(PhysicsBodyHandle h) {
  RL_ASSERT(h && hBodyPool_.alive(h),
            "PhysicsSystem::body: Invalid physics body handle provided!");
  if (!h || !hBodyPool_.alive(h)) return nullptr;
  return &bodies_[h.index];
}

#ifdef RL_DEBUG
void PhysicsSystem::updateDebug() const {
  if (dFlags_ & kPhysicsSystemDebugFlagBitsCollider) {
    for (auto& b : bodies_) {
      if (!hBodyPool_.alive(b.handle)) continue;
      drawDebugCollider(b);
    }
  }
}
#endif  // RL_DEBUG

void PhysicsSystem::tick(const FramePacket& f) {
  auto dt = static_cast<f32>(f.step);

  for (auto& b : bodies_) {
    applyAcceleration(b, dt);
    applyVelocity(b, dt);
    applyDirection(b);
  }

  RL_TRANSSYS.tick(const_cast<FramePacket&>(f));

  for (auto& b : bodies_) {
    applyTransform(b);
  }

  resolveBodiesVsBodies();
  RL_PHASEBUS.invoke(TickPhase::FixedUpdate, f);
  RL_PHYSICS_DEBUG_TICK();
}

void PhysicsSystem::resolveBodiesVsBodies() {
  auto bodyCount = bodies_.size();

  for (usize i = 0; i < bodyCount; ++i) {
    auto& a = bodies_[i];
    if (!a.handle) continue;

    for (usize j = i + 1; j < bodyCount; ++j) {
      auto& b = bodies_[j];
      if (!b.handle) continue;

      const auto& ca = a.wCollider;
      const auto& cb = b.wCollider;

      // Broad phase: quick bounding-volume test to skip non-overlapping pairs.
      if (!overlap(ca, cb)) continue;

      // Narrow phase: compute precise collision info (minimum translation
      // vector).
      Dir mtv{};
      if (!rl::mtv(ca, cb, mtv)) continue;

      // Collision response: separate bodies using the MTV.
      applyMtv(a, b, mtv);
    }
  }
}

#ifdef RL_DEBUG
void PhysicsSystem::drawDebugCollider(const PhysicsBody& b) const {
  const auto& c = b.wCollider;

  switch (c.shape) {
    using enum ColliderShape;
    case Aabb:
      submitGizmoAabb(c.bounds.aabb, kDebugColliderColor_);
      break;
    case Circle:
      submitGizmoCircle(c.bounds.circle, kDebugColliderColor_);
      break;
    default:
      return;
  }
}
#endif  // RL_DEBUG

#ifdef RL_DEBUG_PHYSICS
void PhysicsSystem::debugTick() { ++tickCount_; }

void PhysicsSystem::debugUpdate(const FramePacket& f) {
  debugTime_ += f.delta;

  if (debugTime_ >= 1.0) {
    auto hz = tickCount_ / debugTime_;
    RL_LOG_DEBUG("Logic Tick: ", hz, " Hz.");
    tickCount_ = 0;
    debugTime_ = .0;
  }
}
#endif  // RL_DEBUG_PHYSICS
}  // namespace rl