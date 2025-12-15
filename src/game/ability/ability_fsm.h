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

#ifndef GAME_ABILITY_ABILITY_FSM_H_
#define GAME_ABILITY_ABILITY_FSM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/core/fsm.h"
#include "engine/resource/resource_type.h"
#include "game/ability/ability.h"
#include "game/ability/ability_library.h"

namespace rl {
struct AbilityState : State {
  AbilityState(AbilityId id) : id{id} {}
  virtual ~AbilityState() = default;

  virtual void onEnter() {
    const auto* ability = RL_ABILITYLIB.get(id);
    if (ability->onBegin) ability->onBegin();
  }

  virtual void onExit() {
    const auto* ability = RL_ABILITYLIB.get(id);
    if (ability->onEnd) ability->onEnd();
  }

  virtual void onUpdate(const FramePacket& f) {
    const auto* ability = RL_ABILITYLIB.get(id);
    if (ability->onTick) ability->onTick(f);
  }

  AbilityId id{kInvalidResourceId};
};
}  // namespace rl

#endif  // GAME_ABILITY_ABILITY_FSM_H_
