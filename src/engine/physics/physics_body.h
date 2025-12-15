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

#ifndef ENGINE_PHYSICS_PHYSICS_BODY_H_
#define ENGINE_PHYSICS_PHYSICS_BODY_H_

#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/math/vec2.h"
#include "engine/physics/collider.h"
#include "engine/physics/physics.h"
#include "engine/transform/transform.h"

namespace rl {
struct PhysicsBodyDesc {
  bool dynamic{false};

  Velocity initVel{};
  Velocity maxVel{100.0f, 100.0f};
  Acceleration acc{60.0f, 60.0f};
  Acceleration dec{50.0f, 50.0f};

  Collider collider{};

  TransformHandle trans{kInvalidHandle};
};

struct PhysicsBodyTag {};
using PhysicsBodyHandle = Handle<PhysicsBodyTag>;

struct PhysicsBody {
  bool dynamic{false};

  Velocity vel{};

  Velocity maxVel{2000.0f, 2000.0f};
  Acceleration acc{800.0f, 800.0f};
  Acceleration dec{1200.0f, 1200.0f};

  f32 tauAccel = .08f;
  f32 tauBrake = .04f;
  f32 tauStopBoost = .5f;

  Dir dir{};
  CardinalDir cardDir{};
  Steering steer{};

  Velocity extVel{Velocity::zero()};
  Velocity extDec{60.0f, 60.0f};

  Collider collider{};
  Collider wCollider{};

  PhysicsBodyHandle handle{kInvalidHandle};
  TransformHandle trans{kInvalidHandle};
};
}  // namespace rl

#endif  // ENGINE_PHYSICS_PHYSICS_BODY_H_
