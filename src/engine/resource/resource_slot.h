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

#ifndef ENGINE_RESOURCE_RESOURCE_SLOT_H_
#define ENGINE_RESOURCE_RESOURCE_SLOT_H_

#include "engine/common.h"
#include "engine/resource/resource_type.h"

namespace rl {
using ResourceSlotVersion = u32;

template <typename Tag, typename T>
class ResourceSlots {
 public:
  using IdT = ResourceId<Tag>;
  using Value = T;

  template <class Generator>
  T* acquire(IdT id, Generator&& generator) {
    auto& slot = map_[id];

    if (slot.refCount == 0) {
      slot.refCount = 1;
      slot.value = std::unique_ptr<T>(std::forward<Generator>(generator)());
      ++slot.version;
      return slot.value.get();
    }

    ++slot.refCount;

    if (!slot.value) {
      slot.value = std::unique_ptr<T>(std::forward<Generator>(generator)());
      ++slot.version;
    }

    return slot.value.get();
  }

  T* retain(IdT id) {
    auto& slot = map_[id];

    if (slot.refCount == 0) {
      slot.refCount = 1;
      ++slot.version;
      return slot.value.get();
    }

    ++slot.refCount;
    return slot.value.get();
  }

  T* get(IdT id) noexcept {
    auto it = map_.find(id);
    if (it == map_.end() || it->second.refCount == 0) return nullptr;
    return it->second.refCount ? it->second.value.get() : nullptr;
  }

  const T* get(IdT id) const noexcept {
    auto it = map_.find(id);
    if (it == map_.end() || it->second.refCount == 0) return nullptr;
    return it->second.refCount ? it->second.value.get() : nullptr;
  }

  T* set(IdT id, std::unique_ptr<T> v) {
    auto& slot = map_[id];
    if (slot.refCount == 0) slot.refCount = 1;
    slot.value = std::move(v);
    ++slot.version;
    return slot.value.get();
  }

  template <class... Args>
  T* emplace(IdT id, Args&&... args) {
    auto& slot = map_[id];
    if (slot.refCount == 0) slot.refCount = 1;
    slot.value = std::make_unique<T>(std::forward<Args>(args)...);
    ++slot.version;
    return slot.value.get();
  }

  bool release(IdT id) {
    auto it = map_.find(id);
    if (it == map_.end()) return false;
    auto& s = it->second;
    if (s.refCount == 0) return false;

    if (--s.refCount == 0) {
      s.value.reset();
      map_.erase(it);
      return true;
    }

    return false;
  }

  void clear() { map_.clear(); }

  bool has(IdT id) const noexcept {
    auto it = map_.find(id);
    return it != map_.end() && it->second.refCount > 0;
  }

  void reserve(usize c) { map_.reserve(c); }
  usize size() const noexcept { return map_.size(); }
  bool empty() const noexcept { return size() == 0; }

  ResourceSlotVersion version(IdT id) const noexcept {
    auto it = map_.find(id);
    return (it == map_.end()) ? 0 : it->second.version;
  }

 private:
  struct Slot {
    u32 refCount{0};
    ResourceSlotVersion version{0};
    std::unique_ptr<T> value{};
  };

  std::unordered_map<IdT, Slot> map_{};

 public:
  using map_type = std::unordered_map<IdT, Slot>;
  using iterator = typename map_type::iterator;
  using const_iterator = typename map_type::const_iterator;

  iterator begin() noexcept { return map_.begin(); }
  iterator end() noexcept { return map_.end(); }
  const_iterator begin() const noexcept { return map_.begin(); }
  const_iterator end() const noexcept { return map_.end(); }
  const_iterator cbegin() const noexcept { return map_.cbegin(); }
  const_iterator cend() const noexcept { return map_.cend(); }
};
}  // namespace rl

#endif  // ENGINE_RESOURCE_RESOURCE_SLOT_H_
