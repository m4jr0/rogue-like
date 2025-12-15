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

#ifndef ENGINE_CORE_FRAME_H_
#define ENGINE_CORE_FRAME_H_

#include "engine/common.h"

namespace rl {
using Frame = u64;

constexpr f64 kFixedHz = 60.0f;
constexpr f64 kFixedStep = 1.0f / kFixedHz;

[[nodiscard]] inline constexpr f64 framesToSec(Frame frames) noexcept {
  return frames * kFixedStep;
}

[[nodiscard]] inline constexpr Frame secToFrames(f64 sec) noexcept {
  return static_cast<Frame>(sec * kFixedHz + .5f);
}

inline constexpr f64 operator"" _frames(unsigned long long f) {
  return framesToSec(static_cast<Frame>(f));
}

inline constexpr f64 operator"" _frames(long double f) {
  return static_cast<f64>(f) * kFixedStep;
}

inline constexpr Frame operator"" _sec_frames(long double s) {
  return secToFrames(static_cast<f64>(s));
}

struct FramePacket {
  Frame frame{0};
  f64 lag{.0};
  f64 delta{.0};
  const f64 step{kFixedStep};
  f64 time{.0};
  f64 alpha{.0};
};
}  // namespace rl

#endif  // ENGINE_CORE_FRAME_H_
