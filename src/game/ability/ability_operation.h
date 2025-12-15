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

#ifndef GAME_ABILITY_ABILITY_OPERATION_H_
#define GAME_ABILITY_ABILITY_OPERATION_H_

#include "engine/common.h"

namespace rl {
enum class AbilityOp : u16 {
  NoOp = 0,
  Sleep,
  PlayAnim,
  WaitAnim,
  SetAnimSpeed,
  SetGcd,
  SetCd,
  Damage,
  SpawnHitbox,
  SpawnStickyHitbox,
  Dash,
  End,
  Count,
};
}  // namespace rl

#endif  // GAME_ABILITY_ABILITY_OPERATION_H_
