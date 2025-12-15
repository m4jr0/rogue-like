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

#ifndef ENGINE_MATH_MAT_H_
#define ENGINE_MATH_MAT_H_

#include "engine/common.h"

namespace rl {
template <typename T>
struct Mat4 {
  constexpr Mat4() = default;

  constexpr Mat4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20,
                 T m21, T m22, T m23, T m30, T m31, T m32, T m33) noexcept
      : m{m00, m01, m02, m03, m10, m11, m12, m13,
          m20, m21, m22, m23, m30, m31, m32, m33} {}

  constexpr T& at(usize r, usize c) & noexcept { return m[c * 4 + r]; }
  constexpr const T& at(usize r, usize c) const& noexcept {
    return m[c * 4 + r];
  }

  constexpr T at(usize r, usize c) && noexcept { return m[c * 4 + r]; }

  constexpr T& operator()(usize r, usize c) & noexcept { return at(r, c); }

  constexpr const T& operator()(usize r, usize c) const& noexcept {
    return at(r, c);
  }

  constexpr T operator()(usize r, usize c) && noexcept { return at(r, c); }

  T m[16]{};

  constexpr T& operator[](usize index) noexcept { return m[index]; }
  constexpr const T& operator[](usize index) const noexcept { return m[index]; }

  static constexpr Mat4 identity() {
    Mat4 r{};
    r.m[0] = r.m[5] = r.m[10] = r.m[15] = T{1};
    return r;
  }

  constexpr const T* data() const noexcept { return m; }
  constexpr T* data() noexcept { return m; }

  [[nodiscard]] friend constexpr Mat4 operator*(const Mat4& lhs,
                                                const Mat4& rhs) noexcept {
    return Mat4::mul(lhs, rhs);
  }

  constexpr Mat4& operator*=(const Mat4& rhs) noexcept {
    *this = Mat4::mul(*this, rhs);
    return *this;
  }

  [[nodiscard]] static constexpr Mat4 mul(const Mat4& A,
                                          const Mat4& B) noexcept {
    Mat4 m{};

    for (int c = 0; c < 4; ++c) {
      for (int r = 0; r < 4; ++r) {
        m.m[c * 4 + r] =
            A.m[0 * 4 + r] * B.m[c * 4 + 0] + A.m[1 * 4 + r] * B.m[c * 4 + 1] +
            A.m[2 * 4 + r] * B.m[c * 4 + 2] + A.m[3 * 4 + r] * B.m[c * 4 + 3];
      }
    }

    return m;
  }
};

using Mat4S8 = Mat4<s8>;
using Mat4S16 = Mat4<s16>;
using Mat4S32 = Mat4<s32>;
using Mat4S64 = Mat4<s64>;
using Mat4F32 = Mat4<f32>;
using Mat4F64 = Mat4<f64>;
}  // namespace rl

#endif  // ENGINE_MATH_MAT_H_
