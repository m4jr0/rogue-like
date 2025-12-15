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

#ifndef ENGINE_RESOURCE_RESOURCE_SERIALIZE_H_
#define ENGINE_RESOURCE_RESOURCE_SERIALIZE_H_

#include "engine/common.h"
#include "engine/core/serialize.h"
#include "engine/resource/resource_type.h"

namespace rl {
inline void writeResourceId(std::ostream& os, ResourceTypeId id) {
  writePod(os, id);
}

inline void readResourceId(std::istream& is, ResourceTypeId& id) {
  readPod(is, id);
}

inline bool readAndCheckResourceId(std::istream& is, ResourceTypeId expected,
                                   ResourceTypeId& id) {
  readResourceId(is, id);
  RL_ASSERT(id == expected,
            "readAndCheckResourceId: resource ID mismatch! Expected: ",
            expected, ", got: ", id, ".");
  return id == expected;
}
}  // namespace rl

#endif  // ENGINE_RESOURCE_RESOURCE_SERIALIZE_H_
