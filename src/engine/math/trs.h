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

#ifndef ENGINE_MATH_TRS_H_
#define ENGINE_MATH_TRS_H_

#include "engine/common.h"
#include "engine/core/value_utils.h"
#include "engine/math/mat.h"
#include "engine/math/vec2.h"

namespace rl {
// 2D affine TRS matrix.
// [ rs0x rs1x tx ]
// [ rs0y rs1y ty ]
// [  0    0   1  ]
struct Trs : Mat4F32 {
  using Base = Mat4F32;
  using Base::Base;

  constexpr Trs(f32 rs0x, f32 rs0y, f32 rs1x, f32 rs1y, f32 tx, f32 ty) noexcept
      : Base{
            rs0x, rs0y, .0f,  .0f, rs1x, rs1y, .0f, .0f,
            .0f,  .0f,  1.0f, .0f, tx,   ty,   .0f, 1.0f,
        } {}

  static constexpr Trs identity() {
    Trs r{};
    r.m[0] = r.m[5] = r.m[10] = r.m[15] = 1.0f;
    return r;
  }

  constexpr f32 rs0x() const noexcept { return m[0]; }
  constexpr f32 rs0y() const noexcept { return m[1]; }
  constexpr f32 rs1x() const noexcept { return m[4]; }
  constexpr f32 rs1y() const noexcept { return m[5]; }
  constexpr f32 tx() const noexcept { return m[12]; }
  constexpr f32 ty() const noexcept { return m[13]; }

  constexpr void rs0x(f32 v) noexcept { m[0] = v; }
  constexpr void rs0y(f32 v) noexcept { m[1] = v; }
  constexpr void rs1x(f32 v) noexcept { m[4] = v; }
  constexpr void rs1y(f32 v) noexcept { m[5] = v; }
  constexpr void tx(f32 v) noexcept { m[12] = v; }
  constexpr void ty(f32 v) noexcept { m[13] = v; }

  [[nodiscard]] static constexpr Trs translation(f32 tx, f32 ty) noexcept {
    Trs r = identity();
    r.tx(tx);
    r.ty(ty);
    return r;
  }

  [[nodiscard]] static Trs rotation(f32 rot) noexcept {
    auto r = Trs::identity();
    auto c = std::cos(rot);
    auto s = std::sin(rot);

    r.rs0x(c);
    r.rs0y(s);
    r.rs1x(-s);
    r.rs1y(c);

    return r;
  }

  [[nodiscard]] static constexpr Trs scaling(f32 sx, f32 sy) noexcept {
    auto r = Trs::identity();
    r.rs0x(sx);
    r.rs1y(sy);
    return r;
  }

  [[nodiscard]] static constexpr Trs scaling(f32 s) noexcept {
    return scaling(s, s);
  }

  [[nodiscard]] static Trs trs(const Vec2F32& pos, f32 rot,
                               const Vec2F32& scale) {
    auto c = std::cos(rot);
    auto s = std::sin(rot);

    return {
        c * scale.x, s * scale.x, -s * scale.y, c * scale.y, pos.x, pos.y,
    };
  }

  constexpr Trs& translateLocal(f32 tx, f32 ty) noexcept {
    *this = *this * Trs::translation(tx, ty);
    return *this;
  }

  constexpr Trs& translateGlobal(f32 tx, f32 ty) noexcept {
    *this = Trs::translation(tx, ty) * *this;
    return *this;
  }

  constexpr Trs& translate(f32 tx, f32 ty) noexcept {
    return translateLocal(tx, ty);
  }

  Trs& rotateLocal(f32 rot) noexcept {
    *this = *this * Trs::rotation(rot);
    return *this;
  }

  Trs& rotateGlobal(f32 rot) noexcept {
    *this = Trs::rotation(rot) * *this;
    return *this;
  }

  Trs& rotate(f32 rot) noexcept { return rotateLocal(rot); }

  constexpr Trs& scaleLocal(f32 sx, f32 sy) noexcept {
    *this = *this * Trs::scaling(sx, sy);
    return *this;
  }

  constexpr Trs& scaleLocal(f32 s) noexcept { return scaleLocal(s, s); }

  constexpr Trs& scaleGlobal(f32 sx, f32 sy) noexcept {
    *this = Trs::scaling(sx, sy) * *this;
    return *this;
  }

  constexpr Trs& scaleGlobal(f32 s) noexcept { return scaleGlobal(s, s); }

  constexpr Trs& scale(f32 sx, f32 sy) noexcept { return scaleLocal(sx, sy); }

  constexpr Trs& scale(f32 s) noexcept { return scale(s, s); }

  [[nodiscard]] constexpr Vec2F32 transformPoint(
      const Vec2F32& p) const noexcept {
    // Add translation (since it's a point).
    return {rs0x() * p.x + rs1x() * p.y + tx(),
            rs0y() * p.x + rs1y() * p.y + ty()};
  }

  [[nodiscard]] constexpr Vec2F32 transformVector(
      const Vec2F32& v) const noexcept {
    // Ignore translation (since it's a vector).
    return {rs0x() * v.x + rs1x() * v.y, rs0y() * v.x + rs1y() * v.y};
  }

  [[nodiscard]] constexpr Vec2F32 transformAxisAlignedSize(
      const Vec2F32& size) const noexcept {
    auto a00 = (rs0x() < .0f ? -rs0x() : rs0x());
    auto a01 = (rs1x() < .0f ? -rs1x() : rs1x());
    auto a10 = (rs0y() < .0f ? -rs0y() : rs0y());
    auto a11 = (rs1y() < .0f ? -rs1y() : rs1y());
    return {a00 * size.x + a01 * size.y, a10 * size.x + a11 * size.y};
  }

  [[nodiscard]] constexpr Trs inverse() const noexcept {
    auto rs0x = this->rs0x();
    auto rs1x = this->rs1x();
    auto rs0y = this->rs0y();
    auto rs1y = this->rs1y();
    auto det = rs0x * rs1y - rs1x * rs0y;

    if (almostZero(det)) {
      return identity();
    }

    auto tx = this->tx();
    auto ty = this->ty();
    auto invDet = 1.0f / det;

    auto invRs0x = rs1y * invDet;
    auto invRs1x = -rs1x * invDet;
    auto invRs0y = -rs0y * invDet;
    auto invRs1y = rs0x * invDet;

    auto invTx = -(invRs0x * tx + invRs1x * ty);
    auto invTy = -(invRs0y * tx + invRs1y * ty);

    return {invRs0x, invRs0y, invRs1x, invRs1y, invTx, invTy};
  }

  [[nodiscard]] friend constexpr Trs operator*(const Trs& a,
                                               const Trs& b) noexcept {
    Trs r;
    static_cast<Base&>(r) = Base::mul(a, b);
    return r;
  }

  Trs& operator*=(const Trs& rhs) noexcept {
    Base& self = *this;
    self = Base::mul(self, rhs);
    return *this;
  }

  [[nodiscard]] friend constexpr Trs operator*(const Trs& a,
                                               const Mat4F32& b) noexcept {
    Trs r;
    static_cast<Base&>(r) = Base::mul(a, b);
    return r;
  }

  [[nodiscard]] friend constexpr Trs operator*(const Mat4F32& a,
                                               const Trs& b) noexcept {
    Trs r;
    static_cast<Base&>(r) = Base::mul(a, b);
    return r;
  }
};
}  // namespace rl

#endif  // ENGINE_MATH_TRS_H_
