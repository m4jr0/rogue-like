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

#ifndef ENGINE_PHYSICS_PHYSICS_UTILS_H_
#define ENGINE_PHYSICS_PHYSICS_UTILS_H_

#include "engine/common.h"
#include "engine/core/spatial_ref.h"
#include "engine/core/type_trait.h"
#include "engine/math/geometry.h"
#include "engine/math/trs.h"
#include "engine/physics/collider.h"
#include "engine/physics/physics.h"
#include "engine/physics/physics_body.h"

namespace rl {
Steering steeringFromMask(SteeringMask m);

inline f32 approachRate(f32 current, f32 target, f32 rate, f32 dt);
inline f32 approachExp(f32 current, f32 target, f32 tau, f32 dt);

void applyTransform(PhysicsBody& b);
void applyAcceleration(PhysicsBody& b, f32 dt);
void applyVelocity(PhysicsBody& b, f32 dt);
void applyDirection(PhysicsBody& b);

template <Floating T>
[[nodiscard]] CardinalDir angleToCardinalDir(T a) noexcept {
  // Front/Back take precedence at +/-45 degree boundaries, so Left/Right are
  // exclusive.
  if (std::fabs(a) < kPiF32 * .25f) return CardinalDir::Right;

  if (a > .0f) {
    return (a <= kPiF32 * .75f) ? CardinalDir::Back : CardinalDir::Left;
  } else {
    return (a >= -kPiF32 * .75f) ? CardinalDir::Front : CardinalDir::Left;
  }
}

[[nodiscard]] inline CardinalDir dirToCardinalDir(Dir dir) noexcept {
  return angleToCardinalDir(angleOf(dir));
}

[[nodiscard]] inline constexpr Distance cardinalDirToAngle(CardinalDir d) {
  switch (d) {
    case CardinalDir::Front:
      return -kPiF32 * .5f;
    case CardinalDir::Right:
      return .0f;
    case CardinalDir::Left:
      return kPiF32;
    case CardinalDir::Back:
      return kPiF32 * .5f;
    default:
      return .0f;
  }
};

[[nodiscard]] Aabb aabbOf(const Collider& c);
[[nodiscard]] Extents extentsOf(const Aabb& a);
[[nodiscard]] ClosestInfo closestPointToAabb(const Aabb& collider,
                                             const Circle& c);
[[nodiscard]] Aabb worldAabb(const Aabb& local, const Trs& trs);
[[nodiscard]] Circle worldCircle(const Circle& local, const Trs& trs);

[[nodiscard]] bool overlap(const Aabb& a, const Aabb& b);
[[nodiscard]] bool overlap(const Circle& a, const Circle& b);
[[nodiscard]] bool overlap(const Aabb& a, const Circle& b);
[[nodiscard]] bool overlap(const Circle& a, const Aabb& b);
[[nodiscard]] bool overlap(const Collider& a, const Collider& b);

[[nodiscard]] Position contactPoint(const Collider& a, const Collider& b);

void updateWorldCollider(const Collider& collider, TransformHandle trans,
                         Collider& wCollider);
void updateWorldCollider(const Collider& collider, Position pos,
                         Collider& wCollider);
void updateWorldCollider(const Collider& collider, const SpatialRef& ref,
                         Collider& wCollider);

[[nodiscard]] bool mtvCircleCircle(const Circle& a, const Circle& b,
                                   Dir& outMtv);
[[nodiscard]] bool mtvAabbAabb(const Aabb& a, const Aabb& b, Dir& outMtv);
[[nodiscard]] bool mtvAabbCircle(const Aabb& b, const Circle& c, Dir& outMtv);

[[nodiscard]] inline bool mtvCircleAabb(const Circle& b, const Aabb& c,
                                        Dir& outMtv) {
  auto ok = mtvAabbCircle(c, b, outMtv);

  if (ok) {
    outMtv.x = -outMtv.x;
    outMtv.y = -outMtv.y;
  }

  return ok;
}

[[nodiscard]] bool mtv(const Collider& a, const Collider& b, Dir& outMtv);
void applyMtv(PhysicsBody& a, PhysicsBody& b, const Dir& outMtv);

[[nodiscard]] Collider flipX(const Collider& collider);
}  // namespace rl

#endif  // ENGINE_PHYSICS_PHYSICS_UTILS_H_
