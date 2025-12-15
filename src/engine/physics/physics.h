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

#ifndef ENGINE_PHYSICS_PHYSICS_H_
#define ENGINE_PHYSICS_PHYSICS_H_

#include "engine/common.h"
#include "engine/math/vec2.h"
#include "engine/transform/transform.h"

namespace rl {
using Dir = Vec2<Distance>;
using Steering = Vec2<Distance>;

using Velocity = Vec2<Distance>;
using Acceleration = Vec2<Distance>;

enum class CardinalDir : u8 { Unset = 0, Front, Left, Right, Back };

using SteeringMask = u8;

enum SteeringMaskBits : SteeringMask {
  kSteeringMaskBitsNone = 0x0,
  kSteeringMaskBitsUp = 0x1,
  kSteeringMaskBitsLeft = 0x2,
  kSteeringMaskBitsRight = 0x4,
  kSteeringMaskBitsDown = 0x8,
  kSteeringMaskBitsAll = 0xff
};
}  // namespace rl

#endif  // ENGINE_PHYSICS_PHYSICS_H_
