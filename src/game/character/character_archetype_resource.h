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

#ifndef GAME_CHARACTER_CHARACTER_ARCHETYPE_RESOURCE_H_
#define GAME_CHARACTER_CHARACTER_ARCHETYPE_RESOURCE_H_

#include "engine/anim/anim_resource.h"
#include "engine/anim/anim_type.h"
#include "engine/common.h"
#include "engine/physics/physics_body.h"
#include "engine/resource/resource_type.h"
#include "engine/sound/sound_resource.h"
#include "game/anim/anim_name_id.h"
#include "game/character/character_archetype.h"

namespace rl {
using CharArchetypeId = ResourceId<CharArchetypeTag>;
struct CharArchetypeResource {
  inline constexpr static ResourceVersion kVersion{1};

  CharArchetypeId id{kInvalidResourceId};
  CharType type{CharType::Unknown};
  CharFaction faction{CharFaction::Neutral};
  CharArchetypeFlags flags{kCharArchetypeFlagBitsNone};
  CharTypeId typeId{kInvalidCharTypeId};
  CharArchAbilities abilities{};

  PhysicsBodyDesc physics{};
  AnimSetId animSet{kInvalidResourceId};
  AnimTag startAnim{kAnimIdIdle};
  SoundBankId soundBank{kInvalidResourceId};
};
}  // namespace rl

#endif  // GAME_CHARACTER_CHARACTER_ARCHETYPE_RESOURCE_H_
