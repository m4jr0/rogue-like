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

#ifndef ENGINE_CORE_DEBUG_H_
#define ENGINE_CORE_DEBUG_H_

#include <cassert>
#include <iostream>
#include <string_view>

namespace rl {
namespace internal {
template <typename... Args>
void dumpAssert(std::string_view cond, Args&&... args) {
  std::cout << "[RL] ASSERT ERROR: { " << cond << " }! ";
  ((std::cout << std::forward<Args>(args)), ...);
  std::cout << '\n';
}
}  // namespace internal
}  // namespace rl

#ifdef RL_DEBUG
#define RL_ASSERT(cond, ...)                          \
  do {                                                \
    if (!(cond)) {                                    \
      ::rl::internal::dumpAssert(#cond, __VA_ARGS__); \
      assert(false);                                  \
    }                                                 \
  } while (0)
#else
#define RL_ASSERT(cond, ...) ((void)0)
#endif  // RL_DEBUG

#ifdef RL_DEBUG
#define RL_FASSERT(func, ...) RL_ASSERT(func, ##__VA_ARGS__)
#else
#define RL_FASSERT(func, ...) ((void)(func))
#endif  // RL_DEBUG

#endif  // ENGINE_CORE_DEBUG_H_
