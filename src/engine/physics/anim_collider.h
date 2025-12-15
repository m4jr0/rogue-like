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

#ifndef ENGINE_PHYSICS_ANIM_COLLIDER_H_
#define ENGINE_PHYSICS_ANIM_COLLIDER_H_

#include "engine/common.h"
#include "engine/physics/collider.h"

namespace rl {
struct AnimColliderSample {
  Collider collider{};
};

struct AnimColliderTrack {
  std::vector<AnimColliderSample> samples{};
};

struct AnimColliderSet {
  std::vector<AnimColliderTrack> hurts{};
  std::vector<AnimColliderTrack> hits{};
};

const AnimColliderSample* sampleTrack(const AnimColliderTrack& track, usize i);
}  // namespace rl

#endif  // ENGINE_PHYSICS_ANIM_COLLIDER_H_
