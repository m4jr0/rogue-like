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

#ifndef GAME_ABILITY_ABILITY_RUNTIME_H_
#define GAME_ABILITY_ABILITY_RUNTIME_H_

#include "engine/common.h"
#include "game/ability/ability_program.h"

namespace rl {
using AbilityInstFlags = u32;

enum AbilityInstFlagBits : AbilityInstFlags {
  kAbilityInstFlagBitsNone = 0x0,
  kAbilityInstFlagBitsInUse = 0x1,
  kAbilityInstFlagBitsImpacted = 0x2,
  kAbilityInstFlagBitsAll = static_cast<AbilityInstFlags>(-1),
};

enum class AbilityStage { Pending = 0, Start, Tick, End };

struct AbilityInst {
  f32 cdRemaining{.0f};
  AbilityStage stage{AbilityStage::Pending};
  AbilityVM vm{};
  AbilityInstFlags flags{kAbilityInstFlagBitsNone};
  f32 t{.0f};
};
}  // namespace rl

#endif  // GAME_ABILITY_ABILITY_RUNTIME_H_
