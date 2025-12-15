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
#include "physics_utils.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/value_utils.h"
#include "engine/math/math_common.h"
#include "engine/transform/transform.h"
#include "engine/transform/transform_system.h"

namespace rl {
Steering steeringFromMask(SteeringMask m) {
  Distance x = .0f;
  Distance y = .0f;

  if ((m & kSteeringMaskBitsUp) != 0) y += 1.0f;
  if ((m & kSteeringMaskBitsLeft) != 0) x -= 1.0f;
  if ((m & kSteeringMaskBitsRight) != 0) x += 1.0f;
  if ((m & kSteeringMaskBitsDown) != 0) y -= 1.0f;

  // Case: diagonal of sqrt(2); we normalize and return.
  if (x != .0f && y != .0f) {
    // Normalized diagonal.
    constexpr f32 kInvSqrt2 = .7071067811865476f;
    x *= kInvSqrt2;
    y *= kInvSqrt2;
  }

  return {x, y};
}

inline f32 approachRate(f32 current, f32 target, f32 rate, f32 dt) {
  auto delta = target - current;
  auto step = rate * dt;
  if (std::fabs(delta) <= step) return target;
  return current + sgn(delta) * step;
}

inline f32 approachExp(f32 current, f32 target, f32 tau, f32 dt) {
  auto a = 1.0f - std::exp(-dt / avoidNegOrZero(tau));
  return current + (target - current) * a;
}

void applyTransform(PhysicsBody& b) {
  if (!b.trans) return;
  updateWorldCollider(b.collider, b.trans, b.wCollider);
}

void applyAcceleration(PhysicsBody& b, f32 dt) {
  if (!b.dynamic) return;
  Vec2F32 targetVel = {b.steer.x * b.maxVel.x, b.steer.y * b.maxVel.y};

  auto stopX = almostZero(targetVel.x);
  auto stopY = almostZero(targetVel.y);

  auto tauX =
      (stopX || sgn(targetVel.x) != sgn(b.vel.x)) ? b.tauBrake : b.tauAccel;
  auto tauY =
      (stopY || sgn(targetVel.y) != sgn(b.vel.y)) ? b.tauBrake : b.tauAccel;

  if (stopX && stopY && almostZero(b.steer)) {
    tauX *= b.tauStopBoost;
    tauY *= b.tauStopBoost;
  }

  b.vel.x = approachExp(b.vel.x, targetVel.x, tauX, dt);
  b.vel.y = approachExp(b.vel.y, targetVel.y, tauY, dt);
  if (almostZero(b.vel.x)) b.vel.x = .0f;
  if (almostZero(b.vel.y)) b.vel.y = .0f;

  b.extVel.x = approachRate(b.extVel.x, .0f, b.extDec.x, dt);
  b.extVel.y = approachRate(b.extVel.y, .0f, b.extDec.y, dt);
  if (almostZero(b.extVel.x)) b.extVel.x = .0f;
  if (almostZero(b.extVel.y)) b.extVel.y = .0f;
}

void applyVelocity(PhysicsBody& b, f32 dt) {
  if (!b.dynamic) return;
  auto* t = RL_TRANSSYS.local(b.trans);
  if (!t) return;
  t->pos += (b.vel + b.extVel) * dt;
  t->dirty = true;
}

void applyDirection(PhysicsBody& b) {
  if (!b.dynamic) return;
  if (almostZero(b.steer)) return;
  b.dir = b.steer.normalized();
  b.cardDir = dirToCardinalDir(b.dir);
}

[[nodiscard]] Aabb aabbOf(const Collider& c) {
  switch (c.shape) {
    using enum ColliderShape;

    case Aabb:
      return c.bounds.aabb;

    case Circle: {
      auto r = c.bounds.circle.radius;
      return {
          .center = c.bounds.circle.center,
          .halfExtents = {r, r},
      };
    }

    default:
      return {};
  }
}

[[nodiscard]] Extents extentsOf(const Aabb& a) {
  return {
      .minX = a.center.x - a.halfExtents.x,
      .maxX = a.center.x + a.halfExtents.x,
      .minY = a.center.y - a.halfExtents.y,
      .maxY = a.center.y + a.halfExtents.y,
  };
}

[[nodiscard]] ClosestInfo closestPointToAabb(const Aabb& collider,
                                             const Circle& c) {
  auto ex = extentsOf(collider);
  auto cx = std::clamp(c.center.x, ex.minX, ex.maxX);
  auto cy = std::clamp(c.center.y, ex.minY, ex.maxY);
  auto dx = c.center.x - cx;
  auto dy = c.center.y - cy;
  return {dx, dy, dx * dx + dy * dy};
}

[[nodiscard]] Aabb worldAabb(const Aabb& local, const Trs& trs) {
  return {
      .center = trs.transformPoint(local.center),
      .halfExtents = trs.transformAxisAlignedSize(local.halfExtents),
  };
}

[[nodiscard]] Circle worldCircle(const Circle& local, const Trs& trs) {
  auto wCenter = trs.transformPoint(local.center);

  auto unitX = trs.transformVector(Vec2F32::unitX());
  auto unitY = trs.transformVector(Vec2F32::unitY());

  auto scaleX = unitX.mag();
  auto scaleY = unitY.mag();
  // Case: non-uniform scaling (circle turned into an ellipse).
  // Use the largest axis scale so the new circle fully encloses it.
  auto scale = std::max(scaleX, scaleY);
  auto wRadius = local.radius * scale;

  return Circle{wCenter, wRadius};
}

[[nodiscard]] bool overlap(const Aabb& a, const Aabb& b) {
  auto ea = extentsOf(a);
  auto eb = extentsOf(b);

  auto xOverlap = ea.minX <= eb.maxX && ea.maxX >= eb.minX;
  auto yOverlap = ea.minY <= eb.maxY && ea.maxY >= eb.minY;

  return xOverlap && yOverlap;
}

[[nodiscard]] bool overlap(const Circle& a, const Circle& b) {
  auto dx = a.center.x - b.center.x, dy = a.center.y - b.center.y;
  auto r = a.radius + b.radius;
  return dx * dx + dy * dy <= r * r;
}

[[nodiscard]] bool overlap(const Aabb& a, const Circle& b) {
  auto info = closestPointToAabb(a, b);
  return info.dist2 <= b.radius * b.radius;
}

[[nodiscard]] bool overlap(const Circle& a, const Aabb& b) {
  return overlap(b, a);
}

[[nodiscard]] bool overlap(const Collider& a, const Collider& b) {
  if (a.shape == b.shape) {
    switch (a.shape) {
      using enum ColliderShape;
      case Aabb:
        return overlap(a.bounds.aabb, b.bounds.aabb);
      case Circle:
        return overlap(a.bounds.circle, b.bounds.circle);
      default:
        return false;
    }
  }

  switch (a.shape) {
    using enum ColliderShape;
    case Aabb:
      return overlap(a.bounds.aabb, b.bounds.circle);
    case Circle:
      return overlap(a.bounds.circle, b.bounds.aabb);
    default:
      return false;
  }
}

[[nodiscard]] Position contactPoint(const Collider& a, const Collider& b) {
  auto aa = aabbOf(a);
  auto bb = aabbOf(b);
  auto ea = extentsOf(aa);
  auto eb = extentsOf(bb);

  auto iMinX = std::max(ea.minX, eb.minX);
  auto iMaxX = std::min(ea.maxX, eb.maxX);
  auto iMinY = std::max(ea.minY, eb.minY);
  auto iMaxY = std::min(ea.maxY, eb.maxY);

  return {
      (iMinX + iMaxX) * .5f,
      (iMinY + iMaxY) * .5f,
  };
}

void updateWorldCollider(const Collider& collider, TransformHandle trans,
                         Collider& wCollider) {
  wCollider = {};
  wCollider.shape = collider.shape;

  if (!trans) return;
  const auto* t = RL_CTRANSSYS.global(trans);
  if (!t) return;

  switch (collider.shape) {
    using enum ColliderShape;

    case Aabb:
      wCollider.bounds.aabb = worldAabb(collider.bounds.aabb, t->trs);
      return;

    case Circle:
      wCollider.bounds.circle = worldCircle(collider.bounds.circle, t->trs);
      return;

    default:
      wCollider = {};
      return;
  }
}

void updateWorldCollider(const Collider& collider, Position pos,
                         Collider& wCollider) {
  wCollider = {};
  wCollider.shape = collider.shape;

  switch (collider.shape) {
    using enum ColliderShape;

    case Aabb:
      wCollider.bounds.aabb = collider.bounds.aabb;
      wCollider.bounds.aabb.center = wCollider.bounds.aabb.center + pos;
      return;

    case Circle:
      wCollider.bounds.circle = collider.bounds.circle;
      wCollider.bounds.circle.center = wCollider.bounds.circle.center + pos;
      return;

    default:
      wCollider = {};
      return;
  }
}

void updateWorldCollider(const Collider& collider, const SpatialRef& ref,
                         Collider& wCollider) {
  switch (ref.type) {
    case SpatialRefType::Transform:
      updateWorldCollider(collider, ref.trans, wCollider);
      return;

    case SpatialRefType::Position:
      updateWorldCollider(collider, ref.pos, wCollider);
      return;

    case SpatialRefType::None:
    default:
      wCollider = {};
      return;
  }
}

[[nodiscard]] bool mtvCircleCircle(const Circle& a, const Circle& b,
                                   Dir& outMtv) {
  auto dx = b.center.x - a.center.x;
  auto dy = b.center.y - a.center.y;
  auto r = a.radius + b.radius;

  auto magSquared = dx * dx + dy * dy;

  if (almostZero(magSquared)) {
    outMtv = {r, .0f};
    return true;
  }

  auto d = std::sqrt(magSquared);

  // Case: no overlap.
  if (d >= r) return false;

  auto penetration = r - d;
  auto nx = dx / d;
  auto ny = dy / d;

  outMtv = {nx * penetration, ny * penetration};
  return true;
}

[[nodiscard]] bool mtvAabbAabb(const Aabb& a, const Aabb& b, Dir& outMtv) {
  auto ea = extentsOf(a);
  auto eb = extentsOf(b);

  auto overlapX = std::min(ea.maxX, eb.maxX) - std::max(ea.minX, eb.minX);
  auto overlapY = std::min(ea.maxY, eb.maxY) - std::max(ea.minY, eb.minY);

  if (overlapX <= .0f || overlapY <= .0f) {
    return false;
  }

  auto dx = b.center.x - a.center.x;
  auto dy = b.center.y - a.center.y;

  if (overlapX < overlapY) {
    auto sx = (dx < .0f) ? -1.0f : 1.0f;
    outMtv = {sx * overlapX, .0f};
  } else {
    auto sy = (dy < .0f) ? -1.0f : 1.0f;
    outMtv = {.0f, sy * overlapY};
  }

  return true;
}

[[nodiscard]] bool mtvAabbCircle(const Aabb& b, const Circle& c, Dir& outMtv) {
  auto ex = extentsOf(b);
  auto info = closestPointToAabb(b, c);

  // Case: center outside box (normal circle-vs-AABB case).
  if (!almostZero(info.dist2)) {
    auto d = std::sqrt(info.dist2);
    auto penetration = c.radius - d;
    if (penetration <= .0f) return false;

    auto nx = info.dx / d;
    auto ny = info.dy / d;
    outMtv = {nx * penetration, ny * penetration};
    return true;
  }

  // Case: circle center inside the box. Psh it out along the smallest axis so
  // that the circle ends up just outside the box.
  auto distLeft = c.center.x - ex.minX;
  auto distRight = ex.maxX - c.center.x;
  auto distBottom = c.center.y - ex.minY;
  auto distTop = ex.maxY - c.center.y;

  // Find closest face.
  f32 minDist = distLeft;
  Dir dir = {-1.0f, .0f};

  if (distRight < minDist) {
    minDist = distRight;
    dir = {1.0f, .0f};
  }

  if (distBottom < minDist) {
    minDist = distBottom;
    dir = {.0f, -1.0f};
  }

  if (distTop < minDist) {
    minDist = distTop;
    dir = {.0f, 1.0f};
  }

  auto move = minDist + c.radius;
  outMtv = {dir.x * move, dir.y * move};
  return true;
}

[[nodiscard]] bool mtv(const Collider& a, const Collider& b, Dir& outMtv) {
  auto ok = false;

  if (a.shape == ColliderShape::Circle && b.shape == ColliderShape::Circle) {
    ok = mtvCircleCircle(a.bounds.circle, b.bounds.circle, outMtv);
  } else if (a.shape == ColliderShape::Aabb && b.shape == ColliderShape::Aabb) {
    ok = mtvAabbAabb(a.bounds.aabb, b.bounds.aabb, outMtv);
  } else if (a.shape == ColliderShape::Circle &&
             b.shape == ColliderShape::Aabb) {
    ok = mtvCircleAabb(a.bounds.circle, b.bounds.aabb, outMtv);
  } else if (a.shape == ColliderShape::Aabb &&
             b.shape == ColliderShape::Circle) {
    ok = mtvCircleAabb(b.bounds.circle, a.bounds.aabb, outMtv);

    if (ok) {
      outMtv.x = -outMtv.x;
      outMtv.y = -outMtv.y;
    }
  } else {
    outMtv = {};
  }

  return ok;
}

void applyMtv(PhysicsBody& a, PhysicsBody& b, const Dir& mtv) {
  auto* ta = RL_TRANSSYS.local(a.trans);
  auto* tb = RL_TRANSSYS.local(b.trans);
  if (!ta || !tb) return;

  auto wa = a.dynamic ? 1.0f : .0f;
  auto wb = b.dynamic ? 1.0f : .0f;
  auto sum = wa + wb;

  // Case: both are static.
  if (sum <= .0f) {
    return;
  }

  wa /= sum;
  wb /= sum;

  ta->pos.x -= mtv.x * wa;
  ta->pos.y -= mtv.y * wa;

  tb->pos.x += mtv.x * wb;
  tb->pos.y += mtv.y * wb;

  ta->dirty = true;
  tb->dirty = true;
}

[[nodiscard]] Collider flipX(const Collider& collider) {
  switch (collider.shape) {
    using enum ColliderShape;

    case Aabb: {
      const auto& aabb = collider.bounds.aabb;
      return Collider{
          .shape = ColliderShape::Aabb,
          .bounds =
              {
                  .aabb =
                      {
                          .center = Position{-aabb.center.x, aabb.center.y},
                          .halfExtents = aabb.halfExtents,
                      },
              },
      };
    }

    case Circle: {
      const auto& c = collider.bounds.circle;
      return Collider{
          .shape = ColliderShape::Circle,
          .bounds =
              {
                  .circle =
                      {
                          .center = Position{-c.center.x, c.center.y},
                          .radius = c.radius,
                      },
              },
      };
    }

    case Unknown:
    default:
      return Collider{
          .shape = ColliderShape::Unknown,
          .bounds =
              {
                  .aabb = {},
              },
      };
  }
}
}  // namespace rl