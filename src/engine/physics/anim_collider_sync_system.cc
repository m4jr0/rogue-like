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
#include "anim_collider_sync_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/anim/anim_library.h"
#include "engine/anim/anim_system.h"
#include "engine/core/log.h"
#include "engine/core/spatial_ref.h"
#include "engine/core/vector.h"
#include "engine/physics/anim_collider.h"
#include "engine/physics/anim_collider_library.h"
#include "engine/physics/hitbox_system.h"
#include "engine/physics/physics_utils.h"

namespace rl {
AnimColliderSyncSystem& AnimColliderSyncSystem::instance() {
  static AnimColliderSyncSystem inst;
  return inst;
}

void AnimColliderSyncSystem::init() {
  RL_LOG_DEBUG("AnimColliderSyncSystem::init");
  constexpr auto kRigCapacity = 64;
  hRigPool_.clear();
  hRigPool_.reserve(kRigCapacity);
  rigs_.reserve(kRigCapacity);
}

void AnimColliderSyncSystem::shutdown() {
  RL_LOG_DEBUG("AnimColliderSyncSystem::shutdown");
  hRigPool_.clear();
  rigs_.clear();
}

void AnimColliderSyncSystem::tick(const FramePacket&) {
  for (auto& r : rigs_) {
    if (!hRigPool_.alive(r.handle)) continue;
    auto frame = RL_CANIMSYS.animFrame(r.animator);

    if (frame == kInvalidAnimFrame) {
      deactivate(r);
      continue;
    }

    auto animSetId = RL_CANIMSYS.animSet(r.animator);

    if (!r.animSet || animSetId != r.animSet->id) {
      if (!animSetId) {
        r.animSet = nullptr;
        r.profile = nullptr;
      } else {
        r.animSet = RL_CANIMLIB.get(animSetId);
        if (r.animSet && r.animSet->colliderProfile) {
          r.profile = RL_CANIMCOLLIB.get(r.animSet->colliderProfile);
        } else {
          r.profile = nullptr;
        }
      }
    }

    if (!r.animSet || !r.profile) {
      deactivate(r);
      continue;
    }

    auto animIdx = RL_CANIMSYS.animIdx(r.animator);
    r.animIdx = animIdx;

    if (r.animIdx == kInvalidIndex || r.animIdx >= r.profile->perAnim.size()) {
      deactivate(r);
      continue;
    }

    const auto& colliderSet = r.profile->perAnim[r.animIdx];
    update(frame, colliderSet, r);
  }
}

AnimColliderRigHandle AnimColliderSyncSystem::generate(
    const AnimColliderRigDesc& desc) {
  if (!desc.ref.valid()) {
    return kInvalidHandle;
  }

  auto animSetId = RL_CANIMSYS.animSet(desc.animator);
  if (!animSetId) return kInvalidHandle;
  const auto* animSet = RL_CANIMLIB.get(animSetId);
  if (!animSet || !animSet->colliderProfile) return kInvalidHandle;
  const auto* colliderProfile = RL_CANIMCOLLIB.get(animSet->colliderProfile);
  if (!colliderProfile) return kInvalidHandle;

  auto h = hRigPool_.generate();
  ensureCapacity(rigs_, h.index);
  auto& r = rigs_[h.index];
  r.handle = h;
  r.animator = desc.animator;
  r.animSet = animSet;
  r.profile = colliderProfile;

  r.ref = desc.ref;

  usize maxHits = 0;
  usize maxHurts = 0;

  for (const auto& set : colliderProfile->perAnim) {
    maxHits = std::max(maxHits, set.hits.size());
    maxHurts = std::max(maxHurts, set.hurts.size());
  }

  r.hitHandles.resize(maxHits);
  r.hurtHandles.resize(maxHurts);

  CollisionFilter defaultHitFilter{
      .category = kCollisionFlagBitsHitbox,
      .mask = kCollisionFlagBitsHurtbox,
  };

  CollisionFilter defaultHurtFilter{
      .category = kCollisionFlagBitsHurtbox,
      .mask = kCollisionFlagBitsHitbox,
  };

  for (usize i = 0; i < maxHits; ++i) {
    auto filter =
        i < desc.hitFilters.size() ? desc.hitFilters[i] : defaultHitFilter;

    r.hitHandles[i] = RL_HITBOXSYS.generate({
        .active = false,
        .filter = filter,
        .maxHitCount = kInvalidCollisionHitCount,
        .collider = {},
        .endTime = .0,
        .ref = r.ref,
    });
  }

  for (usize i = 0; i < maxHurts; ++i) {
    auto filter =
        i < desc.hurtFilters.size() ? desc.hurtFilters[i] : defaultHurtFilter;

    r.hurtHandles[i] = RL_HITBOXSYS.onHurt({
        .active = false,
        .filter = filter,
        .collider = {},
        .ref = r.ref,
    });
  }

  return h;
}

void AnimColliderSyncSystem::destroy(AnimColliderRigHandle h) {
  auto* r = rig(h);

  for (auto hit : r->hitHandles) {
    if (hit) RL_HITBOXSYS.destroy(hit);
  }

  for (auto hurt : r->hurtHandles) {
    if (hurt) RL_HITBOXSYS.offHurt(hurt);
  }

  *r = {};
  hRigPool_.destroy(h);
}

void AnimColliderSyncSystem::update(AnimFrame frame, const AnimColliderSet& set,
                                    AnimColliderRig& r) {
  auto hitCount = set.hits.size();
  auto hurtCount = set.hurts.size();
  auto flippedX = RL_CANIMSYS.flippedX(r.animator);

  for (usize i = 0; i < hitCount; ++i) {
    const auto& track = set.hits[i];
    const auto* s = sampleTrack(track, static_cast<usize>(frame));
    auto* h = RL_HITBOXSYS.hitbox(r.hitHandles[i]);

    if (!h || !s || s->collider.shape == ColliderShape::Unknown) {
      h->deactivate();
      continue;
    }

    h->activate();
    h->collider = flippedX ? flipX(s->collider) : s->collider;
  }

  for (usize i = 0; i < hurtCount; ++i) {
    const auto& track = set.hurts[i];
    const auto* s = sampleTrack(track, static_cast<usize>(frame));
    auto* h = RL_HITBOXSYS.hurtbox(r.hurtHandles[i]);

    if (!h || !s) {
      h->active = false;
      continue;
    }

    h->active = true;
    h->collider = flippedX ? flipX(s->collider) : s->collider;
  }

  for (usize i = hitCount; i < r.hitHandles.size(); ++i) {
    if (auto* h = RL_HITBOXSYS.hitbox(r.hitHandles[i])) h->deactivate();
  }

  for (usize i = hurtCount; i < r.hurtHandles.size(); ++i) {
    if (auto* h = RL_HITBOXSYS.hurtbox(r.hurtHandles[i])) h->active = false;
  }
}

void AnimColliderSyncSystem::deactivate(AnimColliderRig& r) {
  for (auto h : r.hitHandles) {
    if (auto* hb = RL_HITBOXSYS.hitbox(h)) hb->deactivate();
  }

  for (auto h : r.hurtHandles) {
    if (auto* hb = RL_HITBOXSYS.hurtbox(h)) hb->active = false;
  }
}

AnimColliderRig* AnimColliderSyncSystem::rig(AnimColliderRigHandle h) {
  RL_ASSERT(h && hRigPool_.alive(h),
            "AnimColliderSyncSystem::rig: Invalid hit rig handle provided!");
  if (!h || !hRigPool_.alive(h)) return nullptr;
  return &rigs_[h.index];
}

const AnimColliderRig* AnimColliderSyncSystem::rig(
    AnimColliderRigHandle h) const {
  RL_ASSERT(h && hRigPool_.alive(h),
            "AnimColliderSyncSystem::rig: Invalid hit rig handle provided!");
  if (!h || !hRigPool_.alive(h)) return nullptr;
  return &rigs_[h.index];
}
}  // namespace rl