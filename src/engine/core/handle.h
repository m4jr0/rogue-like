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

#ifndef ENGINE_CORE_HANDLE_H_
#define ENGINE_CORE_HANDLE_H_

#include "engine/common.h"

namespace rl {
struct InvalidHandleT {};
inline constexpr InvalidHandleT kInvalidHandle{};

template <typename Tag>
struct Handle {
  u32 index{kInvalidHandleIndex};
  u32 gen{0};

  constexpr Handle() noexcept = default;
  constexpr Handle(InvalidHandleT) noexcept
      : index{kInvalidHandleIndex}, gen{0} {}
  constexpr Handle(u32 i, u32 g) noexcept : index{i}, gen{g} {}

  constexpr bool valid() const { return index != kInvalidHandleIndex; }
  explicit constexpr operator bool() const { return valid(); }

  constexpr void clear() { index = kInvalidHandleIndex; }

  friend constexpr bool operator==(Handle a, Handle b) {
    return a.index == b.index && a.gen == b.gen;
  }

  friend constexpr bool operator!=(Handle a, Handle b) { return !(a == b); }

  friend constexpr bool operator==(Handle a, InvalidHandleT) noexcept {
    return !a.valid();
  }

  friend constexpr bool operator==(InvalidHandleT, Handle a) noexcept {
    return !a.valid();
  }

  friend constexpr bool operator!=(Handle a, InvalidHandleT) noexcept {
    return a.valid();
  }

  friend constexpr bool operator!=(InvalidHandleT, Handle a) noexcept {
    return a.valid();
  }

 private:
  inline static u32 kInvalidHandleIndex = static_cast<u32>(-1);
};

template <typename Tag>
class HandlePool {
 public:
  using H = Handle<Tag>;

  H generate() {
    u32 idx;

    if (!free_.empty()) {
      idx = free_.back();
      free_.pop_back();
    } else {
      idx = static_cast<u32>(gen_.size());
      gen_.push_back(0);
    }

    return H{idx, gen_[idx]};
  }

  void destroy(H h) {
    if (!alive(h)) return;
    ++gen_[h.index];
    free_.push_back(h.index);
  }

  bool alive(H h) const {
    return h.valid() && h.index < gen_.size() && gen_[h.index] == h.gen;
  }

  void reserve(u32 n) {
    gen_.reserve(n);
    free_.reserve(n);
  }

  void clear() {
    gen_.clear();
    free_.clear();
  }

  u32 size() const { return static_cast<u32>(gen_.size()); }
  bool empty() const { return gen_.empty(); }

 private:
  std::vector<u32> gen_{};
  std::vector<u32> free_{};
};
}  // namespace rl

namespace std {
template <class Tag>
struct hash<::rl::Handle<Tag>> {
  std::size_t operator()(const ::rl::Handle<Tag>& h) const noexcept {
    return (static_cast<std::size_t>(h.gen) << 32) ^ h.index;
  }
};

template <typename Tag>
inline std::ostream& operator<<(std::ostream& os, const ::rl::Handle<Tag>& h) {
  if (!h.valid()) return os << "Handle<" << typeid(Tag).name() << ">[INVALID]";
  return os << "Handle<" << typeid(Tag).name() << ">[" << h.index << "#"
            << h.gen << "]";
}
}  // namespace std

#endif  // ENGINE_CORE_HANDLE_H_
