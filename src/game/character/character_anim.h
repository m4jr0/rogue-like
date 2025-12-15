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

#ifndef GAME_CHARACTER_CHARACTER_ANIM_H_
#define GAME_CHARACTER_CHARACTER_ANIM_H_

#include "engine/anim/anim_resource.h"
#include "engine/anim/anim_runtime.h"
#include "engine/anim/anim_system.h"
#include "engine/anim/anim_type.h"
#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/core/type_trait.h"

namespace rl {
enum CharAnimKeyTag : AnimKeyTag {
  kCharAnimKeyTagDefaultHurtboxSet = kBaseAnimKeyTagCustomOffset,
  kCharAnimKeyTagCustomHurtboxSet,
  kCharAnimKeyTagHitWalk,
};

enum class CharDir : u8 { Down, Left, Up, Right, Count };

using CharAnimFlags = u32;

enum CharAnimFlagBits : CharAnimFlags {
  kCharAnimFlagBitsNone = 0x0,
  kCharAnimFlagBitsFlipped = 0x1,
  kCharAnimFlagBitsAll = static_cast<CharAnimFlags>(-1),
};

struct CharAnim {
  CharAnimFlags flags{kCharAnimFlagBitsNone};
  std::string anim{};
};

struct CharAnimSet {
  AnimSetId set{kInvalidResourceId};
  std::array<CharAnim, toUnderlying(CharDir::Count) - 1> dirToAnim;

  const CharAnim& anim(CharDir d) {
    static CharAnim kDefault{};
    auto cd = toUnderlying(d);
    if (cd >= toUnderlying(CharDir::Count)) return kDefault;
    return dirToAnim[cd];
  }
};

struct CharAnims {
  AnimatorHandle animator{kInvalidHandle};
  std::vector<AnimEventListenerId> listenerIds{};

  void play(AnimTag tag, const PlaybackDesc& desc = {}) {
    if (!animator) return;
    RL_ANIMSYS.play(animator, tag, desc);
  }

  bool playing() {
    if (!animator) return false;
    return RL_ANIMSYS.playing(animator);
  }

  bool playing(AnimTag tag) {
    if (!animator) return false;
    return RL_ANIMSYS.playing(animator, tag);
  }

  void speed(AnimSpeed speed) {
    if (!animator) return;
    RL_ANIMSYS.speed(animator, speed);
  }

  bool flippedX() {
    if (!animator) return false;
    return RL_CANIMSYS.flippedX(animator);
  }

  AnimEventListenerId on(AnimEventFn fn, AnimKeyTag keyTag = kInvalidAnimKeyTag,
                         AnimTag tag = kInvalidAnimTag,
                         void* userData = nullptr) {
    return RL_ANIMSYS.on(animator, fn, keyTag, tag, userData);
  }

  void off(AnimEventListenerId id) {
    if (!animator) return;
    RL_ANIMSYS.off(animator, id);
  }

  void once(AnimEventFn fn, AnimTag tag = kInvalidAnimTag,
            AnimKeyTag keyTag = kInvalidAnimKeyTag, void* userData = nullptr) {
    RL_ANIMSYS.once(animator, fn, tag, keyTag, userData);
  }
};
}  // namespace rl

#endif  // GAME_CHARACTER_CHARACTER_ANIM_H_
