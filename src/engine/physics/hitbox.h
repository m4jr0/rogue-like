// Copyright 2025 m4jr0. All Rights Reserved.
//-
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

#ifndef GAME_PHYSICS_HITBOX_H_
#define GAME_PHYSICS_HITBOX_H_

#include "engine/common.h"
#include "engine/core/spatial_ref.h"
#include "engine/physics/collider.h"

namespace rl {
struct HitboxTag {};
using HitboxHandle = Handle<HitboxTag>;

struct HitboxDesc {
  bool active{false};
  CollisionFilter filter{};
  CollisionHitCount maxHitCount{kInvalidCollisionHitCount};
  Collider collider{};
  f64 endTime{.0};
  SpatialRef ref{};
};

using HitboxFlags = u8;

enum HitboxFlagBits : HitboxFlags {
  kHitboxFlagBitsNone = 0x0,
  kHitboxFlagBitsActive = 0x1,
  kHitboxFlagBitsConsumed = 0x2,
  kHitboxFlagBitsAll = static_cast<HitboxFlags>(-1),
};

using BoxIndex = u8;

struct Hitbox {
  BoxIndex logicalIndex{0};
  HitboxFlags flags{kHitboxFlagBitsNone};
  HitboxHandle handle{kInvalidHandle};
  CollisionFilter filter{};
  CollisionHitCount hitCount{0};
  CollisionHitCount maxHitCount{kInvalidCollisionHitCount};
  Collider collider{};
  Collider wCollider{};
  f64 endTime{.0};
  SpatialRef ref{};

  void start(Hitbox& h, f64 time, f64 duration) noexcept {
    h.flags |= kHitboxFlagBitsActive;
    h.flags &= ~kHitboxFlagBitsConsumed;
    h.hitCount = 0;
    h.endTime = time + duration;
  }

  void activate() { flags |= kHitboxFlagBitsActive; }
  void deactivate() { flags &= ~kHitboxFlagBitsActive; }
  bool active() const noexcept { return (flags & kHitboxFlagBitsActive); }
  void consume() { flags |= kHitboxFlagBitsConsumed; }
  bool consumed() const noexcept { return (flags & kHitboxFlagBitsConsumed); }
  bool flat() const noexcept { return collider.flat(); }

  bool done(f64 time) const {
    return consumed() || (endTime > 0.0 && time >= endTime);
  }
};

struct HurtboxDesc {
  bool active{true};
  CollisionFilter filter{};
  Collider collider{};
  SpatialRef ref{};
};

struct HurtboxTag {};
using HurtboxHandle = Handle<HurtboxTag>;

using HurtboxStamp = usize;

struct Hurtbox {
  BoxIndex logicalIndex{0};
  bool active{false};
  HurtboxHandle handle{kInvalidHandle};
  CollisionFilter filter{};
  HurtboxStamp seenStamp{0};
  Collider collider{};
  Collider wCollider{};
  SpatialRef ref{};

  bool flat() const noexcept { return collider.flat(); }
};

struct HitContact {
  HitboxHandle hit{kInvalidHandle};
  HurtboxHandle hurt{kInvalidHandle};

  SpatialRef hitRef{};
  SpatialRef hurtRef{};

  Position point{Position::zero()};

  CollisionFilter hitFilter{kCollisionFlagBitsNone};
  CollisionFilter hurtFilter{kCollisionFlagBitsNone};

  f64 time{.0};
};
}  // namespace rl

#endif  // GAME_PHYSICS_HITBOX_H_
