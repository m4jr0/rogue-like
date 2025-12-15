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

#ifndef ENGINE_CORE_PARAM_TRAVERSAL_H_
#define ENGINE_CORE_PARAM_TRAVERSAL_H_

#include "engine/common.h"
#include "engine/core/type_trait.h"
#include "engine/core/value_utils.h"

namespace rl {
enum class PTMode : u8 { None, Loop, PingPong };
using PTIndex = u16;
using PTCount = s32;

struct PTDir {
  constexpr PTDir() = default;

  template <Arithmetic T>
  constexpr explicit PTDir(T v) : value{(v >= T{0}) ? s8{1} : s8{-1}} {}

  constexpr bool forward() const { return value >= s8{0}; }
  constexpr bool reversed() const { return !forward(); }
  constexpr s8 sign() const { return value; }

 private:
  s8 value{1};
};

struct PTContinuousDomain {};
struct PTDiscreteDomain {};

namespace internal {
template <Arithmetic T>
PTCount seamCrossingIndex(T u, T len) {
  if (len <= T{0}) return 0;
  return static_cast<PTCount>(
      std::floor(static_cast<f64>(u) / static_cast<f64>(len)));
}

template <Floating T>
PTCount cycleIndex(PTMode mode, T u, T len) {
  if (len <= T{0}) return 0;

  switch (mode) {
    using enum PTMode;
    case Loop: {
      return static_cast<PTCount>(
          std::floor(static_cast<f64>(u) / static_cast<f64>(len)));
    }
    case PingPong: {
      auto period = T{2} * len;
      return static_cast<PTCount>(
          std::floor(static_cast<f64>(u) / static_cast<f64>(period)));
    }
    case None:
    default:
      return 0;
  }
}
}  // namespace internal

template <Floating T>
struct PTFolded {
  T pos{0};
  PTCount seamCrossingCount{0};
  PTCount cycleCount{0};
  PTDir dir{};
};

template <Floating T>
[[nodiscard]] inline PTFolded<T> foldNone(T u0, T u1, T len) {
  RL_ASSERT(len > 0, "PTFolded::foldNone: Invalid len parameter! len: ", len);
  return {
      .pos = std::clamp(u1, T{0}, len),
      .seamCrossingCount = 0,
      .cycleCount = 0,
      .dir = PTDir{u1 - u0},
  };
}

template <Floating T>
[[nodiscard]] inline PTFolded<T> foldLoop(T u0, T u1, T len) {
  RL_ASSERT(len > 0, "PTFolded::foldLoop: Invalid len parameter! len: ", len);
  auto s0 = internal::seamCrossingIndex(u0, len);
  auto s1 = internal::seamCrossingIndex(u1, len);
  auto seamDelta = s1 - s0;

  auto c0 = internal::cycleIndex(PTMode::Loop, u0, len);
  auto c1 = internal::cycleIndex(PTMode::Loop, u1, len);
  auto cycleDelta = c1 - c0;

  return {
      .pos = modPositive(u1, len),
      .seamCrossingCount = seamDelta,
      .cycleCount = cycleDelta,
      .dir = PTDir{seamDelta != 0 ? seamDelta : (u1 - u0)},
  };
}

template <Floating T>
[[nodiscard]] inline PTFolded<T> foldPingPong(T u0, T u1, T len) {
  RL_ASSERT(len > 0,
            "PTFolded::foldPingPong: Invalid len parameter! len: ", len);
  auto period = T{2} * len;
  auto x = modPositive(u1, period);

  // dir = forward if in [0,len], backward if in (len,2*len).
  auto dir = PTDir{len - x};
  auto p = dir.forward() ? x : (period - x);

  T a;
  T b;

  if (u0 <= u1) {
    a = u0;
    b = u1;
  } else {
    a = u1;
    b = u0;
  }

  auto s0 = internal::seamCrossingIndex(a, len);
  auto s1 = internal::seamCrossingIndex(b, len);
  auto seamDelta = s1 - s0;

  auto c0 = internal::cycleIndex(PTMode::PingPong, a, len);
  auto c1 = internal::cycleIndex(PTMode::PingPong, b, len);
  auto cycleDelta = c1 - c0;

  return PTFolded<T>{
      .pos = p,
      .seamCrossingCount = seamDelta,
      .cycleCount = cycleDelta,
      .dir = dir,
  };
}

template <Floating T>
inline PTFolded<T> fold(PTMode mode, T u0, T u1, T len) {
  switch (mode) {
    using enum PTMode;
    case PingPong:
      return foldPingPong(u0, u1, len);
    case Loop:
      return foldLoop(u0, u1, len);
    case None:
    default:
      return foldNone(u0, u1, len);
  }
}

template <Arithmetic T, typename IntervalPolicy>
struct ParamTraversalSpan {
  bool valid{false};
  T lo{0};
  T hi{0};

  constexpr bool contains(T v) const noexcept {
    return valid && IntervalPolicy::contains(v, lo, hi);
  }
};

template <Arithmetic T, typename IntervalPolicy, typename DomainTag>
struct ParamTraversal {
  using Span = ParamTraversalSpan<T, IntervalPolicy>;

  PTDir dir{};
  PTCount fullSweepCount{0};
  Span begin{};
  Span end{};

  bool contains(T u) const noexcept {
    return fullSweepCount > 0 || begin.contains(u) || end.contains(u);
  }

  constexpr bool any() const {
    return begin.valid || end.valid || fullSweepCount > 0;
  }

  constexpr bool forward() const { return dir.forward(); }
  constexpr bool reversed() const { return dir.reversed(); }
  constexpr s8 sign() const { return dir.sign(); }
};

struct HalfOpen {
  template <Arithmetic T>
  static constexpr bool contains(const T& x, const T& a, const T& b) noexcept {
    return (x >= a) && (x < b);
  }
};

struct Inclusive {
  template <Arithmetic T>
  static constexpr bool contains(const T& x, const T& a, const T& b) noexcept {
    return (x >= a) && (x <= b);
  }
};

template <Floating T, typename Interval = HalfOpen>
inline ParamTraversal<T, Interval, PTContinuousDomain> continuousTraversal(
    PTMode mode, T prev, T cur, T len, PTCount seamCrossingCount,
    PTCount cycleCount) {
  using Tr = ParamTraversal<T, Interval, PTContinuousDomain>;
  RL_ASSERT(
      len > 0,
      "ParamTraversal::continuousTraversal: Invalid len parameter! len: ", len);
  Tr tr{};

  switch (mode) {
    using enum PTMode;
    case PingPong: {
      tr.dir = PTDir{cur - prev};

      tr.begin = {
          .valid = true,
          .lo = std::min(prev, cur),
          .hi = std::max(prev, cur),
      };

      tr.fullSweepCount = std::max<PTCount>(0, 2 * std::abs(cycleCount));
      return tr;
    }
    case Loop: {
      if (seamCrossingCount == 0) {
        tr.begin = {
            .valid = true,
            .lo = std::min(prev, cur),
            .hi = std::max(prev, cur),
        };

        tr.dir = PTDir{cur - prev};
        return tr;
      }

      auto n = std::abs(seamCrossingCount);
      tr.fullSweepCount = std::max(0, n - 1);
      tr.dir = PTDir{seamCrossingCount};

      if (seamCrossingCount > 0) {
        tr.begin = {
            .valid = true,
            .lo = prev,
            .hi = len,
        };

        tr.end = {
            .valid = true,
            .lo = T{0},
            .hi = cur,
        };
      } else {
        tr.begin = {
            .valid = true,
            .lo = cur,
            .hi = len,
        };

        tr.end = {
            .valid = true,
            .lo = T{0},
            .hi = prev,
        };
      }

      return tr;
    }
    case None:
    default: {
      tr.begin = {
          .valid = true,
          .lo = std::min(prev, cur),
          .hi = std::max(prev, cur),
      };

      tr.dir = PTDir{cur - prev};
      return tr;
    }
  }
}

template <IntegralNotBool I, typename Interval = Inclusive>
inline ParamTraversal<PTIndex, Interval, PTDiscreteDomain> discreteTraversal(
    PTMode mode, I prev, I cur, I len, PTCount seamCrossingCount) {
  using Tr = ParamTraversal<PTIndex, Interval, PTDiscreteDomain>;
  RL_ASSERT(
      len > 0,
      "ParamTraversal::discreteTraversal: Invalid len parameter! len: ", len);
  Tr tr{};

  auto iLen = static_cast<s64>(len);
  auto iPrev = modPositive(static_cast<s64>(prev), iLen);
  auto iCur = modPositive(static_cast<s64>(cur), iLen);

  switch (mode) {
    using enum PTMode;
    case PingPong: {
      tr.dir = PTDir{iCur - iPrev};

      if (!tr.reversed()) {
        tr.begin = {
            .valid = true,
            .lo = static_cast<PTIndex>(iPrev + 1),
            .hi = static_cast<PTIndex>(iCur),
        };
      } else {
        tr.begin = {
            .valid = true,
            .lo = static_cast<PTIndex>(iCur),
            .hi = static_cast<PTIndex>(iPrev - 1),
        };
      }

      return tr;
    }
    case Loop: {
      if (seamCrossingCount == 0) {
        tr.dir = PTDir{iCur - iPrev};

        if (tr.dir.forward()) {
          tr.begin = {
              .valid = true,
              .lo = static_cast<PTIndex>(iPrev + 1),
              .hi = static_cast<PTIndex>(iCur),
          };
        } else {
          tr.begin = {
              .valid = true,
              .lo = static_cast<PTIndex>(iCur),
              .hi = static_cast<PTIndex>(iPrev - 1),
          };
        }
        return tr;
      }

      auto n = std::abs(seamCrossingCount);
      tr.fullSweepCount = std::max(0, n - 1);
      tr.dir = PTDir{seamCrossingCount};

      if (tr.dir.forward()) {
        if (iPrev + 1 <= iLen - 1) {
          tr.begin = {
              .valid = true,
              .lo = static_cast<PTIndex>(iPrev + 1),
              .hi = static_cast<PTIndex>(iLen - 1),
          };
        }

        if (iCur >= 0) {
          tr.end = {
              .valid = true,
              .lo = static_cast<PTIndex>(0),
              .hi = static_cast<PTIndex>(iCur),
          };
        }
      } else {
        if (iCur <= iLen - 1) {
          tr.begin = {
              .valid = true,
              .lo = static_cast<PTIndex>(iCur),
              .hi = static_cast<PTIndex>(iLen - 1),
          };
        }

        if (iPrev - 1 >= 0) {
          tr.end = {
              .valid = true,
              .lo = static_cast<PTIndex>(0),
              .hi = static_cast<PTIndex>(iPrev - 1),
          };
        }
      }
      return tr;
    }
    case None:
    default: {
      tr.dir = PTDir{iCur - iPrev};

      if (tr.dir.forward()) {
        tr.begin = {
            .valid = true,
            .lo = static_cast<PTIndex>(iPrev + 1),
            .hi = static_cast<PTIndex>(iCur),
        };
      } else {
        tr.begin = {
            .valid = true,
            .lo = static_cast<PTIndex>(iCur),
            .hi = static_cast<PTIndex>(iPrev - 1),
        };
      }

      return tr;
    }
  }
}

template <typename Tr, typename KeyIt, typename FireFn>
inline void fireAtMostOnce(const Tr& tr, KeyIt first, KeyIt last,
                           FireFn&& fire) {
  for (auto it = first; it != last; ++it) {
    auto key = *it;
    if (tr.contains(key)) std::forward<FireFn>(fire)(key);
  }
}

template <typename Tr, typename KeyIt, typename FireFn>
inline void fireWithMultiplicity(const Tr& tr, KeyIt first, KeyIt last,
                                 FireFn&& fire) {
  for (auto it = first; it != last; ++it) {
    auto key = *it;
    PTCount count = 0;
    if (tr.begin.contains(key)) ++count;
    count += tr.fullSweepCount;
    if (tr.end.contains(key)) ++count;
    if (count > 0) std::forward<FireFn>(fire)(key, count);
  }
}

template <typename VisitFn>
inline void visitIndices(
    const ParamTraversal<PTIndex, Inclusive, PTDiscreteDomain>& tr, PTIndex len,
    VisitFn&& visit) {
  auto walkAsc = [&](PTIndex a, PTIndex b) {
    if (a > b) return;
    for (auto i = a; i <= b; ++i) std::forward<VisitFn>(visit)(i);
  };

  if (tr.begin.valid) walkAsc(tr.begin.lo, tr.begin.hi);
  for (PTCount s = 0; s < tr.fullSweepCount; ++s) walkAsc(0, len - 1);
  if (tr.end.valid) walkAsc(tr.end.lo, tr.end.hi);
}

template <typename T>
using PTContinuousTraversal = ParamTraversal<T, HalfOpen, PTContinuousDomain>;

using PTContinuousF32 = PTContinuousTraversal<f32>;
using PTContinuousF64 = PTContinuousTraversal<f64>;

using PTFoldedF32 = PTFolded<f32>;
using PTFoldedF64 = PTFolded<f64>;

using PTDiscreteTraversal =
    ParamTraversal<PTIndex, Inclusive, PTDiscreteDomain>;
}  // namespace rl

#endif  // ENGINE_CORE_PARAM_TRAVERSAL_H_