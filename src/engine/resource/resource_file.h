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

#ifndef ENGINE_RESOURCE_RESOURCE_FILE_H_
#define ENGINE_RESOURCE_RESOURCE_FILE_H_

#include "engine/common.h"
#include "engine/core/log.h"
#include "engine/resource/resource_file_serialize.h"
#include "engine/resource/resource_type.h"
#include "engine/resource/resource_type_registry.h"

namespace rl {
template <typename T>
std::filesystem::path makeResourcePath(std::string_view subfolder,
                                       ResourceId<T> id) {
  constexpr char kRoot[] = "./resources";
  char fname[16];
  std::snprintf(fname, sizeof(fname), "%u.res", id.id);
  std::filesystem::path p{kRoot};

  if (!subfolder.empty()) {
    p /= subfolder;
  }

  p /= fname;

  return p;
}

template <typename T, typename ReaderFn>
bool loadResourceFile(ResourceTypeId type, ResourceId<T> id,
                      ResourceVersion expectedMinVersion,
                      ResourceVersion expectedMaxVersion, ReaderFn reader) {
  auto path = RL_CRESREG.path(type, id);
  std::ifstream is(path, std::ios::binary);

  if (!is) {
    RL_LOG_ERR("loadResourceFile: Could not open resource: ", path.string(),
               "!");
    return false;
  }

  ResourceVersion version;

  if (!readAndCheckResourceFileHeader(is, type, version)) {
    RL_LOG_ERR("loadResourceFile: Failed to read header for: ", path.string(),
               "!");
    return false;
  }

  if (version < expectedMinVersion || version > expectedMaxVersion) {
    RL_LOG_ERR("loadResourceFile: Version mismatch for: ", path.string(),
               " got: ", version, " expected: [", expectedMinVersion, ",",
               expectedMaxVersion, "].");
    return false;
  }

  reader(is);
  return static_cast<bool>(is);
}

template <typename T, typename WriterFn>
bool saveResourceFile(std::string_view subfolder, ResourceType type,
                      ResourceId<T> id, ResourceVersion version,
                      WriterFn writer) {
  auto path = makeResourcePath(subfolder, id);
  std::filesystem::create_directories(path.parent_path());

  std::ofstream os(path, std::ios::binary);

  if (!os) {
    RL_LOG_ERR("saveResourceFile: Could not open resource for writing: ",
               path.string());
    return false;
  }

  writeResourceFileHeader(os, type, version);
  writer(os);
  return static_cast<bool>(os);
}
}  // namespace rl

#endif  // ENGINE_RESOURCE_RESOURCE_FILE_H_
