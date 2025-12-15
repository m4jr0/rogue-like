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
#include "sound_system.h"
////////////////////////////////////////////////////////////////////////////////

// External. ///////////////////////////////////////////////////////////////////
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/camera/camera_system.h"
#include "engine/core/vector.h"
#include "engine/math/geometry.h"
#include "engine/sound/sound_library.h"
#include "engine/transform/transform.h"
#include "engine/transform/transform_system.h"

namespace rl {
struct SoundSystem::SoundDeviceState {
  bool shuttingDown{false};
  ma_device device{};
};

SoundSystem& SoundSystem::instance() {
  static SoundSystem inst;
  return inst;
}

void SoundSystem::init() {
  RL_LOG_DEBUG("SoundSystem::init");
  constexpr auto kSoundCapacity = 512;
  hSoundPool_.clear();
  hSoundPool_.reserve(kSoundCapacity);
  sounds_.reserve(kSoundCapacity);
  busVolumes_.fill(1.0f);

  deviceState_ = std::make_unique<SoundDeviceState>();
  deviceState_->shuttingDown = false;

  auto config = ma_device_config_init(ma_device_type_playback);
  config.playback.format = ma_format_f32;
  config.playback.channels = 2;
  config.sampleRate = kSampleRate_;
  config.dataCallback = [](auto*, auto* out, const auto*, auto frameCount) {
    RL_SOUNDSYS.mix(frameCount, static_cast<f32*>(out));
  };

  if (ma_device_init(nullptr, &config, &deviceState_->device) != MA_SUCCESS) {
    RL_LOG_ERR("SoundSystem::init: Failed to init audio device!");
    deviceState_.reset();
    return;
  }

  if (ma_device_start(&deviceState_->device) != MA_SUCCESS) {
    RL_LOG_ERR("SoundSystem::init: Failed to start audio device!");
    ma_device_uninit(&deviceState_->device);
    deviceState_.reset();
    return;
  }
}

void SoundSystem::shutdown() {
  RL_LOG_DEBUG("SoundSystem::shutdown");

  if (deviceState_) {
    deviceState_->shuttingDown = true;
    ma_device_stop(&deviceState_->device);
    ma_device_uninit(&deviceState_->device);
    deviceState_.reset();
  }

  std::lock_guard lock{mutex_};
  hSoundPool_.clear();
  sounds_.clear();
  busVolumes_.fill(.0f);
}

void SoundSystem::tick(const FramePacket&) {
  auto listenerPos = RL_CCAMSYS.worldCenter();
  std::lock_guard lock{mutex_};
  auto masterGain = busVolumes_[kSoundBusMaster];

  for (auto& s : sounds_) {
    if (!hSoundPool_.alive(s.handle) || !s.playing) continue;
    if (!s.positional()) continue;
    applyPositional(listenerPos, masterGain, s);
  }
}

SoundInstanceHandle SoundSystem::generate(const SoundInstanceDesc& desc) {
  std::lock_guard lock{mutex_};
  auto* sound = RL_SOUNDLIB.load(desc.soundId);
  RL_ASSERT(sound, "SoundSystem::generate: Unknown sound resource provided: ",
            desc.soundId, "!");
  if (!sound) return kInvalidHandle;

  auto h = hSoundPool_.generate();
  ensureCapacity(sounds_, h.index);

  sounds_[h.index] = {
      .playing = desc.playing,
      .flags = desc.flags,
      .handle = h,
      .soundId = desc.soundId,
      .cursorFrame = 0,
      .volume = desc.volume,
      .pitch = desc.pitch,
      .leftGain = 1.0f,
      .rightGain = 1.0f,
      .spatialRef = desc.spatialRef,
  };

  return h;
}

void SoundSystem::destroy(SoundInstanceHandle h) {
  std::lock_guard lock{mutex_};
  auto* s = sound(h);
  if (!s) return;
  destroy(*s);
}

void SoundSystem::play(SoundInstanceHandle h) {
  std::lock_guard lock{mutex_};
  auto* s = sound(h);
  if (!s) return;
  s->playing = true;
  s->cursorFrame = 0;
}

void SoundSystem::playOnce(const OnceSound& os) {
  SoundInstanceDesc desc{};
  desc.playing = true;
  desc.flags = kSoundInstanceFlagBitsOnce;

  if (!os.spatialRef.none()) {
    desc.flags |= kSoundInstanceFlagBitsPositional;
  }

  desc.bus = os.bus;
  desc.soundId = os.soundId;
  desc.volume = os.volume;
  desc.pitch = os.pitch;
  desc.spatialRef = os.spatialRef;

  std::ignore = generate(desc);
}

void SoundSystem::stop(SoundInstanceHandle h) {
  std::lock_guard lock{mutex_};
  auto* s = sound(h);
  if (!s) return;
  s->playing = false;
  s->cursorFrame = 0;
}

void SoundSystem::volume(SoundInstanceHandle h, f32 vol) {
  std::lock_guard lock{mutex_};
  auto* s = sound(h);
  if (!s) return;
  s->volume = vol;
}

void SoundSystem::loop(SoundInstanceHandle h, bool loop) {
  std::lock_guard lock{mutex_};
  auto* s = sound(h);
  if (!s) return;

  if (loop) {
    s->flags |= kSoundInstanceFlagBitsLoop;
  } else {
    s->flags &= ~kSoundInstanceFlagBitsLoop;
  }
}

bool SoundSystem::playing(SoundInstanceHandle h) const {
  std::lock_guard lock{mutex_};
  auto* s = sound(h);
  if (!s) return false;
  return s->playing;
}

void SoundSystem::mix(u32 frameCount, f32* out) {
  const u32 sampleCount = frameCount * channelCount_;
  std::fill(out, out + sampleCount, .0f);

  std::lock_guard lock{mutex_};
  if (sounds_.empty()) return;
  auto count = sounds_.size();
  usize i = 0;

  while (i < count && !deviceState_->shuttingDown) {
    auto& s = sounds_[i];

    if (!hSoundPool_.alive(s.handle) || !s.playing || mix(frameCount, s, out)) {
      ++i;
    }
  }
}

bool SoundSystem::mix(u32 frameCount, SoundInstance& s, f32* out) {
  const auto* res = RL_CSOUNDLIB.get(s.soundId);
  if (!res || res->samples.empty()) return false;

  auto resChannelCount = res->channelCount;
  auto totalFrameCount =
      static_cast<u32>(res->samples.size() / std::max<u32>(1, resChannelCount));

  if (totalFrameCount == 0) return false;

  for (u32 f = 0; f < frameCount; ++f) {
    auto srcFrame = static_cast<u32>(s.cursorFrame + f);

    if (srcFrame >= totalFrameCount) {
      if (s.loop()) {
        srcFrame %= totalFrameCount;
      } else {
        s.playing = false;

        if (s.once()) {
          destroy(s);
          return true;
        }

        break;
      }
    }

    f32 l = .0f;
    f32 r = .0f;

    if (resChannelCount == 1) {
      auto samp = res->samples[srcFrame];
      l = samp * s.leftGain;
      r = samp * s.rightGain;
    } else {
      const usize base = static_cast<usize>(srcFrame) * resChannelCount;
      l = res->samples[base + 0] * s.leftGain;
      r = res->samples[base + 1] * s.rightGain;
    }

    auto dst = f * channelCount_;
    out[dst + 0] += l;
    out[dst + 1] += r;
  }

  s.cursorFrame += frameCount;
  return true;
}

void SoundSystem::applyPositional(const Position& listenerPos, f32 masterGain,
                                  SoundInstance& s) {
  const Position* pos = nullptr;

  switch (s.spatialRef.type) {
    using enum SpatialRefType;

    case Position: {
      pos = &s.spatialRef.pos;
      break;
    }

    case Transform: {
      auto trans = s.spatialRef.trans;
      if (!trans) break;
      const auto* t = RL_CTRANSSYS.global(trans);
      if (!t) break;
      pos = &t->pos;
      break;
    }

    case None:
    default: {
      break;
    }
  }

  if (!pos) return;

  auto rel = *pos - listenerPos;
  auto dist = rel.magSqrd();

  f32 atten;

  if (dist <= posMinHearDist_) {
    atten = 1.0f;
  } else if (dist >= posMaxHearDist_) {
    atten = .0f;
  } else {
    auto t = (dist - posMinHearDist_) / (posMaxHearDist_ - posMinHearDist_);
    atten = 1.0f - t;
  }

  auto pan = std::clamp(rel.x / panRange_, -1.0f, 1.0f);

  auto t = (pan + 1.0f) * .5f;
  auto angle = t * (kPiF32 * .5f);

  auto l = std::cos(angle);
  auto r = std::sin(angle);

  auto busGain = busVolumes_[s.bus];
  auto globalGain = masterGain * busGain;

  s.leftGain = s.volume * atten * l * globalGain;
  s.rightGain = s.volume * atten * r * globalGain;
}

void SoundSystem::destroy(SoundInstance& s) {
  RL_SOUNDLIB.unload(s.soundId);
  hSoundPool_.destroy(s.handle);
  s = {};
}

SoundInstance* SoundSystem::sound(SoundInstanceHandle h) {
  RL_ASSERT(h && hSoundPool_.alive(h),
            "SoundSystem::sound: Invalid sound instance handle provided!");
  if (!h || !hSoundPool_.alive(h)) return nullptr;
  return &sounds_[h.index];
}

const SoundInstance* SoundSystem::sound(SoundInstanceHandle h) const {
  RL_ASSERT(h && hSoundPool_.alive(h),
            "SoundSystem::sound: Invalid sound instance handle provided!");
  if (!h || !hSoundPool_.alive(h)) return nullptr;
  return &sounds_[h.index];
}
}  // namespace rl