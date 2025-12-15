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

#ifndef ENGINE_SOUND_SOUND_RUNTIME_H_
#define ENGINE_SOUND_SOUND_RUNTIME_H_

#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/core/spatial_ref.h"
#include "engine/sound/sound.h"
#include "engine/sound/sound_resource.h"
#include "engine/transform/transform.h"

namespace rl {
using SoundInstanceFlags = u8;

enum SoundInstanceFlagBits : SoundInstanceFlags {
  kSoundInstanceFlagBitsNone = 0x0,
  kSoundInstanceFlagBitsLoop = 0x1,
  kSoundInstanceFlagBitsPositional = 0x2,
  kSoundInstanceFlagBitsOnce = 0x4,
  kSoundInstanceFlagBitsAll = static_cast<SoundInstanceFlags>(-1),
};

struct SoundInstanceDesc {
  bool playing{false};
  SoundInstanceFlags flags{kSoundInstanceFlagBitsNone};
  SoundBus bus{kSoundBusMaster};
  SoundId soundId{kInvalidResourceId};
  f32 volume{1.0f};
  f32 pitch{1.0f};
  SpatialRef spatialRef{};
};

struct SoundInstanceTag {};
using SoundInstanceHandle = Handle<SoundInstanceTag>;

struct SoundInstance {
  bool playing{false};
  SoundInstanceFlags flags{kSoundInstanceFlagBitsNone};
  SoundBus bus{kSoundBusMaster};
  SoundInstanceHandle handle{kInvalidHandle};
  SoundId soundId{kInvalidResourceId};
  u32 cursorFrame{0};
  f32 volume{1.0f};
  f32 pitch{1.0f};
  f32 leftGain{1.0f};
  f32 rightGain{1.0f};
  SpatialRef spatialRef{};

  constexpr bool loop() const noexcept {
    return flags & kSoundInstanceFlagBitsLoop;
  }

  constexpr bool positional() const noexcept {
    return flags & kSoundInstanceFlagBitsPositional;
  }

  constexpr bool once() const noexcept {
    return flags & kSoundInstanceFlagBitsOnce;
  }
};

struct OnceSound {
  SoundId soundId{kInvalidResourceId};
  SoundBus bus{kSoundBusSfx};
  f32 volume{1.0f};
  f32 pitch{1.0f};
  SpatialRef spatialRef{};
};
}  // namespace rl

#endif  // ENGINE_SOUND_SOUND_RUNTIME_H_
