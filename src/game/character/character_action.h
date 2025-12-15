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

#ifndef GAME_CHARACTER_CHARACTER_ACTION_H_
#define GAME_CHARACTER_CHARACTER_ACTION_H_

#include "engine/common.h"
#include "engine/input/action.h"
#include "engine/physics/physics.h"
#include "engine/player/player.h"
#include "game/character/character_archetype.h"

namespace rl {
struct Char;

enum class CharControlKind { Unknown = 0, Player, AI };

struct CharAction {
  CharControlKind control{CharControlKind::Unknown};

  union Data {
    struct Player {
      PlayerHandle handle;
      ActionScope scope;
    } player;

    struct Ai {
      // Future AI fields.
    } ai;
  } data;
};

enum class CharInputAction : ActionTag {
  Up = 0,
  Down,
  Left,
  Right,
  Ability0,
  Ability1,
  Ability2,
  Ability3,
  Ability4,
  Ability5,
  Ability6,
  Ability7,
  MoveAxis,
  Count
};

[[nodiscard]] inline constexpr CharInputAction charInputActionFromAbilitySlot(
    CharAbilitySlot s) noexcept {
  return static_cast<CharInputAction>(
      static_cast<CharAbilitySlot>(CharInputAction::Ability0) + s);
}

ActionScope scopeOf(const Char& c);
Steering steeringFromActions(ActionScope scope);
}  // namespace rl

#endif  // GAME_CHARACTER_CHARACTER_ACTION_H_
