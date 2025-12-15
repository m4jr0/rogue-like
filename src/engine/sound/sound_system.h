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

#ifndef ENGINE_SOUND_SOUND_SYSTEM_H_
#define ENGINE_SOUND_SOUND_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/sound/sound.h"
#include "engine/sound/sound_runtime.h"

namespace rl {
class SoundSystem {
 public:
  static SoundSystem& instance();

  void init();
  void shutdown();

  void tick(const FramePacket&);

  [[nodiscard]] SoundInstanceHandle generate(const SoundInstanceDesc& desc);
  void destroy(SoundInstanceHandle h);

  void play(SoundInstanceHandle h);
  void playOnce(const OnceSound& os);
  void stop(SoundInstanceHandle h);
  void volume(SoundInstanceHandle h, f32 vol);
  void loop(SoundInstanceHandle h, bool loop);
  bool playing(SoundInstanceHandle h) const;

  void busVolume(SoundBus bus, f32 volume) {
    if (bus >= kSoundBusCount) return;
    std::lock_guard lock{mutex_};
    busVolumes_[bus] = std::clamp(volume, .0f, 1.0f);
  }

  f32 busVolume(SoundBus bus) const {
    if (bus >= kSoundBusCount) return .0f;
    std::lock_guard lock{mutex_};
    return busVolumes_[bus];
  }

  void masterVolume(f32 volume) { busVolume(kSoundBusMaster, volume); }
  f32 masterVolume() const { return busVolume(kSoundBusMaster); }

  void setBus(SoundInstanceHandle h, SoundBus bus) {
    if (bus >= kSoundBusCount) return;
    std::lock_guard lock{mutex_};
    auto* s = sound(h);
    if (!s) return;
    s->bus = bus;
  }

 private:
  inline static constexpr u32 kSampleRate_{48000};
  struct SoundDeviceState;

  u32 channelCount_{2};
  f32 panRange_{200.0f};
  f32 posMaxHearDist_{400.0f * 400.0f};
  f32 posMinHearDist_{2.0f * 2.0f};

  HandlePool<SoundInstanceTag> hSoundPool_{};
  std::vector<SoundInstance> sounds_{};
  std::array<f32, kSoundBusCount> busVolumes_{};
  std::unordered_map<SoundId, DecodedSound> decodedCache_{};
  std::unique_ptr<SoundDeviceState> deviceState_{nullptr};

  mutable std::mutex mutex_{};

  SoundSystem() = default;

  void mix(u32 frameCount, f32* out);
  bool mix(u32 frameCount, SoundInstance& s, f32* out);
  void applyPositional(const Position& listenerPos, f32 masterGain,
                       SoundInstance& s);
  void destroy(SoundInstance& s);

  SoundInstance* sound(SoundInstanceHandle h);
  const SoundInstance* sound(SoundInstanceHandle h) const;
};
}  // namespace rl

#define RL_SOUNDSYS (::rl::SoundSystem::instance())
#define RL_CSOUNDSYS \
  (static_cast<const ::rl::SoundSystem&>(::rl::SoundSystem::instance()))

#endif  // ENGINE_SOUND_SOUND_SYSTEM_H_
