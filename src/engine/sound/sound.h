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

#ifndef ENGINE_SOUND_SOUND_H_
#define ENGINE_SOUND_SOUND_H_

#include "engine/common.h"

namespace rl {
struct SoundTag {};

struct DecodedSound {
  u32 sampleRate{0};
  u32 channelCount{0};
  std::vector<f32> samples{};
};

struct SoundBankTag {};

enum SoundBus : u8 {
  kSoundBusMaster = 0,
  kSoundBusSfx,
  kSoundBusBGM,
  kSoundBusCustom0,
  kSoundBusCustom1,
  kSoundBusCustom2,
  kSoundBusCustom3,
  kSoundBusCustom4,
  kSoundBusCustom5,
  kSoundBusCustom6,
  kSoundBusCustom7,
  kSoundBusCustom8,
  kSoundBusCustom9,
  kSoundBusCount,
};
}  // namespace rl

#endif  // ENGINE_SOUND_SOUND_H_
