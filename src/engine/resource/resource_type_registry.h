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

#ifndef ENGINE_RESOURCE_RESOURCE_TYPE_REGISTRY_H_
#define ENGINE_RESOURCE_RESOURCE_TYPE_REGISTRY_H_

#include "engine/common.h"
#include "engine/core/log.h"
#include "engine/resource/resource_type.h"

namespace rl {
class ResourceTypeRegistry {
 public:
  static ResourceTypeRegistry& instance();

  void init();
  void shutdown();

  bool on(ResourceTypeId type, std::string_view name);
  bool off(ResourceTypeId type);

  void root(std::filesystem::path path);
  const std::filesystem::path* root() const noexcept;

  const char* typeName(ResourceTypeId type) const noexcept;
  const std::filesystem::path* typePath(ResourceTypeId type) const noexcept;

  template <typename T>
  std::filesystem::path path(ResourceTypeId type,
                             ResourceId<T> id) const noexcept {
    const auto* p = typePath(type);
    RL_ASSERT(p, "makeResourcePath: type not registered: ", type, "!");
    if (!p) return root_;
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%04u.res", id.id);
    return *p / buf;
  }

 private:
  struct ResourceTypeEntry {
    std::string name{};
    std::filesystem::path path{};
  };

  std::filesystem::path root_{"resources"};
  std::unordered_map<ResourceTypeId, ResourceTypeEntry> registry_{};

  ResourceTypeRegistry() = default;

  const ResourceTypeEntry* entry(ResourceTypeId type) const noexcept;
};
}  // namespace rl

#define RL_RESREG (::rl::ResourceTypeRegistry::instance())
#define RL_CRESREG                                 \
  (static_cast<const ::rl::ResourceTypeRegistry&>( \
      ::rl::ResourceTypeRegistry::instance()))

#endif  // ENGINE_RESOURCE_RESOURCE_TYPE_REGISTRY_H_
