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

#ifndef ENGINE_SOUND_SOUND_RESOURCE_H_
#define ENGINE_SOUND_SOUND_RESOURCE_H_

#include "engine/common.h"
#include "engine/resource/resource_type.h"
#include "engine/sound/sound.h"

namespace rl {
using SoundId = ResourceId<SoundTag>;

struct SoundResource {
  inline constexpr static ResourceVersion kVersion{1};

  SoundId id{kInvalidResourceId};
  u32 sampleRate{0};
  u32 channelCount{0};
  std::vector<f32> samples{};
};

using SoundBankId = ResourceId<SoundBankTag>;

struct SoundBankResource {
  inline constexpr static ResourceVersion kVersion{1};

  SoundBankId id{kInvalidResourceId};
  std::vector<SoundId> sounds{};
};
}  // namespace rl

#endif  // ENGINE_SOUND_SOUND_RESOURCE_H_
