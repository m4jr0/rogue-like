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

#ifndef ENGINE_ANIM_ANIM_UTILS_H_
#define ENGINE_ANIM_ANIM_UTILS_H_

#include "engine/anim/anim_type.h"
#include "engine/common.h"
#include "engine/physics/physics.h"

namespace rl {
[[nodiscard]] inline constexpr AnimKey animKey(AnimTag tag,
                                               CardinalDir dir) noexcept {
  return static_cast<AnimKey>(tag) << 32 | static_cast<AnimKey>(dir);
}

[[nodiscard]] inline constexpr AnimTag animTag(AnimKey key) noexcept {
  return static_cast<AnimTag>(key >> 32);
}

[[nodiscard]] inline constexpr CardinalDir animDir(AnimKey key) noexcept {
  return static_cast<CardinalDir>(key);
}

inline constexpr void animTagDir(AnimKey key, AnimTag& tag,
                                 CardinalDir& dir) noexcept {
  tag = animTag(key);
  dir = animDir(key);
}

f32 repeat(f32 t, AnimDuration duration);
f32 oneshot(f32 t, AnimDuration duration);
f32 pingPongRepeat(f32 t, AnimDuration duration);
f32 pingPongOnce(f32 t, AnimDuration duration);
}  // namespace rl

#endif  // ENGINE_ANIM_ANIM_UTILS_H_
