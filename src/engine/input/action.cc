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
#include "action.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/player/player.h"
#include "engine/player/player_system.h"

namespace rl {
PlayerHandle fromScopeToPlayer(ActionScope scope) {
  auto pSlot = playerSlotFromActionScope(scope);
  const Player* p = RL_CPLAYSYS.playerFromSlot(pSlot);
  return p ? p->handle : kInvalidHandle;
}

InputControllerId fromScopeToController(ActionScope scope) {
  auto player = fromScopeToPlayer(scope);
  if (!player) return kInvalidInputControllerId;
  return RL_PLAYSYS.gamepad(player);
}
}  // namespace rl