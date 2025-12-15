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

#ifndef ENGINE_CORE_VECTOR_H_
#define ENGINE_CORE_VECTOR_H_

#include "engine/common.h"
#include "engine/core/random.h"

namespace rl {
template <typename T>
inline void eraseUnordered(std::vector<T>& v, const T& value) {
  for (usize i = 0; i < v.size(); ++i) {
    if (v[i] == value) {
      v[i] = std::move(v.back());
      v.pop_back();
      return;
    }
  }
}

template <typename T>
inline void pushBackUnique(std::vector<T>& v, const T& value) {
  if (std::find(v.begin(), v.end(), value) == v.end()) v.emplace_back(value);
}

template <typename T>
inline void ensureCapacity(std::vector<T>& v, usize i, f32 growth = 2.0f) {
  if (i < v.size()) return;
  auto newSize = std::max<usize>(1, v.size());
  newSize = std::max(newSize, static_cast<usize>((i + 1) * growth));
  v.resize(newSize);
}

template <typename T>
inline T& randomPick(std::vector<T>& v) {
  RL_ASSERT(!v.empty(), "randomChoice: vector is empty!");
  auto idx = fastIndex<usize>(v.size());
  return v[idx];
}

template <typename T>
inline const T& randomPick(const std::vector<T>& v) {
  RL_ASSERT(!v.empty(), "randomChoice: vector is empty!");
  auto idx = fastIndex<usize>(v.size());
  return v[idx];
}
}  // namespace rl

#endif  // ENGINE_CORE_VECTOR_H_