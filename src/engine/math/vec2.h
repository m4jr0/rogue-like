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

#ifndef ENGINE_MATH_VEC2_H_
#define ENGINE_MATH_VEC2_H_

#include "engine/common.h"
#include "engine/core/type_trait.h"

namespace rl {
template <Arithmetic T>
struct Vec2 {
  T x;
  T y;

  constexpr Vec2() = default;
  constexpr Vec2(T x, T y) : x{x}, y{y} {}

  constexpr T magSqrd() const { return x * x + y * y; }
  constexpr T mag() const { return std::sqrt(magSqrd()); }

  constexpr Vec2 normalized() noexcept {
    auto m2 = magSqrd();

    if (m2 > static_cast<T>(0)) {
      auto inv = static_cast<T>(1) / std::sqrt(m2);
      return {x * inv, y * inv};
    } else {
      return zero();
    }
  }

  constexpr Vec2 clampMag(T maxMag) {
    auto m2 = magSqrd();

    if (m2 > maxMag * maxMag) {
      auto inv = maxMag / std::sqrt(m2);
      return {x * inv, y * inv};
    }

    return *this;
  }

  [[nodiscard]] constexpr Vec2 operator+(const Vec2& v) const {
    return {x + v.x, y + v.y};
  }

  [[nodiscard]] constexpr Vec2 operator-(const Vec2& v) const {
    return {x - v.x, y - v.y};
  }

  [[nodiscard]] constexpr Vec2 operator*(T s) const { return {x * s, y * s}; }

  [[nodiscard]] constexpr Vec2 operator/(T s) const {
    RL_ASSERT(s != T{0}, "Vec2::operator/: Cannot divide by 0!");
    return {x / s, y / s};
  }

  [[nodiscard]] constexpr Vec2 operator-() const noexcept { return {-x, -y}; }

  constexpr Vec2& operator+=(const Vec2& v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  constexpr Vec2& operator-=(const Vec2& v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  constexpr Vec2& operator*=(T s) {
    x *= s;
    y *= s;
    return *this;
  }

  constexpr Vec2& operator/=(T s) {
    RL_ASSERT(s != T{0}, "Vec2::operator/: Cannot divide by 0!");
    x /= s;
    y /= s;
    return *this;
  }

  [[nodiscard]] constexpr bool operator==(const Vec2& v) const noexcept {
    return x == v.x && y == v.y;
  }

  [[nodiscard]] constexpr bool operator!=(const Vec2& v) const noexcept {
    return !(*this == v);
  }

  template <Arithmetic U>
  explicit constexpr Vec2(const Vec2<U>& v) noexcept : x(T{v.x}), y(T{v.y}) {}

  static constexpr Vec2 zero() noexcept { return {T{0}, T{0}}; }
  static constexpr Vec2 unitX() noexcept { return {T{1}, T{0}}; }
  static constexpr Vec2 unitY() noexcept { return {T{0}, T{1}}; }
};

using Vec2S8 = Vec2<s8>;
using Vec2S16 = Vec2<s16>;
using Vec2S32 = Vec2<s32>;
using Vec2S64 = Vec2<s64>;

using Vec2U8 = Vec2<u8>;
using Vec2U16 = Vec2<u16>;
using Vec2U32 = Vec2<u32>;
using Vec2U64 = Vec2<u64>;

using Vec2F32 = Vec2<f32>;
using Vec2F64 = Vec2<f64>;

template <typename T>
using Point = Vec2<T>;

using PointS8 = Point<s8>;
using PointS16 = Point<s16>;
using PointS32 = Point<s32>;
using PointS64 = Point<s64>;

using PointU8 = Point<u8>;
using PointU16 = Point<u16>;
using PointU32 = Point<u32>;
using PointU64 = Point<u64>;

using PointF32 = Point<f32>;
using PointF64 = Point<f64>;
}  // namespace rl

#endif  // ENGINE_MATH_VEC2_H_
