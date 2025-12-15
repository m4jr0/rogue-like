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

#ifndef ENGINE_SOUND_SOUND_SERIALIZE_H_
#define ENGINE_SOUND_SOUND_SERIALIZE_H_

#include "engine/common.h"
#include "engine/sound/sound_resource.h"

namespace rl {
void writeSoundResource(std::ostream& os, const SoundResource& s);
void readSoundResource(std::istream& is, SoundResource& r);
void writeSoundBankResource(std::ostream& os, const SoundBankResource& r);
void readSoundBankResource(std::istream& is, SoundBankResource& r);
}  // namespace rl

#endif  // ENGINE_SOUND_SOUND_SERIALIZE_H_
