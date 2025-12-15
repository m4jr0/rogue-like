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
#include "anim_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/anim/anim_library.h"
#include "engine/anim/anim_utils.h"
#include "engine/core/color.h"
#include "engine/core/param_traversal.h"
#include "engine/core/value_utils.h"
#include "engine/core/vector.h"

namespace rl {
AnimSystem& AnimSystem::instance() {
  static AnimSystem inst;
  return inst;
}

void AnimSystem::init() {
  RL_LOG_DEBUG("AnimSystem::init");
  constexpr auto kAnimatorCapacity = 64;
  hAnimatorPool_.clear();
  hAnimatorPool_.reserve(kAnimatorCapacity);
  animators_.reserve(kAnimatorCapacity);
}

void AnimSystem::shutdown() {
  RL_LOG_DEBUG("AnimSystem::shutdown");
  hAnimatorPool_.clear();
  animators_.clear();
}

void AnimSystem::tick(const FramePacket& f) {
  globalTime_ += static_cast<AnimDuration>(f.step);
  if (animators_.empty()) return;

  for (auto& a : animators_) {
    if (!hAnimatorPool_.alive(a.handle)) continue;
    if (a.finished()) continue;
    const auto* set = RL_ANIMLIB.get(a.animSet);
    const auto& state = a.animState;
    const auto& anim = set->anims[state.idx];
    auto duration = anim.duration;

    if (anim.samples.empty() || duration <= .0f) {
      a.currentFrame = 0;
      continue;
    }

    auto u0 = a.uTime;
    auto u1 = std::max(.0f, (globalTime_ - a.startTime) * a.speed);
    a.uTime = u1;

    auto mode = PTMode::None;

    if (state.pingPong()) {
      mode = PTMode::PingPong;
    } else if (state.loop()) {
      mode = PTMode::Loop;
    }

    stepAnimatorMode(mode, a, anim, u0, u1, duration);
  }
}

void AnimSystem::fireAnimEvents(Animator& a, const AnimResource& anim,
                                const PTDiscreteTraversal& tr) {
  auto tag = animTag(anim.key);

  for (const auto& k : anim.keys) {
    auto u = static_cast<PTIndex>(k.frame);
    if (tr.contains(u)) fireAnimEvent(a, tag, k);
  }
}

void AnimSystem::fireFirstAnimEvents(Animator& a, const AnimResource& anim) {
  auto tag = animTag(anim.key);

  for (const auto& k : anim.keys) {
    if (k.frame == 0) {
      fireAnimEvent(a, tag, k);
    }
  }
}

void AnimSystem::fireAnimEvent(Animator& a, AnimTag tag,
                               const AnimKeyFrame& keyFrame) {
  if (a.listeners.empty()) return;

  AnimEvent e{
      .animator = a.handle,
      .tag = tag,
      .key = keyFrame,
  };

  usize i = 0;

  while (i < a.listeners.size()) {
    const auto& l = a.listeners[i];

    if (!l.fn) {
      ++i;
      continue;
    }

    if ((l.tag != kInvalidAnimTag && l.tag != tag) ||
        (l.keyTag != kInvalidAnimKeyTag && l.keyTag != keyFrame.keyTag)) {
      ++i;
      continue;
    }

    auto id = l.id;
    l.fn(e, l.userData);
    auto alive = (i < a.listeners.size()) && (a.listeners[i].id == id);

    if (!alive) {
      continue;
    }

    // Case: maybe a.listeners got resized (adding a listener inside another
    // listener), so we need a new alias to make it safe.
    const auto& postFnL = a.listeners[i];

    if (postFnL.once) {
      off(a.handle, postFnL.id);
    } else {
      ++i;
    }
  }
}

AnimatorHandle AnimSystem::generate(AnimatorDesc desc) {
  auto* set = RL_ANIMLIB.load(desc.animSet);
  RL_ASSERT(set,
            "AnimSystem::generate: Unknown animation set resource provided: ",
            desc.animSet, "!");
  if (!set) return kInvalidHandle;

  auto h = hAnimatorPool_.generate();
  ensureCapacity(animators_, h.index);
  auto& a = animators_[h.index];
  a.handle = h;
  a.animSet = desc.animSet;
  play(a.handle, desc.anim, desc.dir);

  if (!desc.colorMods.empty()) {
    colorize(a.handle, std::move(desc.colorMods));
  }

  return h;
}

void AnimSystem::destroy(AnimatorHandle h) {
  auto* a = animator(h);
  if (!a) return;
  RL_ANIMLIB.unload(a->animSet);
  *a = {};
  hAnimatorPool_.destroy(h);
}

bool AnimSystem::play(AnimatorHandle h, AnimTag tag, const PlaybackDesc& desc) {
  auto* a = animator(h);
  if (!a) return false;
  return playInternal(a, tag, desc);
}

bool AnimSystem::play(AnimatorHandle h, AnimTag tag, CardinalDir dir,
                      const PlaybackDesc& desc) {
  auto* a = animator(h);
  if (!a) return false;
  if (!a->dirState.set(dir)) return true;
  return playInternal(a, tag, desc);
}

bool AnimSystem::play(AnimatorHandle h, AnimTag tag, Dir dir,
                      const PlaybackDesc& desc) {
  auto* a = animator(h);
  if (!a) return false;
  if (!a->dirState.set(dir)) return true;
  return playInternal(a, tag, desc);
}

bool AnimSystem::dir(AnimatorHandle h, CardinalDir dir) {
  auto* a = animator(h);
  if (!a) return false;
  if (!a->dirState.set(dir)) return true;
  return playInternal(a, a->animState.tag, {});
}

bool AnimSystem::dir(AnimatorHandle h, Dir dir) {
  auto* a = animator(h);
  if (!a) return false;
  if (!a->dirState.set(dir)) return true;
  return playInternal(a, a->animState.tag, {});
}

void AnimSystem::speed(AnimatorHandle h, AnimSpeed speed) {
  auto* a = animator(h);
  if (!a) return;
  speedInternal(a, speed, true);
}

void AnimSystem::colorize(AnimatorHandle h, std::vector<Rgba> colorMods) {
  auto* a = animator(h);
  if (!a) return;
  a->colorMods = std::move(colorMods);
}

bool AnimSystem::playing(AnimatorHandle h) {
  auto* a = animator(h);
  if (!a) return false;
  return !a->finished();
}

bool AnimSystem::playing(AnimatorHandle h, AnimTag tag) {
  auto* a = animator(h);
  if (!a) return false;
  return !a->finished() && a->animState.tag == tag;
}

bool AnimSystem::flippedX(AnimatorHandle h) const noexcept {
  const auto* a = animator(h);
  return a && a->flipped();
}

AnimSetId AnimSystem::animSet(AnimatorHandle h) const noexcept {
  const auto* a = animator(h);
  return a ? a->animSet : kInvalidResourceId;
}

usize AnimSystem::animIdx(AnimatorHandle h) const noexcept {
  const auto* a = animator(h);
  return a ? a->animState.idx : kInvalidIndex;
}

AnimFrame AnimSystem::animFrame(AnimatorHandle h) const noexcept {
  const auto* a = animator(h);
  return a ? a->currentFrame : kInvalidAnimFrame;
}

bool AnimSystem::resolveFrame(AnimatorHandle h, AnimatorFrame& out) const {
  const auto* a = animator(h);

  if (!a) {
    out = {};
    return false;
  }

  const auto* set = RL_ANIMLIB.get(a->animSet);
  auto animIdx = a->animState.idx;

  if (!set || animIdx >= set->anims.size()) {
    out = {};
    return false;
  }

  const auto& anim = set->anims[animIdx];

  if (anim.samples.empty()) {
    out = {};
    return false;
  }

  auto idx = std::min<usize>(a->currentFrame, anim.samples.size() - 1);
  const auto& sample = anim.samples[idx];

  out = {
      .flipped = a->flipped(),
      .tex = set->tex,
      .sprite = sample.sprite,
      .colorMod = a->colorMod,
  };

  return true;
}

AnimEventListenerId AnimSystem::on(AnimatorHandle h, AnimEventFn fn,
                                   AnimKeyTag keyTag, AnimTag tag,
                                   void* userData, bool once) noexcept {
  RL_ASSERT(fn, "AnimSystem::on: Function provided is null!");
  auto* a = animator(h);
  auto id = a->listenerIdCounter++;

  a->listeners.push_back({.once = once,
                          .id = id,
                          .tag = tag,
                          .keyTag = keyTag,
                          .fn = fn,
                          .userData = userData});
  return id;
}

void AnimSystem::off(AnimatorHandle h, AnimEventListenerId id) noexcept {
  RL_ASSERT(id != kInvalidAnimEventListenerId,
            "AnimSystem::off: Listener ID provided is invalid!");
  auto* a = animator(h);
  auto& l = a->listeners;

  for (usize i = 0; i < l.size(); ++i) {
    if (l[i].id == id) {
      l[i] = l.back();
      l.pop_back();
      return;
    }
  }
}

void AnimSystem::once(AnimatorHandle h, AnimEventFn fn, AnimTag tag,
                      AnimKeyTag keyTag, void* userData) noexcept {
  std::ignore = on(h, fn, tag, keyTag, userData, true);
}

bool AnimSystem::stepFinishedAnimOnce(const Animator& a,
                                      const AnimResource& anim, AnimDuration u0,
                                      AnimDuration u1) {
  if (anim.duration <= .0f) return true;
  if (a.animState.loop()) return false;

  auto period = anim.duration;
  if (a.animState.pingPong()) period *= 2.0f;
  return (u0 < period && u1 >= period);
}

bool AnimSystem::playInternal(Animator* a, AnimTag tag,
                              const PlaybackDesc& desc) {
  if (!a) return false;
  const auto* set = RL_ANIMLIB.get(a->animSet);
  if (!set) return false;
  auto solved = set->solver.solve(animKey(tag, a->dirState.dir));
  usize idx;
  auto found = set->idx(solved.key, idx);
  RL_ASSERT(idx != kInvalidIndex,
            "AnimSystem::playInternal: Invalid index found in animation set!");
  const auto& anim = set->anims[idx];
  auto flags = found ? anim.flags : kAnimFlagBitsNone;

  if (solved.flipPolicy == AnimFlipPolicy::Flip) {
    a->stateFlags |= kAnimatorStateFlagBitsFlipped;
  } else if (solved.flipPolicy == AnimFlipPolicy::DontFlip) {
    a->stateFlags &= ~kAnimatorStateFlagBitsFlipped;
  } else if (solved.flipPolicy == AnimFlipPolicy::Keep) {
    if (a->dirState.bias != CardinalDir::Unset) {
      if (a->dirState.bias != animDir(anim.key)) {
        a->stateFlags |= kAnimatorStateFlagBitsFlipped;
      } else {
        a->stateFlags &= ~kAnimatorStateFlagBitsFlipped;
      }
    }
  }

  auto tagChanged = (tag != a->animState.tag);
  auto restart = (desc.flags & kPlaybackFlagBitsRestart) || tagChanged;

  if (desc.flags & kPlaybackFlagBitsUseSpeedParam) {
    speedInternal(a, desc.speed, !restart);
  }

  a->animState = {.flags = flags, .tag = tag, .idx = idx};

  if (restart) {
    a->currentFrame = kInvalidAnimFrame;
    a->startTime = globalTime_;
    a->uTime = .0f;
    a->stateFlags &=
        ~(kAnimatorStateFlagBitsSentEndEvent | kAnimatorStateFlagBitsFinished);
  }

  return found;
}

void AnimSystem::speedInternal(Animator* a, AnimSpeed speed,
                               bool preserveProgress) {
  if (!a) return;
  speed = avoidNegOrZero(speed);
  auto oldSpeed = avoidNegOrZero(a->speed);

  if (preserveProgress) {
    auto elapsed = std::max(.0f, (globalTime_ - a->startTime) * oldSpeed);
    a->startTime = globalTime_ - (elapsed / speed);
  } else {
    a->startTime = globalTime_;
    a->currentFrame = 0;
  }

  a->speed = speed;
}

Animator* AnimSystem::animator(AnimatorHandle h) {
  RL_ASSERT(h && hAnimatorPool_.alive(h),
            "AnimSystem::animator: Invalid animator handle provided!");
  if (!h || !hAnimatorPool_.alive(h)) return nullptr;
  return &animators_[h.index];
}

const Animator* AnimSystem::animator(AnimatorHandle h) const {
  RL_ASSERT(h && hAnimatorPool_.alive(h),
            "AnimSystem::animator: Invalid animator handle provided!");
  if (!h || !hAnimatorPool_.alive(h)) return nullptr;
  return &animators_[h.index];
}

void AnimSystem::stepAnimatorMode(PTMode mode, Animator& a,
                                  const AnimResource& anim, AnimDuration u0,
                                  AnimDuration u1, AnimDuration duration) {
  auto folded = fold(mode, u0, u1, duration);
  auto curPos = folded.pos;

  auto firstStep = a.currentFrame == kInvalidAnimFrame;
  auto prevFrame = firstStep ? 0 : a.currentFrame;
  auto curFrame = timeIdx(a, curPos);

  if (curFrame == kInvalidAnimFrame) {
    curFrame = prevFrame;
  }

  a.currentFrame = curFrame;
  a.progress = (duration > .0f) ? (curPos / duration) : .0f;

  if (!anim.keys.empty() && !anim.samples.empty()) {
    auto len = static_cast<PTIndex>(std::min<std::size_t>(
        anim.samples.size(), std::numeric_limits<PTIndex>::max()));

    auto trDisc = discreteTraversal<AnimFrame>(mode, prevFrame, curFrame, len,
                                               folded.seamCrossingCount);

    fireAnimEvents(a, anim, trDisc);

    if (firstStep) {
      fireFirstAnimEvents(a, anim);
    }
  }

  if (stepFinishedAnimOnce(a, anim, u0, u1)) {
    a.stateFlags |= kAnimatorStateFlagBitsFinished;
  }

  if (a.stateFlags & kAnimatorStateFlagBitsFinished) {
    if (!a.sentEndEvent()) {
      fireAnimEvent(a, animTag(anim.key),
                    {
                        static_cast<AnimKeyTag>(kBaseAnimKeyTagEnd),
                        curFrame,
                    });

      a.stateFlags |= kAnimatorStateFlagBitsSentEndEvent;
    }
  } else {
    a.stateFlags &= ~kAnimatorStateFlagBitsSentEndEvent;
  }

  if (!a.colorMods.empty()) {
    a.colorMod = gradientMultiply(a.colorMods, a.progress);
  } else {
    a.colorMod = kRgbaWhite;
  }
}

AnimFrame AnimSystem::timeIdx(const Animator& a, AnimDuration t) {
  const auto* set = RL_ANIMLIB.get(a.animSet);
  if (!set) return kInvalidAnimFrame;
  const auto animIdx = a.animState.idx;
  if (animIdx >= set->anims.size()) return kInvalidAnimFrame;
  const auto& accs = set->anims[animIdx].accDurations;
  if (accs.empty()) return 0;
  auto it = std::upper_bound(accs.begin(), accs.end(), t);
  auto idx = static_cast<AnimFrame>(it - accs.begin());
  return std::min<AnimFrame>(idx, static_cast<AnimFrame>(accs.size() - 1));
}
}  // namespace rl