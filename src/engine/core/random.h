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

#ifndef ENGINE_CORE_RANDOM_H_
#define ENGINE_CORE_RANDOM_H_

#include "engine/common.h"
#include "engine/core/type_trait.h"

namespace rl {
class Splitmix64 {
 public:
  Splitmix64() noexcept : seed_(0) {}
  explicit Splitmix64(u64 seed) noexcept : seed_(seed) {}

  void seed(u64 seed) noexcept { seed_ = seed; }

  // Basic PRNG algorithm.
  // https://rosettacode.org/wiki/Pseudo-random_numbers/Splitmix64
  [[nodiscard]] u64 nextU64() noexcept {
    splitmix64Next(seed_);
    return seed_;
  }

  // Integers default: [lo, hi] (closed). Floats default: [lo, hi) (half-open).
  template <class T>
  [[nodiscard]] T next(T minVal, T maxVal) noexcept {
    if (maxVal < minVal) std::swap(minVal, maxVal);

    if constexpr (Enum<T>) {
      using U = std::underlying_type_t<T>;
      return static_cast<T>(
          next<U>(static_cast<U>(minVal), static_cast<U>(maxVal)));
    } else if constexpr (IntegralNotBool<T>) {
      return nextInt(minVal, maxVal);
    } else if constexpr (Floating<T>) {
      return nextReal(minVal, maxVal);
    } else {
      static_assert(!sizeof(T*), "Unsupported T!");
    }
  }

  [[nodiscard]] u64 operator()() noexcept { return nextU64(); }

  [[nodiscard]] f64 next01() noexcept { return unitF64(); }
  [[nodiscard]] f32 next01f() noexcept { return unitF32(); }

  template <IntegralNotBool Int>
  [[nodiscard]] Int nextInt(Int lo, Int hi) noexcept {
    using U = std::make_unsigned_t<Int>;
    if (hi < lo) std::swap(lo, hi);

    auto clo = static_cast<U>(lo);
    auto chi = static_cast<U>(hi);
    auto span = chi - clo + U{1};

    // Case: full span inclusive.
    if (span == U{0}) {
      constexpr auto kBitCount = 8 * sizeof(Int);
      auto bits = static_cast<U>(nextU64() >> (64 - kBitCount));
      return static_cast<Int>(bits);
    }

    auto mapped = boundedU64(nextU64(), static_cast<u64>(span));

    if constexpr (sizeof(U) >= 8) {
      return static_cast<Int>(static_cast<u64>(clo) + mapped);
    } else {
      return static_cast<Int>(clo + static_cast<U>(mapped));
    }
  }

  template <IntegralNotBool T>
  [[nodiscard]] T nextIntClosed(T lo, T hi) noexcept {
    return nextInt(lo, hi);
  }

  template <Enum E>
  [[nodiscard]] E nextIntClosed(E lo, E hi) noexcept {
    using U = std::underlying_type_t<E>;
    return static_cast<E>(
        nextIntClosed(static_cast<U>(lo), static_cast<U>(hi)));
  }

  template <IntegralNotBool T>
  [[nodiscard]] T nextIntHalfOpen(T lo, T hi) noexcept {
    using U = std::make_unsigned_t<T>;
    RL_ASSERT(lo < hi,
              "Splitmix64::nextIntHalfOpen: [lo, hi) must be non-empty!");

    auto clo = static_cast<U>(lo);
    auto chi = static_cast<U>(hi);
    auto span = chi - clo;
    auto mapped = boundedU64(nextU64(), static_cast<u64>(span));

    if constexpr (sizeof(U) >= 8) {
      return static_cast<T>(static_cast<u64>(clo) + mapped);
    } else {
      return static_cast<T>(clo + static_cast<U>(mapped));
    }
  }

  template <Enum E>
  [[nodiscard]] E nextIntHalfOpen(E lo, E hi) noexcept {
    using U = std::underlying_type_t<E>;
    return static_cast<E>(
        nextIntHalfOpen(static_cast<U>(lo), static_cast<U>(hi)));
  }

  template <IntegralNotBool T>
  [[nodiscard]] T nextIntOpen(T lo, T hi) noexcept {
    using U = std::make_unsigned_t<T>;
    RL_ASSERT(hi > lo, "Splitmix64::nextIntOpen: (lo, hi) requires lo < hi!");

    auto span = static_cast<U>(hi) - static_cast<U>(lo) - U{1};
    RL_ASSERT(
        span >= U{1},
        "Splitmix64::nextIntOpen: (lo, hi) must contain at least one integer!");

    auto base = static_cast<U>(lo) + U{1};
    auto mapped = boundedU64(nextU64(), static_cast<u64>(span));

    if constexpr (sizeof(U) >= 8) {
      return static_cast<T>(static_cast<u64>(base) + mapped);
    } else {
      return static_cast<T>(base + static_cast<U>(mapped));
    }
  }

  template <Enum E>
  [[nodiscard]] E nextIntOpen(E lo, E hi) noexcept {
    using U = std::underlying_type_t<E>;
    return static_cast<E>(nextIntOpen(static_cast<U>(lo), static_cast<U>(hi)));
  }

  [[nodiscard]] bool nextBool() noexcept {
    return boundedU64(nextU64(), 2) != 0;
  }

  template <IntegralNotBool Num, IntegralNotBool Den>
  [[nodiscard]] bool nextBool(Num num, Den den) noexcept {
    using U = std::make_unsigned_t<std::common_type_t<Num, Den, u64>>;
    U cden = static_cast<U>(den);
    U cnum = static_cast<U>(num);
    RL_ASSERT(cden > 0 && cnum <= cden,
              "Splitmix64::nextBool: Invalid fraction provided!");
    return boundedU64(nextU64(), static_cast<u64>(cden)) <
           static_cast<u64>(cnum);
  }

  template <Floating F>
  [[nodiscard]] bool nextBool(F p) noexcept {
    RL_ASSERT(std::isfinite(p), "Splitmix64::nextBool: p must be finite!");
    RL_ASSERT(p >= F{0} && p <= F{1},
              "Splitmix64::nextBool: p must be in [0,1]!");

    if constexpr (std::is_same_v<F, f32>) {
      constexpr auto kM = static_cast<u32>(1) << 24;
      auto threshold = (p >= F{1}) ? kM : static_cast<u32>(p * F{kM});
      auto r = static_cast<u32>(nextU64() >> 40);
      return r < threshold;
    } else {
      constexpr auto kM = static_cast<u64>(1) << 53;
      auto threshold = (p >= F{1}) ? kM : static_cast<u64>(p * F{kM});
      auto r = nextU64() >> 11;
      return r < threshold;
    }
  }

  template <Floating F>
  [[nodiscard]] F nextReal(F lo, F hi) noexcept {
    RL_ASSERT(std::isfinite(lo) && std::isfinite(hi),
              "Splitmix64::nextReal: lo/hi must be finite!");
    RL_ASSERT(lo < hi, "Splitmix64::nextReal: (lo,hi) must be non-empty!");
    if (lo == hi) return lo;

    if constexpr (std::is_same_v<F, f32>) {
      auto u = unitF32();
      return static_cast<F>(static_cast<f64>(lo) +
                            static_cast<f64>(hi - lo) * u);
    } else {
      auto u = unitF64();
      return static_cast<F>(static_cast<f64>(lo) +
                            static_cast<f64>(hi - lo) * static_cast<f64>(u));
    }
  }

  template <Floating F>
  [[nodiscard]] F nextRealClosed(F lo, F hi) noexcept {
    RL_ASSERT(std::isfinite(lo) && std::isfinite(hi),
              "Splitmix64::nextRealClosed: lo/hi must be finite!");
    if (lo == hi) return lo;
    auto u = std::is_same_v<F, f32> ? unitF32Closed() : unitF64Closed();
    return static_cast<F>(static_cast<f64>(lo) +
                          static_cast<f64>(hi - lo) * static_cast<f64>(u));
  }

  template <Floating F>
  [[nodiscard]] F nextRealHalfOpen(F lo, F hi) noexcept {
    return nextReal(lo, hi);
  }

  template <Floating F>
  [[nodiscard]] F nextRealOpen(F lo, F hi) noexcept {
    RL_ASSERT(std::isfinite(lo) && std::isfinite(hi),
              "Splitmix64::nextRealOpen: lo/hi must be finite!");
    RL_ASSERT(lo < hi, "Splitmix64::nextRealOpen: (lo,hi) must be non-empty!");
    auto u = std::is_same_v<F, f32> ? unitF32Open() : unitF64Open();
    return static_cast<F>(static_cast<f64>(lo) +
                          static_cast<f64>(hi - lo) * static_cast<f64>(u));
  }

 private:
  static inline constexpr u64 kSplitMix64Gamma_ =
      static_cast<u64>(0x9e3779b97f4a7c15);
  static inline constexpr u64 kSplitMix64Mul1_ =
      static_cast<u64>(0xbf58476d1ce4e5b9);
  static inline constexpr u64 kSplitMix64Mul2_ =
      static_cast<u64>(0x94d049bb133111eb);
  static inline constexpr f64 kInv2Pow53 = 1.0 / 9007199254740992.0;  // 2^53.
  static inline constexpr f32 kInv2Pow24 = 1.0f / 16777216.0f;        // 2^24.

  u64 seed_{0};

  static inline void splitmix64Mix(u64& x) noexcept {
    x = (x ^ (x >> 30)) * kSplitMix64Mul1_;
    x = (x ^ (x >> 27)) * kSplitMix64Mul2_;
    x ^= (x >> 31);
  }

  static inline void splitmix64Next(u64& x) noexcept {
    x += kSplitMix64Gamma_;
    splitmix64Mix(x);
  }

  static inline constexpr u64 boundedU64(u64 r, u64 range) noexcept {
    RL_ASSERT(range > 0, "Splitmix64::boundedU64: Range must be > 0!");
    return mulHiU64(r, range);
  }

  // High 64 bits of 128-bit product.
  static inline constexpr u64 mulHiU64(u64 a, u64 b) noexcept {
    u64 aLo = static_cast<u32>(a);
    auto aHi = a >> 32;
    u64 bLo = static_cast<u32>(b);
    auto bHi = b >> 32;

    // a = aHi * 2^32 + aLo.
    // b = bHi * 2^32 + bLo.
    //
    // So:
    // a * b = (aHi * bHi) * 2^64 + (aHi * bLo + aLo * bHi) * 2^32 + (aLo * bLo)
    //
    // Using:
    //   p0 = aLo * bLo;
    //   p1 = aLo * bHi;
    //   p2 = aHi * bLo;
    //   p3 = aHi * bHi;
    //
    // yields:
    //   a * b = p3 * 2^64 + (p2 + p1) * 2^32 + p0
    //
    // Split 64-bit partials into 32-bit halves:
    //   a * b = p3 * 2^64 + (p1Hi + p2Hi) * 2^64 + (p1Lo + p2Lo) * 2^32
    //           + p0Hi * 2^32 + p0Lo
    //
    // Let mid = p0Hi + p1Lo + p2Lo                                          (A)
    // Therefore hi = p3 + p1Hi + p2Hi + (mid >> 32)                         (B)

    auto p0 = aLo * bLo;
    auto p1 = aLo * bHi;
    auto p2 = aHi * bLo;
    auto p3 = aHi * bHi;

    auto mid = (p0 >> 32) + static_cast<u32>(p1) + static_cast<u32>(p2);  // (A)
    auto hi = p3 + (p1 >> 32) + (p2 >> 32) + (mid >> 32);                 // (B)
    return hi;
  }

  [[nodiscard]] f64 unitF64() noexcept {
    return static_cast<f64>(nextU64() >> 11) * kInv2Pow53;
  }

  [[nodiscard]] f32 unitF32() noexcept {
    return static_cast<f32>(nextU64() >> 40) * kInv2Pow24;
  }

  [[nodiscard]] f64 unitF64Open() noexcept {
    constexpr auto kM = (static_cast<u64>(1) << 53) - 1;
    auto r = boundedU64(nextU64(), kM) + 1;
    return static_cast<f64>(r) * kInv2Pow53;
  }

  [[nodiscard]] f32 unitF32Open() noexcept {
    constexpr auto kM = (static_cast<u32>(1) << 24) - 1;
    auto r = static_cast<u32>(boundedU64(nextU64(), kM)) + 1;
    return static_cast<f32>(r) * kInv2Pow24;
  }

  [[nodiscard]] f64 unitF64HalfOpen() noexcept { return unitF64(); }

  [[nodiscard]] f32 unitF32HalfOpen() noexcept { return unitF32(); }

  [[nodiscard]] f64 unitF64Closed() noexcept {
    constexpr auto kN = (static_cast<u64>(1) << 53) + 1;
    auto r = boundedU64(nextU64(), kN);
    return static_cast<f64>(r) * kInv2Pow53;
  }

  [[nodiscard]] f32 unitF32Closed() noexcept {
    constexpr auto kN = (static_cast<u32>(1) << 24) + 1;
    auto r = static_cast<u32>(boundedU64(nextU64(), kN));
    return static_cast<f32>(r) * kInv2Pow24;
  }
};

inline Splitmix64& globalRng() noexcept {
  static Splitmix64 rng{0xdeadbeef};
  return rng;
}

template <IntegralNotBool SizeT>
inline SizeT fastIndex(SizeT size) noexcept {
  RL_ASSERT(size > 0, "fastIndex: size must be > 0!");
  return globalRng().nextIntHalfOpen<SizeT>(0, size);
}

template <IntegralNotBool SizeT>
inline SizeT fastIndex(Splitmix64& rng, SizeT size) noexcept {
  RL_ASSERT(size > 0, "fastIndex: size must be > 0!");
  return rng.nextIntHalfOpen<SizeT>(0, size);
}
}  // namespace rl

#endif  // ENGINE_CORE_RANDOM_H_
