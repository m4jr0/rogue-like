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

#ifndef ENGINE_ANIM_ANIM_RESOURCE_H_
#define ENGINE_ANIM_ANIM_RESOURCE_H_

#include "engine/anim/anim_type.h"
#include "engine/common.h"
#include "engine/physics/collider_resource.h"
#include "engine/resource/resource_type.h"
#include "engine/sprite/sprite.h"
#include "engine/texture/texture_resource.h"

namespace rl {
using AnimSetId = ResourceId<AnimSetTag>;

struct AnimSampleResource {
  Sprite sprite{};
  AnimDuration duration{.0f};
};

struct AnimResource {
  AnimFlags flags{kAnimFlagBitsNone};
  AnimDuration duration{.0f};
  AnimKey key{kInvalidAnimKey};
  std::vector<AnimSampleResource> samples{};
  std::vector<AnimDuration> accDurations{};
  std::vector<AnimKeyFrame> keys{};
};

struct AnimSolvedEntry {
  AnimKey fromKey{kInvalidAnimKey};
  AnimKey toKey{kInvalidAnimKey};
  AnimFlipPolicy flipPolicy{AnimFlipPolicy::Unknown};
};

struct AnimSetResource {
  inline constexpr static ResourceVersion kVersion{1};

  AnimSetId id{kInvalidResourceId};
  TextureId tex{kInvalidResourceId};
  AnimColliderProfileId colliderProfile{kInvalidResourceId};
  usize defaultAnimIdx{0};
  std::unordered_map<AnimKey, usize> keyToIdx{};
  std::vector<AnimResource> anims{};
  std::vector<AnimSolvedEntry> solved{};

  AnimSolver solver{};

  bool idx(AnimKey key, usize& idx) const {
    if (auto it = keyToIdx.find(key); it != keyToIdx.cend()) {
      idx = it->second;
      return true;
    } else {
      idx = defaultAnimIdx;
      return false;
    }
  }
};
}  // namespace rl

#endif  // ENGINE_ANIM_ANIM_RESOURCE_H_
