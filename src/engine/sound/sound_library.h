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

#ifndef ENGINE_SOUND_SOUND_LIBRARY_H_
#define ENGINE_SOUND_SOUND_LIBRARY_H_

#include "engine/common.h"
#include "engine/resource/resource_slot.h"
#include "engine/sound/sound.h"
#include "engine/sound/sound_resource.h"

namespace rl {
class SoundLibrary {
 public:
  static SoundLibrary& instance();

  void init();
  void shutdown();

  const SoundResource* load(SoundId id);
  void unload(SoundId id);
  const SoundResource* get(SoundId id) const;
  const SoundBankResource* getBank(SoundBankId id) const;

  const SoundBankResource* loadBank(SoundBankId id);
  void unloadBank(SoundBankId id);

 private:
  ResourceSlots<SoundTag, SoundResource> slots_{};
  ResourceSlots<SoundBankTag, SoundBankResource> bankSlots_{};

  SoundLibrary() = default;
};
}  // namespace rl

#define RL_SOUNDLIB (::rl::SoundLibrary::instance())
#define RL_CSOUNDLIB \
  (static_cast<const ::rl::SoundLibrary&>(::rl::SoundLibrary::instance()))

#endif  // ENGINE_SOUND_SOUND_LIBRARY_H_
