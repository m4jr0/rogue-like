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
#include "hitbox_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/log.h"
#include "engine/core/param_traversal.h"
#include "engine/core/vector.h"
#include "engine/physics/physics_utils.h"

#ifdef RL_DEBUG
#include "engine/render/render_gizmo.h"
#endif  // RL_DEBUG

namespace rl {
HitboxSystem& HitboxSystem::instance() {
  static HitboxSystem inst;
  return inst;
}

void HitboxSystem::init() {
  RL_LOG_DEBUG("HitboxSystem::init");
  constexpr auto kHitboxCapacity = 256;
  hHitboxPool_.clear();
  hHitboxPool_.reserve(kHitboxCapacity);
  hitboxes_.reserve(kHitboxCapacity);

  constexpr auto kHurtboxCapacity = 64;
  hHurtboxPool_.clear();
  hHurtboxPool_.reserve(kHurtboxCapacity);
  hurtboxes_.reserve(kHitboxCapacity);

  constexpr auto kContactCapacity = 256;
  contacts_.clear();
  contacts_.reserve(kContactCapacity);

  // TODO(m4jr0): Be smarter.
  grid_.build({-1000, -1000}, {1000, 1000});
}

void HitboxSystem::shutdown() {
  RL_LOG_DEBUG("HitboxSystem::shutdown");
  hHitboxPool_.clear();
  hitboxes_.clear();

  hHurtboxPool_.clear();
  hurtboxes_.clear();

  contacts_.clear();
  grid_.clear();
}

void HitboxSystem::tick(const FramePacket& f) {
  contacts_.clear();
  grid_.clear();

  for (auto& hurt : hurtboxes_) {
    if (!hurt.active) continue;
    rebuildWorldCollider(hurt);
    if (hurt.flat()) continue;
    grid_.add(hurt);
  }

  for (auto& hit : hitboxes_) {
    if (!hit.active()) continue;
    rebuildWorldCollider(hit);
    if (hit.collider.flat()) continue;
    prepareSeenStamp();
    const auto& collider = aabbOf(hit.wCollider);

    usize x0, y0, x1, y1;
    grid_.coveredCells(collider, x0, y0, x1, y1);

    for (usize cy = y0; cy <= y1 && !hit.consumed(); ++cy) {
      for (usize cx = x0; cx <= x1 && !hit.consumed(); ++cx) {
        const auto& cell = grid_.at(cx, cy);

        for (const auto& handle : cell.hurtboxes) {
          // Broad phase.
          auto* hurt = hurtbox(handle);

          // Deduplicate.
          if (hurt->seenStamp == seenStamp_) continue;
          hurt->seenStamp = seenStamp_;

          if (!hurt->active) continue;
          if (!shouldCollide(hit.filter, hurt->filter)) continue;
          if (hit.ref.sameTrans(hurt->ref)) continue;

          // Narrow phase.
          if (!overlap(hit.wCollider, hurt->wCollider)) continue;
          auto alreadyHit = false;  // TODO.
          if (alreadyHit) continue;

          // Event phase.
          handleHit(hit, hurt, f.time);

          if (!noHitCount(hit.maxHitCount) &&
              ++hit.hitCount == hit.maxHitCount) {
            hit.consume();
          }
        }
      }
    }

    // if (!hit.done && (hit.filter.mask & kCollisionFlagBitsWorld)) {
    //   if (overlapsAnySolidTile(box)) {
    //     hit.consume();
    //   }
    // }
  }

  cleanupHitboxes(f.time);
}

HitboxHandle HitboxSystem::generate(const HitboxDesc& desc) {
  auto h = hHitboxPool_.generate();
  ensureCapacity(hitboxes_, h.index);

  auto& hit = hitboxes_[h.index] = {
      .handle = h,
      .filter = desc.filter,
      .maxHitCount = desc.maxHitCount,
      .collider = desc.collider,
      .endTime = desc.endTime,
      .ref = desc.ref,
  };

  if (desc.active) hit.activate();
  return h;
}

void HitboxSystem::destroy(HitboxHandle h) {
  auto* hit = hitbox(h);
  *hit = {};
  hHitboxPool_.destroy(h);
}

HurtboxHandle HitboxSystem::onHurt(const HurtboxDesc& desc) {
  auto h = hHurtboxPool_.generate();
  ensureCapacity(hurtboxes_, h.index);

  hurtboxes_[h.index] = {
      .active = desc.active,
      .handle = h,
      .filter = desc.filter,
      .collider = desc.collider,
      .ref = desc.ref,
  };

  return h;
}

void HitboxSystem::offHurt(HurtboxHandle h) {
  auto* hurt = hurtbox(h);
  *hurt = {};
  hHurtboxPool_.destroy(h);
}

Hitbox* HitboxSystem::hitbox(HitboxHandle h) {
  RL_ASSERT(h && hHitboxPool_.alive(h),
            "HitboxSystem::hitbox: Invalid hitbox handle provided!");
  if (!h || !hHitboxPool_.alive(h)) return nullptr;
  return &hitboxes_[h.index];
}

const Hitbox* HitboxSystem::hitbox(HitboxHandle h) const {
  RL_ASSERT(h && hHitboxPool_.alive(h),
            "HitboxSystem::hitbox: Invalid hitbox handle provided!");
  if (!h || !hHitboxPool_.alive(h)) return nullptr;
  return &hitboxes_[h.index];
}

Hurtbox* HitboxSystem::hurtbox(HurtboxHandle h) {
  RL_ASSERT(h && hHurtboxPool_.alive(h),
            "HitboxSystem::hurtbox: Invalid hurtbox handle provided!");
  if (!h || !hHurtboxPool_.alive(h)) return nullptr;
  return &hurtboxes_[h.index];
}

const Hurtbox* HitboxSystem::hurtbox(HurtboxHandle h) const {
  RL_ASSERT(h && hHurtboxPool_.alive(h),
            "HitboxSystem::hurtbox: Invalid hurtbox handle provided!");
  if (!h || !hHurtboxPool_.alive(h)) return nullptr;
  return &hurtboxes_[h.index];
}

#ifdef RL_DEBUG
void HitboxSystem::updateDebug() {
  if (dFlags_ & kHitboxSystemDebugFlagBitsHitbox) {
    for (auto& hit : hitboxes_) {
      if (!hHitboxPool_.alive(hit.handle) || !hit.active()) continue;
      drawDebugHitbox(hit);
    }

    for (auto& hurt : hurtboxes_) {
      if (!hHurtboxPool_.alive(hurt.handle) || !hurt.active) continue;
      drawDebugHurtbox(hurt);
    }
  }
}
#endif  // RL_DEBUG

void HitboxSystem::prepareSeenStamp() {
  if (++seenStamp_ == 0) {
    for (auto& hurt : hurtboxes_) {
      hurt.seenStamp = 0;
    }

    seenStamp_ = 1;
  }
}

void HitboxSystem::rebuildWorldCollider(Hitbox& h) {
  updateWorldCollider(h.collider, h.ref, h.wCollider);
}

void HitboxSystem::rebuildWorldCollider(Hurtbox& h) {
  updateWorldCollider(h.collider, h.ref, h.wCollider);
}

void HitboxSystem::handleHit(Hitbox& hit, const Hurtbox* hurt, f64 time) {
  auto p = contactPoint(hit.wCollider, hurt->wCollider);

  contacts_.push_back(HitContact{
      .hit = hit.handle,
      .hurt = hurt->handle,
      .hitRef = hit.ref,
      .hurtRef = hurt->ref,
      .point = p,
      .hitFilter = hit.filter,
      .hurtFilter = hurt->filter,
      .time = time,
  });
}

void HitboxSystem::cleanupHitboxes(f64 time) {
  for (auto& hit : hitboxes_) {
    auto handle = hit.handle;
    if (!hHitboxPool_.alive(handle)) continue;
    if (!hit.done(time)) continue;
    hit = {};
    hHitboxPool_.destroy(handle);
  }
}

#ifdef RL_DEBUG
void HitboxSystem::drawDebugHitbox(const Hitbox& h) const {
  drawDebugCollider(h.wCollider, kDebugHitboxColor_);
}

void HitboxSystem::drawDebugHurtbox(const Hurtbox& h) const {
  drawDebugCollider(h.wCollider, kDebugHurtboxColor_);
}

void HitboxSystem::drawDebugCollider(const Collider& c,
                                     const Rgba& color) const {
  switch (c.shape) {
    using enum ColliderShape;
    case Aabb:
      submitGizmoAabb(c.bounds.aabb, color);
      break;
    case Circle:
      submitGizmoCircle(c.bounds.circle, color);
      break;
    default:
      return;
  }
}
#endif  // RL_DEBUG
}  // namespace rl