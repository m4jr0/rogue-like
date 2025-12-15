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
#include "time_system.h"
////////////////////////////////////////////////////////////////////////////////

namespace rl {
TimeSystem& TimeSystem::instance() {
  static TimeSystem inst;
  return inst;
}

void TimeSystem::init() {
  RL_LOG_DEBUG("TimeSystem::init");
  startTime_ = std::chrono::steady_clock::now();
  lastTick_ = startTime_;
  delta_ = .0;
  total_ = .0;
}

void TimeSystem::shutdown() {
  RL_LOG_DEBUG("TimeSystem::shutdown");
  startTime_ = {};
  lastTick_ = {};
  delta_ = .0;
  total_ = .0;
}

void TimeSystem::update(FramePacket& f) {
  auto now = std::chrono::steady_clock::now();
  delta_ = std::chrono::duration<f64>(now - lastTick_).count();
  lastTick_ = now;
  total_ += delta_;

  f.delta = delta_;
  f.time = total_;
}
}  // namespace rl