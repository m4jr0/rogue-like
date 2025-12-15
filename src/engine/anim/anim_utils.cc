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

// Precompiled. ////////////////////////////////////////////////////////////////
#include "precompiled.h"
////////////////////////////////////////////////////////////////////////////////

// Header. /////////////////////////////////////////////////////////////////////
#include "anim_utils.h"
////////////////////////////////////////////////////////////////////////////////

namespace rl {
f32 repeat(f32 t, AnimDuration duration) {
  if (duration <= .0f) return .0f;
  auto r = std::fmod(t, duration);
  return (r < .0f) ? (r + duration) : r;
}

f32 oneshot(f32 t, AnimDuration duration) {
  return std::clamp(t, .0f, duration);
}

f32 pingPongRepeat(f32 t, AnimDuration duration) {
  auto period = duration * 2.0f;
  auto x = repeat(t, period);
  return (x <= duration) ? x : (period - x);
}

f32 pingPongOnce(f32 t, AnimDuration duration) {
  auto period = duration * 2.0f;
  auto x = std::clamp(t, .0f, period);
  return (x <= duration) ? x : (period - x);
}
}  // namespace rl