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
#include "resource_table.h"
////////////////////////////////////////////////////////////////////////////////
#include "engine/core/log.h"
#include "engine/core/serialize.h"
#include "engine/resource/resource_file.h"
#include "resource_type_registry.h"

namespace rl {
ResourceTable& ResourceTable::instance() {
  static ResourceTable inst;
  return inst;
}

void ResourceTable::init() {
  RL_LOG_DEBUG("ResourceTable::init");
  idToName_.clear();
  nameToId_.clear();
  indexPath_ = *RL_CRESREG.root() / "resources.idx";
  RL_FASSERT(loadIndex(indexPath_),
             "ResourceTable::init: Failed to load index!");
}

void ResourceTable::shutdown() {
  RL_LOG_DEBUG("ResourceTable::shutdown");
  indexPath_.clear();
  idToName_.clear();
  nameToId_.clear();
}

bool ResourceTable::loadIndex(const std::filesystem::path& path) {
  std::ifstream is(path, std::ios::binary);

  if (!is) {
    RL_LOG_ERR("ResourceTable::loadIndex: Could not open index file: ", path,
               "!");
    return false;
  }

  ResourceVersion version;

  if (!readAndCheckResourceFileHeader(is, kResourceTypeResourceIndex,
                                      version)) {
    RL_LOG_ERR("ResourceTable::loadIndex: Failed to read header from: ", path,
               "!");
    return false;
  }

  if (version != kVersion_) {
    RL_LOG_ERR("ResourceTable::loadIndex: Unsupported index version: ", version,
               " (expected ", kVersion_, ").");
    return false;
  }

  usize count;
  readPod(is, count);

  idToName_.clear();
  nameToId_.clear();
  idToName_.reserve(count);
  nameToId_.reserve(count);

  for (usize i = 0; i < count; ++i) {
    u32 id;
    ResourceTypeId type;
    u32 nameLen;

    readPod(is, id);
    readPod(is, type);
    readPod(is, nameLen);

    std::string name(nameLen, '\0');
    is.read(name.data(), static_cast<std::streamsize>(nameLen));

    if (!is) {
      RL_LOG_ERR("ResourceTable::loadIndex: I/O error while reading entry #", i,
                 " from ", path, "!");
      return false;
    }

    idToName_[id] = name;

    [[maybe_unused]] auto [_, inserted] = nameToId_.emplace(name, id);
    RL_ASSERT(inserted,
              "ResourceTable::loadIndex: duplicate canonical name in index: ",
              name, "!");
  }

  return true;
}
}  // namespace rl