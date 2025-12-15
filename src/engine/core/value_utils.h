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

#ifndef ENGINE_CORE_VALUE_UTILS_H_
#define ENGINE_CORE_VALUE_UTILS_H_

#include "engine/common.h"
#include "engine/core/type_trait.h"
#include "engine/math/vec2.h"

namespace rl {
template <typename T>
constexpr T kEpsilonV = static_cast<T>(1e-6);

constexpr auto kEpsilonF32 = kEpsilonV<f32>;
constexpr auto kEpsilonF64 = kEpsilonV<f64>;

template <Floating T>
[[nodiscard]] inline constexpr bool almostZero(T v,
                                               T eps = kEpsilonV<T>) noexcept {
  return std::abs(v) <= eps;
}

template <Floating T>
[[nodiscard]] inline constexpr bool almostZero(const Vec2<T>& v,
                                               T eps = kEpsilonV<T>) noexcept {
  return almostZero(v.x, eps) && almostZero(v.y, eps);
}

template <Floating T>
constexpr Vec2<T> normalizedSafe(const Vec2<T>& v,
                                 T eps = kEpsilonV<T>) noexcept {
  const T m2 = v.magSqrd();
  if (almostZero(m2, eps)) return Vec2<T>::zero();
  const T inv = static_cast<T>(1) / std::sqrt(m2);
  return {v.x * inv, v.y * inv};
}

template <Floating T>
[[nodiscard]] inline constexpr T avoidZero(T v, T eps = kEpsilonV<T>) noexcept {
  if (almostZero(v, eps)) {
    return (v < T{0} ? -eps : eps);
  }

  return v;
}

template <Floating T>
[[nodiscard]] inline constexpr T avoidNegOrZero(T v,
                                                T eps = kEpsilonV<T>) noexcept {
  return std::fabs(avoidZero(v, eps));
}

template <Floating T>
constexpr bool nearlyEqual(T a, T b, T eps = kEpsilonV<T>) noexcept {
  return std::abs(a - b) <= eps;
}

template <Floating T>
constexpr bool nearlyEqual(const Vec2<T>& a, const Vec2<T>& b,
                           T eps = kEpsilonV<T>) noexcept {
  return nearlyEqual(a.x, b.x, eps) && nearlyEqual(a.y, b.y, eps);
}

template <Arithmetic T>
constexpr T modPositive(T v, T d) noexcept {
  T r;

  if constexpr (Floating<T>) {
    r = std::fmod(v, d);
  } else {
    r = v % d;
  }

  if (r < T{0}) r += d;
  return r;
}
}  // namespace rl

#endif  // ENGINE_CORE_VALUE_UTILS_H_