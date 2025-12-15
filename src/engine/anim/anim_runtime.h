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

#ifndef ENGINE_ANIM_ANIM_RUNTIME_H_
#define ENGINE_ANIM_ANIM_RUNTIME_H_

#include "engine/anim/anim_resource.h"
#include "engine/anim/anim_type.h"
#include "engine/common.h"
#include "engine/core/color.h"
#include "engine/core/handle.h"
#include "engine/physics/physics.h"
#include "engine/resource/resource_type.h"
#include "engine/sprite/sprite.h"
#include "engine/texture/texture_resource.h"

namespace rl {
struct AnimatorTag {};
using AnimatorHandle = Handle<AnimatorTag>;

struct AnimEvent {
  AnimatorHandle animator{kInvalidHandle};
  AnimTag tag{kInvalidAnimTag};
  const AnimKeyFrame& key;
};

using AnimEventFn = void (*)(const AnimEvent&, void*);

using AnimEventListenerId = u32;

constexpr auto kInvalidAnimEventListenerId =
    static_cast<AnimEventListenerId>(-1);

struct AnimEventListener {
  bool once{false};
  AnimEventListenerId id{kInvalidAnimEventListenerId};
  AnimTag tag{kInvalidAnimTag};
  AnimKeyTag keyTag{kInvalidAnimKeyTag};
  AnimEventFn fn{nullptr};
  void* userData{nullptr};
};

struct AnimState {
  AnimFlags flags{kAnimFlagBitsNone};
  AnimTag tag{kInvalidAnimTag};
  usize idx{0};

  constexpr bool loop() const noexcept {
    return (flags & kAnimFlagBitsLoop) != 0;
  }

  constexpr bool pingPong() const noexcept {
    return (flags & kAnimFlagBitsPingPong) != 0;
  }
};

struct AnimDirState {
  CardinalDir dir{CardinalDir::Unset};
  CardinalDir bias{CardinalDir::Unset};
  f32 hysteresis = .2f;

  bool set(CardinalDir newDir) {
    if (newDir == dir && bias == CardinalDir::Unset) return false;
    dir = newDir;
    bias = CardinalDir::Unset;
    return true;
  }

  bool set(Dir newDir);
};

using PlaybackFlags = u16;

enum PlaybackFlagBits : PlaybackFlags {
  kPlaybackFlagBitsNone = 0x0,
  kPlaybackFlagBitsRestart = 0x1,
  kPlaybackFlagBitsUseSpeedParam = 0x2,
  kPlaybackFlagBitsAll = static_cast<PlaybackFlagBits>(-1),
};

struct PlaybackDesc {
  AnimSpeed speed{1.0f};
  PlaybackFlags flags{kPlaybackFlagBitsNone};
};

struct AnimatorDesc {
  PlaybackDesc playback{};
  AnimTag anim{kDefaultAnimTag};
  CardinalDir dir{kDefaultAnimDir};
  AnimSetId animSet{kInvalidResourceId};
  std::vector<Rgba> colorMods{};
};

using AnimatorStateFlags = u8;

enum AnimatorStateFlagBits : AnimatorStateFlags {
  kAnimatorStateFlagBitsNone = 0x0,
  kAnimatorStateFlagBitsFinished = 0x1,
  kAnimatorStateFlagBitsSentEndEvent = 0x2,
  kAnimatorStateFlagBitsFlipped = 0x4,
  kAnimatorStateFlagBitsAll = static_cast<AnimatorStateFlags>(-1),
};

struct Animator {
  AnimatorStateFlags stateFlags{kAnimatorStateFlagBitsNone};
  AnimState animState{};
  AnimDirState dirState{};
  AnimatorHandle handle{kInvalidHandle};
  AnimSetId animSet{kInvalidResourceId};
  Rgba colorMod{kRgbaWhite};
  AnimDuration startTime{.0f};
  AnimSpeed speed{1.0f};
  AnimDuration uTime{.0f};
  f32 progress{.0f};
  AnimFrame currentFrame{kInvalidAnimFrame};
  AnimEventListenerId listenerIdCounter{0};
  std::vector<Rgba> colorMods{kRgbaWhite};
  std::vector<AnimEventListener> listeners{};

  constexpr bool finished() const noexcept {
    return stateFlags & kAnimatorStateFlagBitsFinished;
  }

  constexpr bool sentEndEvent() const noexcept {
    return stateFlags & kAnimatorStateFlagBitsSentEndEvent;
  }

  constexpr bool flipped() const noexcept {
    return stateFlags & kAnimatorStateFlagBitsFlipped;
  }
};

struct AnimatorFrame {
  bool flipped{false};
  TextureId tex{kInvalidResourceId};
  Sprite sprite{};
  Rgba colorMod{kRgbaWhite};
};
}  // namespace rl

#endif  // ENGINE_ANIM_ANIM_RUNTIME_H_
