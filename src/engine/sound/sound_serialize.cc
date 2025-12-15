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
#include "sound_serialize.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/serialize.h"

namespace rl {
void writeSoundResource(std::ostream& os, const SoundResource& r) {
  writePod(os, r.id);
  writePod(os, r.sampleRate);
  writePod(os, r.channelCount);

  auto sampleCount = r.samples.size();
  writePod(os, sampleCount);

  if (sampleCount > 0) {
    os.write(reinterpret_cast<const char*>(r.samples.data()),
             static_cast<std::streamsize>(sampleCount * sizeof(f32)));
  }
}

void readSoundResource(std::istream& is, SoundResource& r) {
  readPod(is, r.id);
  readPod(is, r.sampleRate);
  readPod(is, r.channelCount);

  usize sampleCount;
  readPod(is, sampleCount);
  r.samples.resize(sampleCount);

  if (sampleCount > 0) {
    is.read(reinterpret_cast<char*>(r.samples.data()),
            static_cast<std::streamsize>(sampleCount * sizeof(f32)));
  }
}

void writeSoundBankResource(std::ostream& os, const SoundBankResource& r) {
  writePod(os, r.id);
  auto count = r.sounds.size();
  writePod(os, count);

  for (SoundId sid : r.sounds) {
    writePod(os, sid.id);
  }
}

void readSoundBankResource(std::istream& is, SoundBankResource& r) {
  readPod(is, r.id);
  usize count;
  readPod(is, count);
  r.sounds.resize(count);

  for (usize i = 0; i < count; ++i) {
    readPod(is, r.sounds[i].id);
  }
}
}  // namespace rl