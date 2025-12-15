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
#include "texture_serialize.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/serialize.h"

namespace rl {
void writeTextureResource(std::ostream& os, const TextureResource& r) {
  writePod(os, r.id);

  auto fmt = toUnderlying(r.fmt);
  writePod(os, fmt);
  writePod(os, r.size);
  auto dataSize = r.data.size();
  writePod(os, dataSize);

  if (dataSize > 0) {
    os.write(r.data.data(), static_cast<std::streamsize>(dataSize));
  }
}

void readTextureResource(std::istream& is, TextureResource& r) {
  readPod(is, r.id);
  readPod(is, r.fmt);
  readPod(is, r.size);

  usize dataSize;
  readPod(is, dataSize);
  r.data.resize(dataSize);

  if (dataSize > 0) {
    is.read(r.data.data(), static_cast<std::streamsize>(dataSize));
  }
}
}  // namespace rl