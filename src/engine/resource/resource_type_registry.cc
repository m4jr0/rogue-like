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
#include "resource_type.h"
////////////////////////////////////////////////////////////////////////////////
#include "engine/core/log.h"
#include "resource_type_registry.h"

namespace rl {
ResourceTypeRegistry& ResourceTypeRegistry::instance() {
  static ResourceTypeRegistry inst;
  return inst;
}

void ResourceTypeRegistry::init() {
  RL_LOG_DEBUG("ResourceTypeRegistry::init");
}

void ResourceTypeRegistry::shutdown() {
  RL_LOG_DEBUG("ResourceTypeRegistry::shutdown");
}

bool ResourceTypeRegistry::on(ResourceTypeId type, std::string_view name) {
  if (auto it = registry_.find(type); it != registry_.end()) {
    RL_LOG_ERR("onResourceType: type already registered: ", type);
    return false;
  }

  registry_.emplace(
      type, ResourceTypeEntry{.name = std::string{name}, .path = root_ / name});
  return true;
}

bool ResourceTypeRegistry::off(ResourceTypeId type) {
  return registry_.erase(type) != 0;
}

void ResourceTypeRegistry::root(std::filesystem::path path) {
  root_ = std::move(path);
}

const std::filesystem::path* ResourceTypeRegistry::root() const noexcept {
  return &root_;
}

const char* ResourceTypeRegistry::typeName(ResourceTypeId type) const noexcept {
  const auto* e = entry(type);
  return !e ? nullptr : e->name.c_str();
}

const std::filesystem::path* ResourceTypeRegistry::typePath(
    ResourceTypeId type) const noexcept {
  const auto* e = entry(type);
  return !e ? nullptr : &e->path;
}

const ResourceTypeRegistry::ResourceTypeEntry* ResourceTypeRegistry::entry(
    ResourceTypeId type) const noexcept {
  if (auto it = registry_.find(type); it != registry_.end()) {
    return &it->second;
  }

  return nullptr;
}
}  // namespace rl