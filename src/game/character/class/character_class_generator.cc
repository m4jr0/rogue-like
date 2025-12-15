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
#include "character_class_generator.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/anim/anim_system.h"
#include "engine/input/action_system.h"
#include "engine/physics/anim_collider_sync_system.h"
#include "engine/physics/physics_system.h"
#include "engine/sound/sound_library.h"
#include "engine/transform/transform_system.h"
#include "game/character/class/character_class_default_bindings.h"

namespace rl {
namespace internal {
void generateFsmCommon(Char& c, const CharArchetypeResource*) {
  c.fsm.states.clear();
  c.fsm.states.resize(5);
  c.fsm.states[kCharStateIdle] = {&onEnterIdle, &onUpdateIdle, nullptr};
  c.fsm.states[kCharStateMove] = {&onEnterMove, &onUpdateMove, &onExitMove};
  c.fsm.states[kCharStateAbilityRun] = {&onEnterAbility, &onUpdateAbility,
                                        &onExitAbility};
  c.fsm.states[kCharStateHurt] = {};
  c.fsm.states[kCharStateDeath] = {};

  auto& tr = c.fsm.transitions;
  tr.clear();
  tr.push_back({kCharStateIdle, kCharStateMove, kCharStateEventStartMove});
  tr.push_back({kCharStateMove, kCharStateIdle, kCharStateEventStopMove});
  tr.push_back(
      {kCharStateIdle, kCharStateAbilityRun, kCharStateEventStartAbility});
  tr.push_back(
      {kCharStateMove, kCharStateAbilityRun, kCharStateEventStartAbility});
  tr.push_back(
      {kCharStateAbilityRun, kCharStateIdle, kCharStateEventAbilityDone});

  c.fsm.current = kCharStateIdle;

  if (c.fsm.states[kCharStateIdle].onEnter) {
    c.fsm.states[kCharStateIdle].onEnter(c);
  }
}

void generateAnimatorCommon(Char& c, const CharArchetypeResource* arch) {
  AnimatorDesc desc;
  desc.animSet = arch->animSet;
  desc.anim = arch->startAnim;
  desc.colorMods = {};
  c.anim.animator = RL_ANIMSYS.generate(desc);
}

void generateInputCommon(Char& c, const CharArchetypeResource*) {
  if (c.action.control != CharControlKind::Player) return;
  auto scope = c.action.data.player.scope;
  auto bindings = defaultBindings(playerSlotFromActionScope(scope));
  RL_ACTIONSYS.set(ActionSet{.scope = scope, .bindings = bindings});
  RL_ACTIONSYS.enable(scope);
}

void generateSoundsCommon(Char&, const CharArchetypeResource* arch) {
  if (arch->soundBank) {
    RL_SOUNDLIB.loadBank(arch->soundBank);
  }
}

void generateAnimColRigCommon(Char& c, const CharArchetypeResource*) {
  c.animColRig = RL_ANIMCOLSYNCSYS.generate({
      .ref =
          {
              .type = SpatialRefType::Transform,
              .trans = c.trans,
          },
      .animator = c.anim.animator,
  });
}

void destroyFsmCommon(Char& c, const CharArchetypeResource*) {
  if (c.body) RL_PHYSICSSYS.destroy(c.body);
  if (c.trans) RL_TRANSSYS.destroy(c.trans);
}

void destroyAnimatorCommon(Char& c, const CharArchetypeResource*) {
  if (c.anim.animator) RL_ANIMSYS.destroy(c.anim.animator);
}

void destroyInputCommon(Char& c, const CharArchetypeResource*) {
  RL_ACTIONSYS.unset(c.action.data.player.scope);
}
void destroySoundsCommon(Char&, const CharArchetypeResource* arch) {
  if (arch->soundBank) {
    RL_SOUNDLIB.unloadBank(arch->soundBank);
  }
}

void destroyAnimColRigCommon(Char& c, const CharArchetypeResource*) {
  if (c.animColRig) {
    RL_ANIMCOLSYNCSYS.destroy(c.animColRig);
  }
}
}  // namespace internal

void generateCommon(Char& c, const CharArchetypeResource* arch) {
  internal::generateFsmCommon(c, arch);
  internal::generateAnimatorCommon(c, arch);
  internal::generateInputCommon(c, arch);
  internal::generateSoundsCommon(c, arch);
  internal::generateAnimColRigCommon(c, arch);
}

void destroyCommon(Char& c, const CharArchetypeResource* arch) {
  internal::destroyFsmCommon(c, arch);
  internal::destroyAnimatorCommon(c, arch);
  internal::destroyInputCommon(c, arch);
  internal::destroySoundsCommon(c, arch);
  internal::destroyAnimColRigCommon(c, arch);
}
}  // namespace rl