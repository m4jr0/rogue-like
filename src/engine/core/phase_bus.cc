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
#include "phase_bus.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/type_trait.h"

namespace rl {
PhaseBus& PhaseBus::instance() {
  static PhaseBus inst;
  return inst;
}

void PhaseBus::init() {
  RL_LOG_DEBUG("PhaseBus::init");
  constexpr auto kCapacity = 16;

  for (auto& l : lifeCycle_) {
    l.clear();
    l.reserve(kCapacity);
  }

  for (auto& t : tick_) {
    t.clear();
    t.reserve(kCapacity);
  }
}

void PhaseBus::shutdown() {
  RL_LOG_DEBUG("PhaseBus::shutdown");
  for (auto& l : lifeCycle_) {
    l.clear();
  }

  for (auto& t : tick_) {
    t.clear();
  }
}

void PhaseBus::on(LifeCyclePhase phase, LifeCycleFn fn) {
  RL_ASSERT(isLifecycle(phase),
            "PhaseBus::on: VoidFn is only allowed for life cycles");
  lifeCycle_[toUnderlying(phase)].push_back(std::move(fn));
}

void PhaseBus::on(TickPhase phase, TickFn fn) {
  RL_ASSERT(isTick(phase), "PhaseBus::on: VoidFn is only allowed for ticks");
  tick_[toUnderlying(phase)].push_back(std::move(fn));
}

void PhaseBus::invoke(LifeCyclePhase phase) {
  RL_ASSERT(isLifecycle(phase),
            "PhaseBus::invoke: invoke(Phase) is for life cycle phases only");
  for (auto& l : lifeCycle_[toUnderlying(phase)]) l();
}

void PhaseBus::invoke(TickPhase phase, const FramePacket& f) {
  RL_ASSERT(
      isTick(phase),
      "PhaseBus::invoke: invoke(Phase, FramePacket&) is for tick phases only");
  for (auto& t : tick_[toUnderlying(phase)]) t(f);
}
}  // namespace rl