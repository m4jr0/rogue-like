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

#ifndef ENGINE_RESOURCE_RESOURCE_TABLE_H_
#define ENGINE_RESOURCE_RESOURCE_TABLE_H_

#include "engine/common.h"
#include "engine/core/string.h"
#include "engine/resource/resource_type.h"

namespace rl {
class ResourceTable {
 public:
  static ResourceTable& instance();

  void init();
  void shutdown();

  template <typename T>
  T rid(std::string_view nid) const {
    auto it = nameToId_.find(nid);
    RL_ASSERT(it != nameToId_.end(), "ResourceTable::rid: unknown NID: ", nid,
              "!");
    return it != nameToId_.end() ? T{it->second} : kInvalidResourceId;
  }

  template <typename T>
  std::string_view nid(ResourceId<T> rid) const {
    auto id = rid.id;
    RL_ASSERT(id < idToName_.size(), "ResourceTable::nid: Invalid ", rid, "!");
    return idToName_[id];
  }

 private:
  inline constexpr static ResourceVersion kVersion_{1};

  std::filesystem::path indexPath_{};
  std::unordered_map<u32, std::string> idToName_{};
  StringMap<u32> nameToId_{};

  ResourceTable() = default;

  bool loadIndex(const std::filesystem::path& path);
};
}  // namespace rl

#define RL_RESTAB (::rl::ResourceTable::instance())
#define RL_CRESTAB \
  (static_cast<const ::rl::ResourceTable&>(::rl::ResourceTable::instance()))

#endif  // ENGINE_RESOURCE_RESOURCE_TABLE_H_
