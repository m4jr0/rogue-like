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

#ifndef ENGINE_RESOURCE_RESOURCE_FILE_TYPE_H_
#define ENGINE_RESOURCE_RESOURCE_FILE_TYPE_H_

#include "engine/common.h"
#include "engine/resource/resource_type.h"

namespace rl {
using ResourceMagic = u32;
constexpr auto kInvalidResourceMagic = static_cast<ResourceMagic>(-1);

inline constexpr ResourceMagic makeFourCC(char a, char b, char c, char d) {
  return (static_cast<ResourceMagic>(a) << 24) |
         (static_cast<ResourceMagic>(b) << 16) |
         (static_cast<ResourceMagic>(c) << 8) | (static_cast<ResourceMagic>(d));
}

constexpr ResourceMagic kResourceFileMagic = makeFourCC('R', 'L', 'R', 'S');

struct ResourceFileHeader {
  ResourceMagic magic{kInvalidResourceMagic};
  ResourceTypeId type;
  u32 version;
};

void writeResourceFileHeader(std::ostream& os, ResourceTypeId type,
                             u32 version);
bool readAndCheckResourceFileHeader(std::istream& is,
                                    ResourceTypeId expectedType,
                                    u32& outVersion);
}  // namespace rl

#endif  // ENGINE_RESOURCE_RESOURCE_FILE_TYPE_H_
