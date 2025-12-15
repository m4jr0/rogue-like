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

#ifndef ENGINE_CORE_FSM_H_
#define ENGINE_CORE_FSM_H_

#include "engine/common.h"
#include "engine/core/frame.h"

namespace rl {
using StateId = u32;
constexpr auto kInvalidStateId = static_cast<StateId>(-1);

using StateEventId = u32;
constexpr auto kInvalidStateEventId = static_cast<StateEventId>(-1);

struct State {
  virtual ~State() = default;
  virtual void onEnter() {}
  virtual void onExit() {}
  virtual void onUpdate(const FramePacket&) {}
};

struct Transition {
  StateId from{kInvalidStateId};
  StateId to{kInvalidStateId};
  StateEventId on{kInvalidStateEventId};
};

struct Fsm {
  StateId current{kInvalidStateId};
  std::vector<std::unique_ptr<State>> states;
  std::vector<Transition> transitions{};

  void set(StateId sid) {
    if (current == sid) return;

    if (current != kInvalidStateId) {
      states[current]->onExit();
    }

    current = sid;

    if (current != kInvalidStateId) {
      states[current]->onEnter();
    }
  }

  void update(const FramePacket& f) {
    if (current != kInvalidStateId) {
      states[current]->onUpdate(f);
    }
  }

  void signal(StateEventId eid) {
    for (const auto& t : transitions) {
      if (t.from == current && t.on == eid) {
        set(t.to);
        return;
      }
    }
  }
};
}  // namespace rl

#endif  // ENGINE_CORE_FSM_H_
