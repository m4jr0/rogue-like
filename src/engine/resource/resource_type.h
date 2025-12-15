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

#ifndef ENGINE_RESOURCE_RESOURCE_TYPE_H_
#define ENGINE_RESOURCE_RESOURCE_TYPE_H_

#include "engine/common.h"

namespace rl {
struct kInvalidResourceIdT {};
inline constexpr kInvalidResourceIdT kInvalidResourceId{};

template <typename Tag>
struct ResourceId {
  u32 id{kInvalidResourceId};

  constexpr ResourceId() noexcept = default;
  constexpr ResourceId(kInvalidResourceIdT) noexcept : id{kInvalidResourceId} {}
  constexpr ResourceId(u32 id) noexcept : id{id} {}

  constexpr bool valid() const { return id != kInvalidResourceId; }
  explicit constexpr operator bool() const { return valid(); }
  explicit constexpr operator u32() const { return id; }

  constexpr void clear() { id = kInvalidResourceId; }

  friend constexpr bool operator==(ResourceId a, ResourceId b) {
    return a.id == b.id;
  }

  friend constexpr bool operator!=(ResourceId a, ResourceId b) {
    return !(a == b);
  }

  friend constexpr bool operator==(ResourceId a, kInvalidResourceIdT) noexcept {
    return !a.valid();
  }

  friend constexpr bool operator==(kInvalidResourceIdT, ResourceId a) noexcept {
    return !a.valid();
  }

  friend constexpr bool operator!=(ResourceId a, kInvalidResourceIdT) noexcept {
    return a.valid();
  }

  friend constexpr bool operator!=(kInvalidResourceIdT, ResourceId a) noexcept {
    return a.valid();
  }

 private:
  inline static u32 kInvalidResourceId = static_cast<u32>(-1);
};

using ResourceTypeId = u32;
constexpr ResourceTypeId kResourceTypeIdCustomOffset = 1000;

enum ResourceType : ResourceTypeId {
  kResourceTypeUnknown = 0,
  kResourceTypeResourceIndex,
  kResourceTypeTexture,
  kResourceTypeAtlas,
  kResourceTypeAnimSet,
  kResourceTypeAnimColliderProfile,
  kResourceTypeSound,
  kResourceTypeSoundBank,
  kResourceTypeCustomOffset = kResourceTypeIdCustomOffset
};

using ResourceVersion = u32;
constexpr auto kInvalidResourceVersion = static_cast<ResourceVersion>(-1);
}  // namespace rl

namespace std {
template <class Tag>
struct hash<::rl::ResourceId<Tag>> {
  std::size_t operator()(const ::rl::ResourceId<Tag>& r) const noexcept {
    return std::hash<std::uint32_t>()(r.id);
  }
};

template <typename Tag>
inline std::ostream& operator<<(std::ostream& os,
                                const ::rl::ResourceId<Tag>& r) {
  if (!r.valid())
    return os << "ResourceId<" << typeid(Tag).name() << ">[INVALID]";
  return os << "ResourceId<" << typeid(Tag).name() << ">[" << r.id << "]";
}
}  // namespace std

#endif  // ENGINE_RESOURCE_RESOURCE_TYPE_H_
