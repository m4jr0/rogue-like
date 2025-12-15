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

#ifndef ENGINE_CORE_STRING_H_
#define ENGINE_CORE_STRING_H_

#include "engine/common.h"

struct SvHash {
  using is_transparent = void;
  std::size_t operator()(std::string_view s) const noexcept {
    return std::hash<std::string_view>{}(s);
  }
  std::size_t operator()(const std::string& s) const noexcept {
    return std::hash<std::string_view>{}(s);
  }
  std::size_t operator()(const char* s) const noexcept {
    return std::hash<std::string_view>{}(s);
  }
};

using SvEq = std::equal_to<>;

template <typename T>
using StringMap = std::unordered_map<std::string, T, SvHash, SvEq>;

#endif  // ENGINE_CORE_STRING_H_