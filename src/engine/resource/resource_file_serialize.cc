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
#include "resource_file_serialize.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/resource/resource_serialize.h"

namespace rl {
void writeResourceFileHeader(std::ostream& os, ResourceTypeId type,
                             u32 version) {
  ResourceFileHeader h{
      .magic = kResourceFileMagic,
      .type = type,
      .version = version,
  };

  writePod(os, h.magic);
  writePod(os, h.type);
  writePod(os, h.version);
}

bool readAndCheckResourceFileHeader(std::istream& is,
                                    ResourceTypeId expectedType,
                                    u32& outVersion) {
  ResourceFileHeader h{};
  readPod(is, h.magic);
  readPod(is, h.type);
  readPod(is, h.version);

  if (h.magic != kResourceFileMagic) {
    RL_ASSERT(false, "readAndCheckResourceFileHeader: Bad magic: ", h.magic,
              "!");
    return false;
  }

  if (h.type != expectedType) {
    RL_ASSERT(false,
              "readAndCheckResourceFileHeader: Type mismatch! Expected: ",
              expectedType, ", got: ", h.type, ".");
    return false;
  }

  outVersion = h.version;
  return true;
}
}  // namespace rl