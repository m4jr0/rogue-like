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

#ifndef ENGINE_TIME_TIME_SYSTEM_H_
#define ENGINE_TIME_TIME_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/frame.h"

namespace rl {
class TimeSystem {
 public:
  static TimeSystem& instance();

  void init();
  void shutdown();

  void update(FramePacket& f);

  inline f64 delta() const { return delta_; }
  inline f64 now() const { return total_; };

 private:
  std::chrono::steady_clock::time_point startTime_{};
  std::chrono::steady_clock::time_point lastTick_{};

  f64 delta_{.0};
  f64 total_{.0};

  TimeSystem() = default;
};
}  // namespace rl

#define RL_TIMESYS (::rl::TimeSystem::instance())
#define RL_CTIMESYS \
  (static_cast<const ::rl::TimeSystem&>(::rl::TimeSystem::instance()))

#endif  // ENGINE_TIME_TIME_SYSTEM_H_
