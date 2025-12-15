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
#include "sound_library.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/log.h"
#include "engine/resource/resource_file.h"
#include "engine/resource/resource_type_registry.h"
#include "engine/sound/sound_serialize.h"

namespace rl {
SoundLibrary& SoundLibrary::instance() {
  static SoundLibrary inst;
  return inst;
}

void SoundLibrary::init() {
  RL_LOG_DEBUG("SoundLibrary::init");
  RL_RESREG.on(kResourceTypeSound, "sound");
  RL_RESREG.on(kResourceTypeSoundBank, "soundbank");
  constexpr auto kSoundCapacity = 512;
  slots_.reserve(kSoundCapacity);
}

void SoundLibrary::shutdown() {
  RL_LOG_DEBUG("SoundLibrary::shutdown");
  RL_RESREG.off(kResourceTypeSound);
  RL_RESREG.off(kResourceTypeSoundBank);
  slots_.clear();
}

const SoundResource* SoundLibrary::load(SoundId id) {
  return slots_.acquire(id, [&, id] {
    auto* res = new SoundResource{};

    auto ok = loadResourceFile(kResourceTypeSound, id, SoundResource::kVersion,
                               SoundResource::kVersion, [&](std::istream& is) {
                                 readSoundResource(is, *res);
                               });

    RL_FASSERT(
        ok, "SoundLibrary::load: Failed to load sound resource with id: ", id,
        "!");

    return res;
  });
}

void SoundLibrary::unload(SoundId id) { slots_.release(id); }

const SoundResource* SoundLibrary::get(SoundId id) const {
  return slots_.get(id);
}

const SoundBankResource* SoundLibrary::getBank(SoundBankId id) const {
  return bankSlots_.get(id);
}

const SoundBankResource* SoundLibrary::loadBank(SoundBankId id) {
  return bankSlots_.acquire(id, [&, id] {
    auto* res = new SoundBankResource{};

    auto ok = loadResourceFile(
        kResourceTypeSoundBank, id, SoundBankResource::kVersion,
        SoundBankResource::kVersion,
        [&](std::istream& is) { readSoundBankResource(is, *res); });

    RL_FASSERT(
        ok, "SoundLibrary::loadBank: Failed to load sound bank with id: ", id,
        "!");

    for (SoundId sid : res->sounds) {
      load(sid);
    }

    return res;
  });
}

void SoundLibrary::unloadBank(SoundBankId id) {
  const auto* res = getBank(id);

  if (res && !res->sounds.empty()) {
    for (SoundId sid : res->sounds) {
      unload(sid);
    }
  }

  bankSlots_.release(id);
}
}  // namespace rl