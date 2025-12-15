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
#include "sprite_serialize.h"
////////////////////////////////////////////////////////////////////////////////

namespace rl {
void writeSprite(std::ostream& os, const Sprite& s) {
  writePod(os, s.rect);
  writePod(os, s.offset);
  writePod(os, s.offsetFlipped);
  writePod(os, s.sourceSize);
}

void readSprite(std::istream& is, Sprite& s) {
  readPod(is, s.rect);
  readPod(is, s.offset);
  readPod(is, s.offsetFlipped);
  readPod(is, s.sourceSize);
}

void writeAtlasResource(std::ostream& os, const AtlasResource& a) {
  writePod(os, a.id);
  writePod(os, a.tex);

  auto count = a.sprites.size();
  writePod(os, count);

  for (const auto& [name, idx] : a.nameToIdx) {
    const auto& s = a.sprites[idx];

    auto nameLen = static_cast<u32>(name.size());
    writePod(os, nameLen);
    os.write(name.data(), static_cast<std::streamsize>(name.size()));

    writeSprite(os, s);
  }
}

void readAtlasResource(std::istream& is, AtlasResource& a) {
  readPod(is, a.id);
  readPod(is, a.tex);

  usize count;
  readPod(is, count);

  a.sprites.resize(count);
  a.nameToIdx.clear();
  a.nameToIdx.reserve(count);

  for (usize i = 0; i < count; ++i) {
    u32 nameLen;
    readPod(is, nameLen);

    std::string name(nameLen, '\0');
    is.read(name.data(), static_cast<std::streamsize>(nameLen));

    auto idx = static_cast<SpriteIndex>(i);
    a.nameToIdx.emplace(std::move(name), idx);

    readSprite(is, a.sprites[i]);
  }
}
}  // namespace rl