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

#ifndef GAME_CHARACTER_CHARACTER_ARCHETYPE_H_
#define GAME_CHARACTER_CHARACTER_ARCHETYPE_H_

#include "engine/anim/anim_resource.h"
#include "engine/anim/anim_type.h"
#include "engine/common.h"
#include "engine/physics/physics_body.h"
#include "engine/resource/resource_type.h"
#include "engine/sound/sound_resource.h"
#include "game/ability/ability_resource.h"
#include "game/anim/anim_name_id.h"

namespace rl {
enum class CharType { Unknown = 0, Class, Species };
enum class CharFaction { Neutral, Good, Evil };

using CharTypeId = u16;
constexpr auto kInvalidCharTypeId = static_cast<CharTypeId>(0);

enum class CharClass : CharTypeId {
  Unknown = kInvalidCharTypeId,
  Adventurer,
  Swordsman
};

enum class CharSpecies : CharTypeId { Unknown = kInvalidCharTypeId };

using CharArchetypeFlags = u32;

enum CharArchetypeFlagBits : CharArchetypeFlags {
  kCharArchetypeFlagBitsNone = 0x0,
  kCharArchetypeFlagBitsAll = static_cast<CharArchetypeFlags>(-1),
};

using CharAbilitySlot = u8;
constexpr auto kInvalidAbilitySlot = static_cast<CharAbilitySlot>(-1);

constexpr CharAbilitySlot kCharAbilitySlots = 8;

struct CharArchetypeTag {};

struct CharArchAbilities {
  CharAbilitySlot slotCount{0};
  std::array<AbilityId, kCharAbilitySlots> slots{kInvalidResourceId};
};
}  // namespace rl

#endif  // GAME_CHARACTER_CHARACTER_ARCHETYPE_H_
