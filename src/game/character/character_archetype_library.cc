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
#include "character_archetype_library.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/log.h"
#include "engine/physics/anim_collider_library.h"
#include "engine/resource/resource_file.h"
#include "engine/resource/resource_type_registry.h"
#include "game/character/character_archetype_serialize.h"
#include "game/resource/game_resource.h"

namespace rl {
CharArchetypeLibrary& CharArchetypeLibrary::instance() {
  static CharArchetypeLibrary inst;
  return inst;
}

void CharArchetypeLibrary::init() {
  RL_LOG_DEBUG("CharArchetypeLibrary::init");
  RL_RESREG.on(kGameResourceTypeCharArchetype, "chararch");
  constexpr auto kCharArchetypeCapacity = 256;
  slots_.reserve(kCharArchetypeCapacity);
}

void CharArchetypeLibrary::shutdown() {
  RL_LOG_DEBUG("CharArchetypeLibrary::shutdown");
  slots_.clear();
}

const CharArchetypeResource* CharArchetypeLibrary::load(CharArchetypeId id) {
  return slots_.acquire(id, [&, id] {
    auto* res = new CharArchetypeResource{};

    auto ok = loadResourceFile(
        kGameResourceTypeCharArchetype, id, CharArchetypeResource::kVersion,
        CharArchetypeResource::kVersion,
        [&](std::istream& is) { readCharArchetypeResource(is, *res); });

    RL_FASSERT(ok,
               "CharArchetypeLibrary::load: Failed to load archetype with id: ",
               id, "!");

    return res;
  });
}

void CharArchetypeLibrary::unload(CharArchetypeId id) { slots_.release(id); }

const CharArchetypeResource* CharArchetypeLibrary::get(
    CharArchetypeId id) const {
  return slots_.get(id);
}
}  // namespace rl