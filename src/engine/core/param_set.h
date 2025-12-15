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

#ifndef ENGINE_CORE_PARAM_SET_H_
#define ENGINE_CORE_PARAM_SET_H_

#include "engine/common.h"
namespace rl {
template <class T>
concept SmallTrivial = std::is_trivially_copyable_v<std::remove_cvref_t<T>> &&
                       (sizeof(std::remove_cvref_t<T>) <= sizeof(u64));

template <class T>
concept SupportedTypes =
    std::is_same_v<std::remove_cvref_t<T>, bool> ||
    std::is_same_v<std::remove_cvref_t<T>, s8> ||
    std::is_same_v<std::remove_cvref_t<T>, s16> ||
    std::is_same_v<std::remove_cvref_t<T>, s32> ||
    std::is_same_v<std::remove_cvref_t<T>, s64> ||
    std::is_same_v<std::remove_cvref_t<T>, u8> ||
    std::is_same_v<std::remove_cvref_t<T>, u16> ||
    std::is_same_v<std::remove_cvref_t<T>, u32> ||
    std::is_same_v<std::remove_cvref_t<T>, u64> ||
    std::is_same_v<std::remove_cvref_t<T>, f32> ||
    std::is_same_v<std::remove_cvref_t<T>, f64> ||
    std::is_same_v<std::remove_cvref_t<T>, byte> ||
    std::is_same_v<std::remove_cvref_t<T>, u8cu> ||
    std::is_same_v<std::remove_cvref_t<T>, u16cu> ||
    std::is_same_v<std::remove_cvref_t<T>, u32cu> ||
    std::is_enum_v<std::remove_cvref_t<T>> || SmallTrivial<T>;

union GenericParam {
  bool bV;
  s8 s8V;
  s16 s16V;
  s32 s32V;
  s64 s64V;
  u8 u8V;
  u16 u16V;
  u32 u32V;
  u64 u64V;
  f32 f32V;
  f64 f64V;
  byte byteV;
  u8cu u8cuV;
  u16cu u16cuV;
  u32cu u32cuV;

  template <SupportedTypes T>
  constexpr void set(T&& v) {
    using U = std::remove_cvref_t<T>;
    if constexpr (std::is_enum_v<U>) {
      using Base = std::underlying_type_t<U>;
      set(static_cast<Base>(v));
    } else if constexpr (std::is_same_v<U, bool>) {
      bV = v;
    } else if constexpr (std::is_same_v<U, s8>) {
      s8V = v;
    } else if constexpr (std::is_same_v<U, s16>) {
      s16V = v;
    } else if constexpr (std::is_same_v<U, s32>) {
      s32V = v;
    } else if constexpr (std::is_same_v<U, s64>) {
      s64V = v;
    } else if constexpr (std::is_same_v<U, u8>) {
      u8V = v;
    } else if constexpr (std::is_same_v<U, u16>) {
      u16V = v;
    } else if constexpr (std::is_same_v<U, u32>) {
      u32V = v;
    } else if constexpr (std::is_same_v<U, u64>) {
      u64V = v;
    } else if constexpr (std::is_same_v<U, f32>) {
      f32V = v;
    } else if constexpr (std::is_same_v<U, f64>) {
      f64V = v;
    } else if constexpr (std::is_same_v<U, byte>) {
      byteV = v;
    } else if constexpr (std::is_same_v<U, u8cu>) {
      u8cuV = v;
    } else if constexpr (std::is_same_v<U, u16cu>) {
      u16cuV = v;
    } else if constexpr (std::is_same_v<U, u32cu>) {
      u32cuV = v;
    } else if constexpr (SmallTrivial<U>) {
      U tmp = v;
      std::memcpy(&u64V, &tmp, sizeof(U));
    } else {
      static_assert(always_false_v<T>, "Unsupported GenericParam type!");
    }
  }

  template <class T>
  void set(T&&) = delete;

  template <typename T>
  constexpr T get() const noexcept {
    using U = std::remove_cv_t<std::remove_reference_t<T>>;

    if constexpr (std::is_same_v<U, bool>) {
      return bV;
    } else if constexpr (std::is_same_v<U, s8>) {
      return s8V;
    } else if constexpr (std::is_same_v<U, s16>) {
      return s16V;
    } else if constexpr (std::is_same_v<U, s32>) {
      return s32V;
    } else if constexpr (std::is_same_v<U, s64>) {
      return s64V;
    } else if constexpr (std::is_same_v<U, u8>) {
      return u8V;
    } else if constexpr (std::is_same_v<U, u16>) {
      return u16V;
    } else if constexpr (std::is_same_v<U, u32>) {
      return u32V;
    } else if constexpr (std::is_same_v<U, u64>) {
      return u64V;
    } else if constexpr (std::is_same_v<U, f32>) {
      return f32V;
    } else if constexpr (std::is_same_v<U, f64>) {
      return f64V;
    } else if constexpr (std::is_same_v<U, byte>) {
      return byteV;
    } else if constexpr (std::is_same_v<U, u8cu>) {
      return u8cuV;
    } else if constexpr (std::is_same_v<U, u16cu>) {
      return u16cuV;
    } else if constexpr (std::is_same_v<U, u32cu>) {
      return u32cuV;
    } else if constexpr (std::is_enum_v<U>) {
      using Base = std::underlying_type_t<U>;
      return static_cast<U>(get<Base>());
    } else if constexpr (SmallTrivial<U>) {
      U out{};
      std::memcpy(&out, &u64V, sizeof(U));
      return out;
    } else {
      static_assert(always_false_v<T>, "Unsupported GenericParam type!");
    }
  }

 private:
  template <class>
  static constexpr bool always_false_v = false;
};

template <usize N>
struct ParamSet {
  static constexpr usize kParamMaxCount = N;
  GenericParam params[N]{};

  constexpr ParamSet() : params{} {}

  template <typename... Ps>
  constexpr ParamSet(Ps&&... ps) : params{} {
    static_assert(sizeof...(Ps) <= kParamMaxCount,
                  "Too many parameters for ParamSet");
    usize i = 0;
    ((params[i++].set(std::forward<Ps>(ps))), ...);
  }

  template <typename T>
  constexpr void set(usize i, T&& v) {
    RL_ASSERT(i < kParamMaxCount,
              "ParamSet::set: Parameter index out of range");
    params[i].set(std::forward<T>(v));
  }

  template <typename T>
  constexpr T get(usize i) const {
    RL_ASSERT(i < kParamMaxCount,
              "ParamSet::get: Parameter index out of range");
    return params[i].template get<T>();
  }

  constexpr GenericParam* data() noexcept { return params; }
  constexpr const GenericParam* data() const noexcept { return params; }

  constexpr GenericParam* begin() noexcept { return params; }
  constexpr const GenericParam* begin() const noexcept { return params; }
  constexpr GenericParam* end() noexcept { return params + N; }
  constexpr const GenericParam* end() const noexcept { return params + N; }
};
}  // namespace rl

#endif  // ENGINE_CORE_PARAM_SET_H_
