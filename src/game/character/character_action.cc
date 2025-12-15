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
#include "character_action.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/value_utils.h"
#include "engine/input/action_system.h"
#include "engine/physics/physics_utils.h"
#include "game/character/character.h"

namespace rl {
ActionScope scopeOf(const Char& c) {
  if (c.action.control == CharControlKind::Player) {
    return c.action.data.player.scope;
  }

  return kInvalidActionScope;
}

Steering steeringFromActions(ActionScope scope) {
  // TODO(m4jr0): AI.
  if (scope == kInvalidActionScope) return {};
  SteeringMask m = kSteeringMaskBitsNone;
  const auto& a = RL_ACTIONSYS;

  auto steer = a.axis(scope, CharInputAction::MoveAxis);
  if (!almostZero(steer)) return steer;

  if (a.on(scope, CharInputAction::Up)) m |= kSteeringMaskBitsUp;
  if (a.on(scope, CharInputAction::Left)) m |= kSteeringMaskBitsLeft;
  if (a.on(scope, CharInputAction::Right)) m |= kSteeringMaskBitsRight;
  if (a.on(scope, CharInputAction::Down)) m |= kSteeringMaskBitsDown;

  return steeringFromMask(m);
}
}  // namespace rl