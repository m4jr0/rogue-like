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

#ifndef ENGINE_CORE_PHASE_BUS_H_
#define ENGINE_CORE_PHASE_BUS_H_

#include "engine/common.h"
#include "engine/core/frame.h"

namespace rl {
enum class LifeCyclePhase { Init, Shutdown };
enum class TickPhase { FixedUpdate, Update };

class PhaseBus {
 public:
  using LifeCycleFn = std::function<void()>;
  using TickFn = std::function<void(const FramePacket&)>;

  static PhaseBus& instance();

  void init();
  void shutdown();

  void on(LifeCyclePhase phase, LifeCycleFn fn);
  void on(TickPhase phase, TickFn fn);

  void invoke(LifeCyclePhase phase);
  void invoke(TickPhase phase, const FramePacket& f);

 private:
  std::array<std::vector<LifeCycleFn>, 2> lifeCycle_{};
  std::array<std::vector<TickFn>, 2> tick_{};

  static bool isLifecycle(LifeCyclePhase p) {
    return p == LifeCyclePhase::Init || p == LifeCyclePhase::Shutdown;
  }

  static bool isTick(TickPhase p) {
    return p == TickPhase::FixedUpdate || p == TickPhase::Update;
  }
};
}  // namespace rl

#define RL_PHASEBUS (::rl::PhaseBus::instance())
#define RL_CPHASEBUS \
  (static_cast<const ::rl::PhaseBus&>(::rl::PhaseBus::instance()))

#endif  // ENGINE_CORE_PHASE_BUS_H_
