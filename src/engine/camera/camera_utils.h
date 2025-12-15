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

#ifndef ENGINE_CAMERA_CAMERA_UTILS_H_
#define ENGINE_CAMERA_CAMERA_UTILS_H_

#include "engine/common.h"
#include "engine/math/mat.h"

namespace rl {

[[nodiscard]] constexpr Mat4F32 ortho(f32 left, f32 right, f32 bottom, f32 top,
                                      f32 zNear = -1.0f,
                                      f32 zFar = 1.0f) noexcept {
  Mat4F32 r{};
  auto rl = right - left;
  auto tb = top - bottom;
  auto fn = zFar - zNear;
  r[0] = 2.0f / rl;
  r[5] = 2.0f / tb;
  r[10] = -2.0f / fn;
  r[12] = -(right + left) / rl;
  r[13] = -(top + bottom) / tb;
  r[14] = -(zFar + zNear) / fn;
  r[15] = 1.0f;
  return r;
}
}  // namespace rl

#endif  // ENGINE_CAMERA_CAMERA_UTILS_H_
