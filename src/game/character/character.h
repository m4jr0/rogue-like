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

#ifndef GAME_CHARACTER_CHARACTER_H_
#define GAME_CHARACTER_CHARACTER_H_

#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/physics/anim_collider_sync.h"
#include "engine/physics/collider_resource.h"
#include "engine/physics/physics.h"
#include "engine/physics/physics_body.h"
#include "engine/resource/resource_type.h"
#include "engine/transform/transform.h"
#include "game/ability/ability_runtime.h"
#include "game/character/character_action.h"
#include "game/character/character_anim.h"
#include "game/character/character_archetype.h"
#include "game/character/character_archetype_resource.h"
#include "game/character/character_fsm.h"

namespace rl {
enum class CharKind { Unknown = 0, Player, Monster, Npc };

struct CharDesc {
  CharKind kind{CharKind::Unknown};
  CharArchetypeId archetype{kInvalidResourceId};
  CharAction action{};
  Position pos{};
  Scale scale{1.0f, 1.0f};
  Rotation rot{.0f};
};

struct CharTag {};
using CharHandle = Handle<CharTag>;

struct CharAbilities {
  std::array<const AbilityResource*, kCharAbilitySlots> slots{nullptr};
  std::array<AbilityInst, kCharAbilitySlots> inst{};

  f32 gcd{.0f};
  f32 gcdMax{.0f};

  CharAbilitySlot slotCount{0};
  CharAbilitySlot activeSlot{kInvalidAbilitySlot};
};

struct CharCollision {
  AnimColliderProfileId profileId{kInvalidResourceId};

  static constexpr BoxIndex kMaxHurt = 5;
  static constexpr BoxIndex kMaxHit = 5;

  std::array<HurtboxHandle, kMaxHurt> hurtHandles{};
  std::array<HitboxHandle, kMaxHit> hitHandles{};

  BoxIndex hurtCount{0};
  BoxIndex hitCount{0};
};

struct Char {
  CharHandle handle{kInvalidHandle};
  CharKind kind{CharKind::Unknown};
  CharArchetypeId archetype{kInvalidResourceId};
  CharAction action{};
  CardinalDir dir{CardinalDir::Unset};
  TransformHandle trans{kInvalidHandle};
  PhysicsBodyHandle body{kInvalidHandle};
  AnimColliderRigHandle animColRig{kInvalidHandle};
  CharAnims anim{};
  CharFsm fsm{};
  CharAbilities abilities{};
  CharCollision collision{};
};
}  // namespace rl

#endif  // GAME_CHARACTER_CHARACTER_H_
