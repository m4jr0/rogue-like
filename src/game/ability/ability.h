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

#ifndef GAME_ABILITY_ABILITY_H_
#define GAME_ABILITY_ABILITY_H_

#include "engine/common.h"
#include "engine/physics/collider.h"

namespace rl {
using AbilityFlags = u32;

enum AbilityFlagBits : AbilityFlags {
  kAbilityFlagBitsNone = 0x0,
  kAbilityFlagBitsLockMovement = 0x1,
  kAbilityFlagBitsUninterrupt = 0x2,
  kAbilityFlagBitsAll = static_cast<AbilityFlags>(-1),
};

struct AbilityCost {
  f32 stamina{.0f};
  f32 mana{.0f};
};

struct AbilityTag {};

enum class HitShapeKind : u8 { Snapshot, Transform };

struct HitShapeByDir {
  Collider collider{};
};

struct HitProfile {
  HitShapeKind kind{HitShapeKind::Snapshot};
  CollisionFilter filter{};
  f64 duration{.1};
  std::array<HitShapeByDir, 4> byDir;
};
}  // namespace rl

#endif  // GAME_ABILITY_ABILITY_H_
