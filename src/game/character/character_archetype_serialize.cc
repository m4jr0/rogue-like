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
#include "character_archetype_serialize.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/serialize.h"
#include "engine/core/type_trait.h"
#include "engine/physics/physics_body_serialize.h"

namespace rl {
void writeCharArchAbilities(std::ostream& os, const CharArchAbilities& a) {
  writePod(os, a.slotCount);

  for (AbilityId id : a.slots) {
    writePod(os, id.id);
  }
}

void readCharArchAbilities(std::istream& is, CharArchAbilities& a) {
  readPod(is, a.slotCount);

  for (AbilityId& id : a.slots) {
    readPod(is, id.id);
    ;
  }
}

void writeCharArchetypeResource(std::ostream& os,
                                const CharArchetypeResource& r) {
  writePod(os, r.id);

  auto typeVal = toUnderlying(r.type);
  writePod(os, typeVal);

  auto factionVal = toUnderlying(r.faction);
  writePod(os, factionVal);

  writePod(os, r.flags);
  writePod(os, r.typeId);

  writeCharArchAbilities(os, r.abilities);
  writePhysicsBodyDesc(os, r.physics);

  writePod(os, r.animSet.id);
  writePod(os, r.startAnim);
  writePod(os, r.soundBank.id);
}

void readCharArchetypeResource(std::istream& is, CharArchetypeResource& r) {
  readPod(is, r.id);

  underlying<CharType> typeVal{};
  readPod(is, typeVal);
  r.type = static_cast<CharType>(typeVal);

  underlying<CharFaction> factionVal{};
  readPod(is, factionVal);
  r.faction = static_cast<CharFaction>(factionVal);

  readPod(is, r.flags);
  readPod(is, r.typeId);

  readCharArchAbilities(is, r.abilities);
  readPhysicsBodyDesc(is, r.physics);

  readPod(is, r.animSet.id);
  readPod(is, r.startAnim);
  readPod(is, r.soundBank.id);
}
}  // namespace rl