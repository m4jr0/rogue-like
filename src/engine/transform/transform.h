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

#ifndef ENGINE_TRANSFORM_TRANSFORM_H_
#define ENGINE_TRANSFORM_TRANSFORM_H_

#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/math/trs.h"
#include "engine/math/vec2.h"

namespace rl {
using Distance = f32;
using Position = Vec2<Distance>;

using Rotation = f32;

using Scale = Vec2F32;
using Pivot = Vec2F32;

struct TransformDesc {
  Position pos{Position::zero()};
  Rotation rot{.0f};
  Scale scale{1.0f, 1.0f};
};

struct TransformTag {};
using TransformHandle = Handle<TransformTag>;

struct Transform {
  bool dirty{false};
  TransformHandle handle{kInvalidHandle};
  Position pos{Position::zero()};
  Rotation rot{.0f};
  Scale scale{1.0f, 1.0f};
};

struct GlobalTransform {
  TransformHandle handle{kInvalidHandle};
  Position pos{Position::zero()};
  Rotation rot{.0f};
  Scale scale{1.0f, 1.0f};
  Trs trs{Trs::identity()};
  Trs prevTrs{Trs::identity()};
};

struct TransformHierarchy {
  TransformHandle parent{kInvalidHandle};
  TransformHandle firstChild{kInvalidHandle};
  TransformHandle nextSibling{kInvalidHandle};
  TransformHandle prevSibling{kInvalidHandle};
};
}  // namespace rl

#endif  // ENGINE_TRANSFORM_TRANSFORM_H_
