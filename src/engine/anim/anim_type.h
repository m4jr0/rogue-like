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

#ifndef ENGINE_ANIM_ANIM_TYPE_H_
#define ENGINE_ANIM_ANIM_TYPE_H_

#include "engine/core/param_set.h"
#include "engine/physics/physics.h"

namespace rl {
using AnimTag = u32;
constexpr auto kInvalidAnimTag = static_cast<AnimTag>(-1);

using AnimKey = u64;
constexpr auto kInvalidAnimKey = static_cast<AnimKey>(-1);

constexpr auto kDefaultAnimDir = CardinalDir::Right;
constexpr auto kDefaultAnimTag = 0;

using AnimFrame = u32;
constexpr auto kInvalidAnimFrame = static_cast<AnimFrame>(-1);

using AnimDuration = f32;  // Seconds.
using AnimProgress = f32;  // 0..1.
using AnimSpeed = f32;

using AnimFlags = u32;

enum AnimFlagBits : AnimFlags {
  kAnimFlagBitsNone = 0x0,
  kAnimFlagBitsLoop = 0x1,
  kAnimFlagBitsPingPong = 0x2,
  kAnimFlagBitsAll = static_cast<AnimFlags>(-1),
};

struct AnimSetTag {};

using AnimKeyTag = u32;
constexpr auto kInvalidAnimKeyTag = static_cast<AnimKeyTag>(-1);
constexpr AnimKeyTag kAnimKeyTagCustomOffset = 1000;

enum BaseAnimKeyTag : AnimKeyTag {
  kBaseAnimKeyTagEnd,
  kBaseAnimKeyTagCustomOffset = kAnimKeyTagCustomOffset,
};

using AnimParamSet = ParamSet<3>;

struct AnimKeyFrame : AnimParamSet {
  AnimKeyTag keyTag{kInvalidAnimKeyTag};
  AnimFrame frame{kInvalidAnimFrame};

  AnimKeyFrame() = default;

  template <typename... Ps>
  AnimKeyFrame(AnimKeyTag k, AnimFrame f, Ps&&... ps)
      : AnimParamSet{std::forward<Ps>(ps)...}, keyTag{k}, frame{f} {
    if (f == 1) {
      int a = 0;
      ++a;
    }
  }
};

enum class AnimFlipPolicy : u8 { Unknown = 0, Keep, Flip, DontFlip };

struct SolvedAnim {
  AnimFlipPolicy flipPolicy{AnimFlipPolicy::Unknown};
  AnimKey key{kInvalidAnimKey};
};

struct AnimSolver {
  AnimSolver() = default;
  AnimSolver(std::unordered_map<AnimKey, SolvedAnim> r) : map{std::move(r)} {}

  SolvedAnim solve(AnimKey key) const {
    if (auto it = map.find(key); it != map.cend()) {
      return it->second;
    } else {
      return {
          .flipPolicy = AnimFlipPolicy::DontFlip,
          .key = key,
      };
    }
  }

 private:
  std::unordered_map<AnimKey, SolvedAnim> map{};
};

}  // namespace rl

#endif  // ENGINE_ANIM_ANIM_TYPE_H_
