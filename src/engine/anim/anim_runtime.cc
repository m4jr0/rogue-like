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

// Precompiled. ////////////////////////////////////////////////////////////////
#include "precompiled.h"
////////////////////////////////////////////////////////////////////////////////

// Header. /////////////////////////////////////////////////////////////////////
#include "anim_runtime.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/value_utils.h"
#include "engine/math/geometry.h"
#include "engine/physics/physics_utils.h"

namespace rl {
bool AnimDirState::set(Dir newDir) {
  auto oldDir = dir;
  auto oldBias = bias;
  auto changed = [&]() -> auto { return oldDir != dir || oldBias != bias; };

  if (almostZero(newDir.x)) {
    bias = CardinalDir::Unset;
  } else if (newDir.x > kEpsilonF32) {
    bias = CardinalDir::Right;
  } else {
    bias = CardinalDir::Left;
  }

  if (almostZero(newDir)) return changed();
  auto angle = angleOf(newDir);
  auto animDir = angleToCardinalDir(angle);
  if (animDir == dir) return changed();

  auto d = std::remainder(angle - cardinalDirToAngle(dir), 2.0f * kPiF32);
  auto border = kPiF32 * .25f - hysteresis;
  if (std::fabs(d) > border) dir = animDir;
  return changed();
}
}  // namespace rl