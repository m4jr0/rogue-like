
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

#ifndef ENGINE_CORE_HASH_H_
#define ENGINE_CORE_HASH_H_

#include "engine/common.h"

namespace rl {
namespace internal {
constexpr u32 kFnvPrime32{16777619};
constexpr u32 kFnvOffsetBasis32{2166136261};
constexpr u64 kFnvPrime64{1099511628211ULL};
constexpr u64 kFnvOffsetBasis64{14695981039346656037ULL};
}  // namespace internal

using HashValue = u32;
constexpr HashValue kInvalidHashValue{0};

[[nodiscard]] inline constexpr HashValue GenerateHash(u32 value) {
  return value * internal::kFnvPrime32;
}

[[nodiscard]] inline constexpr HashValue GenerateHash(bool value) {
  return static_cast<u32>(value) * internal::kFnvPrime32;
}

[[nodiscard]] inline constexpr HashValue GenerateHash(s8 value) {
  return GenerateHash(static_cast<u32>(value));
}

[[nodiscard]] inline constexpr HashValue GenerateHash(u8 value) {
  return GenerateHash(static_cast<u32>(value));
}

[[nodiscard]] inline constexpr HashValue GenerateHash(s16 value) {
  return GenerateHash(static_cast<u32>(value));
}

[[nodiscard]] inline constexpr HashValue GenerateHash(u16 value) {
  return GenerateHash(static_cast<u32>(value));
}

[[nodiscard]] inline constexpr HashValue GenerateHash(s32 value) {
  return GenerateHash(static_cast<u32>(value));
}

[[nodiscard]] inline constexpr HashValue GenerateHash(s64 value) {
  return static_cast<u32>(value) ^ static_cast<u32>(value >> 32);
}

[[nodiscard]] inline constexpr HashValue GenerateHash(u64 value) {
  return static_cast<u32>(value) ^ static_cast<u32>(value >> 32);
}

[[nodiscard]] inline constexpr HashValue GenerateHash(f32 value) {
  union Tmp {
    f32 f;
    u32 u;
  };

  Tmp bits{value};
  return bits.u * internal::kFnvPrime32;
}

[[nodiscard]] inline constexpr HashValue GenerateHash(f64 value) {
  union Tmp {
    f64 f;
    u64 u;
  };

  Tmp bits{value};
  return static_cast<u32>(bits.u * internal::kFnvPrime64);
}

[[nodiscard]] inline constexpr HashValue GenerateHash(char value) {
  return GenerateHash(static_cast<u32>(value));
}

[[nodiscard]] inline constexpr HashValue GenerateHash(const void* value) {
  if (value == nullptr) {
    return 0;
  }

  auto cast{reinterpret_cast<u64>(value)};
  return static_cast<u32>(cast) ^ static_cast<u32>(cast >> 32);
}

constexpr HashValue fnv1a32(std::span<const std::byte> bytes) noexcept {
  HashValue h = internal::kFnvOffsetBasis32;

  for (auto b : bytes) {
    h ^= static_cast<HashValue>(std::to_integer<unsigned char>(b));
    h *= internal::kFnvPrime32;
  }

  return h;
}

constexpr HashValue fnv1a32(std::string_view s) noexcept {
  HashValue h = internal::kFnvOffsetBasis32;

  for (auto c : s) {
    h ^= static_cast<HashValue>(c);
    h *= internal::kFnvPrime32;
  }

  return h;
}
}  // namespace rl

#endif  // ENGINE_CORE_HASH_H_
