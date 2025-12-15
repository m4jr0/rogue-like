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
#include "character_factory.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/anim/anim_system.h"
#include "engine/anim/anim_type.h"
#include "engine/core/value_utils.h"
#include "engine/physics/hitbox.h"
#include "engine/physics/physics_system.h"
#include "engine/transform/transform.h"
#include "engine/transform/transform_system.h"
#include "game/ability/ability_library.h"
#include "game/character/character_archetype.h"
#include "game/character/character_archetype_library.h"
#include "game/character/class/character_class_generator.h"

namespace rl {
CharFactory& CharFactory::instance() {
  static CharFactory inst;
  return inst;
}

void CharFactory::init() {
  RL_LOG_DEBUG("CharFactory::init");
  generateTable();
}

void CharFactory::shutdown() {
  RL_LOG_DEBUG("CharFactory::shutdown");
  table_.clear();
}
bool CharFactory::populate(Char& c, const CharDesc& desc) const {
  c.kind = desc.kind;
  c.action = desc.action;
  c.archetype = desc.archetype;

  const CharArchetypeResource* arch = nullptr;

  if (desc.archetype) {
    arch = RL_CHARARCHLIB.load(desc.archetype);
    if (!arch) return false;
  } else {
    return false;
  }

  c.trans = RL_TRANSSYS.generate({
      .pos = desc.pos,
      .rot = desc.rot,
      .scale = desc.scale,
  });

  auto bodyDesc = arch->physics;
  bodyDesc.trans = c.trans;
  c.body = RL_PHYSICSSYS.generate(bodyDesc);
  if (!almostZero(desc.pos)) RL_PHYSICSSYS.pos(c.body, desc.pos);

  const auto& abilities = arch->abilities;
  c.abilities.slotCount = abilities.slotCount;

  if (abilities.slotCount) {
    for (CharAbilitySlot i = 0; i < abilities.slotCount; ++i) {
      const auto& aid = abilities.slots[i];
      if (!aid) continue;
      c.abilities.slots[i] = RL_ABILITYLIB.load(aid);
      c.abilities.inst[i] = {};
    }
  }

  if (const auto* vt = get(arch->id); vt->generateChar) {
    vt->generateChar(c, arch);
  }

  if (arch->startAnim != kInvalidAnimTag) {
    RL_ANIMSYS.play(c.anim.animator, arch->startAnim);
  }

  return true;
}

void CharFactory::unpopulate(Char& c) const {
  if (c.archetype) {
    const auto* arch = RL_CHARARCHLIB.get(c.archetype);

    if (const auto* vt = get(arch->id); vt->destroyChar) {
      vt->destroyChar(c, arch);
    }

    RL_CHARARCHLIB.unload(c.archetype);
  }

  c = {};
}

const internal::CharVTable* CharFactory::get(CharArchetypeId key) const {
  const static internal::CharVTable common = {
      .generateChar = generateCommon,
      .destroyChar = destroyCommon,
  };

  auto it = table_.find(key);
  return it == table_.end() ? &common : &it->second;
}

void CharFactory::generateTable() { table_ = {}; }
}  // namespace rl