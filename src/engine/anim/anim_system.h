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

#ifndef ENGINE_ANIM_ANIM_SYSTEM_H_
#define ENGINE_ANIM_ANIM_SYSTEM_H_

#include "engine/anim/anim_resource.h"
#include "engine/anim/anim_runtime.h"
#include "engine/anim/anim_type.h"
#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/core/handle.h"
#include "engine/core/param_traversal.h"
#include "engine/physics/physics.h"

namespace rl {
class AnimSystem {
 public:
  static AnimSystem& instance();

  void init();
  void shutdown();

  void tick(const FramePacket& f);

  [[nodiscard]] AnimatorHandle generate(AnimatorDesc desc);
  void destroy(AnimatorHandle h);

  bool play(AnimatorHandle h, AnimTag tag, const PlaybackDesc& desc = {});
  bool play(AnimatorHandle h, AnimTag tag, CardinalDir dir,
            const PlaybackDesc& desc = {});
  bool play(AnimatorHandle h, AnimTag tag, Dir dir,
            const PlaybackDesc& desc = {});
  bool dir(AnimatorHandle h, CardinalDir dir);
  bool dir(AnimatorHandle h, Dir dir);
  void speed(AnimatorHandle h, AnimSpeed speed);
  void colorize(AnimatorHandle h, std::vector<Rgba> colorMods);
  bool playing(AnimatorHandle h);
  bool playing(AnimatorHandle h, AnimTag tag);
  bool flippedX(AnimatorHandle h) const noexcept;
  AnimSetId animSet(AnimatorHandle h) const noexcept;
  usize animIdx(AnimatorHandle h) const noexcept;
  AnimFrame animFrame(AnimatorHandle h) const noexcept;

  bool resolveFrame(AnimatorHandle h, AnimatorFrame& out) const;

  [[nodiscard]] AnimEventListenerId on(AnimatorHandle h, AnimEventFn fn,
                                       AnimKeyTag keyTag = kInvalidAnimKeyTag,
                                       AnimTag tag = kInvalidAnimTag,
                                       void* userData = nullptr,
                                       bool once = false) noexcept;

  void off(AnimatorHandle h, AnimEventListenerId id) noexcept;

  void once(AnimatorHandle h, AnimEventFn fn, AnimTag tag = kInvalidAnimTag,
            AnimKeyTag keyTag = kInvalidAnimKeyTag,
            void* userData = nullptr) noexcept;

 private:
  AnimDuration globalTime_{.0f};

  HandlePool<AnimatorTag> hAnimatorPool_{};

  std::vector<Animator> animators_{};

  static bool stepFinishedAnimOnce(const Animator& a, const AnimResource& anim,
                                   AnimDuration u0, AnimDuration u1);

  AnimSystem() = default;

  bool playInternal(Animator* a, AnimTag tag, const PlaybackDesc& desc);
  void speedInternal(Animator* a, AnimSpeed speed, bool preserveProgress);

  Animator* animator(AnimatorHandle h);
  const Animator* animator(AnimatorHandle h) const;

  void stepAnimatorMode(PTMode mode, Animator& a, const AnimResource& anim,
                        AnimDuration u0, AnimDuration u1,
                        AnimDuration duration);
  void fireAnimEvents(Animator& a, const AnimResource& anim,
                      const PTDiscreteTraversal& tr);
  void fireFirstAnimEvents(Animator& a, const AnimResource& anim);

  void fireAnimEvent(Animator& a, AnimTag tag, const AnimKeyFrame& keyFrame);

  [[nodiscard]] AnimFrame timeIdx(const Animator& a, AnimDuration t);
};
}  // namespace rl

#define RL_ANIMSYS (::rl::AnimSystem::instance())
#define RL_CANIMSYS \
  (static_cast<const ::rl::AnimSystem&>(::rl::AnimSystem::instance()))

#endif  // ENGINE_ANIM_ANIM_SYSTEM_H_
