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

#ifndef ENGINE_MATH_GEOMETRY_H_
#define ENGINE_MATH_GEOMETRY_H_

#include "engine/common.h"
#include "engine/math/vec2.h"

namespace rl {
constexpr f64 kPi = 3.14159265358979323846;
constexpr auto kPiF32 = static_cast<f32>(kPi);

template <typename T, typename A = f32>
inline constexpr A angleOf(Vec2<T> v) {
  return static_cast<A>(std::atan2(v.y, v.x));
}
}  // namespace rl

#endif  // ENGINE_MATH_GEOMETRY_H_
