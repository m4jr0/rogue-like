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

#ifndef ENGINE_PHYSICS_COLLIDER_H_
#define ENGINE_PHYSICS_COLLIDER_H_

#include "engine/common.h"
#include "engine/core/value_utils.h"
#include "engine/transform/transform.h"

namespace rl {
using HalfExtents = Vec2<Distance>;

struct Extents {
  f32 minX{.0f};
  f32 maxX{.0f};
  f32 minY{.0f};
  f32 maxY{.0f};
};

struct ClosestInfo {
  f32 dx{.0f};
  f32 dy{.0f};
  f32 dist2{.0f};
};

struct Aabb {
  Position center;
  HalfExtents halfExtents;
};

struct Circle {
  Position center{Position::zero()};
  f32 radius{.0f};
};

using CollisionFlags = u32;

enum CollisionFlagBits : CollisionFlags {
  kCollisionFlagBitsNone = 0x0,
  kCollisionFlagBitsWorld = 0x1,
  kCollisionFlagBitsCharacter = 0x2,
  kCollisionFlagBitsHurtbox = 0x4,
  kCollisionFlagBitsHitbox = 0x8,
  kCollisionFlagBitsProjectile = 0x10,
  kCollisionFlagBitsTrigger = 0x20,
  kCollisionFlagBitsProp = 0x40,
  kCollisionFlagBitsAll = static_cast<CollisionFlags>(-1),
};

struct CollisionFilter {
  CollisionFlags category{kCollisionFlagBitsNone};
  CollisionFlags mask{kCollisionFlagBitsNone};
};

inline bool shouldCollide(const CollisionFilter& a, const CollisionFilter& b) {
  return (a.mask & b.category) && (b.mask & a.category);
}

enum class ColliderShape : u8 { Unknown = 0, Aabb, Circle };

struct Collider {
  ColliderShape shape{ColliderShape::Unknown};

  union {
    Aabb aabb;
    Circle circle;
  } bounds;

  bool flat() const noexcept {
    switch (shape) {
      using enum ColliderShape;
      case Aabb:
        return almostZero(bounds.aabb.halfExtents.magSqrd());
      case Circle:
        return almostZero(bounds.circle.radius);
      case Unknown:
      default:
        return true;
    }
  }
};

enum class ColliderTrackTimePolicy : u8 {
  Normalized,
  AbsoluteSeconds,
  FixedFrames
};

using CollisionHitCount = u32;
constexpr auto kInvalidCollisionHitCount = static_cast<CollisionHitCount>(-1);

inline constexpr bool noHitCount(CollisionHitCount c) noexcept {
  return c == kInvalidCollisionHitCount;
}
}  // namespace rl

namespace std {
inline std::ostream& operator<<(std::ostream& os, const ::rl::Aabb& a) {
  return os << "Aabb[x: " << a.center.x << ", y: " << a.center.y
            << ", hw: " << a.halfExtents.x << ", hh: " << a.halfExtents.y
            << "]";
}

inline std::ostream& operator<<(std::ostream& os, const ::rl::Circle& c) {
  return os << "Circle[x: " << c.center.x << ", y: " << c.center.y
            << ", r: " << c.radius << "]";
}
}  // namespace std

#endif  // ENGINE_PHYSICS_COLLIDER_H_
