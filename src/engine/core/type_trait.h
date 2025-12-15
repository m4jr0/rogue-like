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

#ifndef ENGINE_CORE_TYPE_TRAIT_H_
#define ENGINE_CORE_TYPE_TRAIT_H_

#include "engine/common.h"

namespace rl {
template <typename E>
constexpr auto toUnderlying(E e) noexcept -> std::underlying_type_t<E> {
  static_assert(std::is_enum_v<E>, "toUnderlying requires an enum");
  return static_cast<std::underlying_type_t<E>>(e);
}

template <typename E>
using underlying = std::underlying_type_t<E>;

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept IntegralNotBool =
    std::is_integral_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>;

template <typename T>
concept Enum = std::is_enum_v<T>;

template <typename T>
concept Floating = std::is_floating_point_v<T>;
}  // namespace rl

#endif  // ENGINE_CORE_TYPE_TRAIT_H_