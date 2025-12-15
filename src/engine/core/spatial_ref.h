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

#ifndef ENGINE_CORE_SPATIAL_REF_H_
#define ENGINE_CORE_SPATIAL_REF_H_

#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/transform/transform.h"

namespace rl {
enum class SpatialRefType : u8 { None = 0, Position, Transform };

struct SpatialRef {
  SpatialRefType type{SpatialRefType::None};
  TransformHandle trans{kInvalidHandle};
  Position pos{Position::zero()};

  static constexpr SpatialRef fromWorld(Position p) {
    return {.type = SpatialRefType::Position, .pos = p};
  }

  static constexpr SpatialRef fromSticky(TransformHandle t) {
    return {.type = SpatialRefType::Transform, .trans = t};
  }

  constexpr bool none() const { return type == SpatialRefType::None; }
  constexpr bool valid() const { return fixed() || sticky(); }

  constexpr bool fixed() const { return type == SpatialRefType::Position; }
  constexpr bool sticky() const {
    return type == SpatialRefType::Transform && trans;
  }

  constexpr bool sameTrans(const SpatialRef& other) {
    return type == SpatialRefType::Transform &&
           other.type == SpatialRefType::Transform && trans == other.trans;
  }
};
}  // namespace rl

#endif  // ENGINE_CORE_SPATIAL_REF_H_
