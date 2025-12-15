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
#include "character_fsm.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/value_utils.h"
#include "engine/input/action_system.h"
#include "engine/physics/physics_system.h"
#include "game/ability/ability_program_utils.h"
#include "game/anim/anim_name_id.h"
#include "game/character/character.h"
#include "game/character/character_action.h"
#include "game/character/character_anim.h"

namespace rl {
bool tryStartAbility(Char& c) noexcept {
  auto scope = scopeOf(c);
  // TODO(m4jr0): AI.
  if (scope == kInvalidActionScope) return false;
  auto& abilities = c.abilities;

  for (CharAbilitySlot s = 0; s < abilities.slotCount; ++s) {
    if (RL_ACTIONSYS.on(scope, charInputActionFromAbilitySlot(s))) {
      abilities.activeSlot = s;
      c.fsm.signal(kCharStateEventStartAbility, c);
      return true;
    }
  }

  return false;
}

Steering handleMove(const Char& c, bool canMove) {
  auto scope = scopeOf(c);
  auto steer = canMove ? steeringFromActions(scope) : Steering{};
  RL_PHYSICSSYS.steer(c.body, steer);

  if (!almostZero(steer)) {
    RL_ANIMSYS.dir(c.anim.animator, steer);
  }

  return steer;
}

void CharFsm::signal(CharStateEventId ev, Char& c) noexcept {
  auto cur = current;

  for (const auto& t : transitions) {
    if (t.from == cur && t.ev == ev) {
      if (states[cur].onExit) states[cur].onExit(c);
      current = t.to;
      if (states[t.to].onEnter) states[t.to].onEnter(c);
      return;
    }
  }
}

void onEnterIdle(Char& c) noexcept { c.anim.play(kAnimIdIdle); }

void onUpdateIdle(Char& c, const FramePacket&) noexcept {
  auto scope = scopeOf(c);
  if (tryStartAbility(c)) return;
  auto steer = steeringFromActions(scope);

  if (steer.x != .0f || steer.y != .0f) {
    c.fsm.signal(kCharStateEventStartMove, c);
  }
}

void onEnterMove(Char& c) noexcept { c.anim.play(kAnimIdWalk); }

void onUpdateMove(Char& c, const FramePacket&) noexcept {
  if (tryStartAbility(c)) return;
  auto steer = handleMove(c, true);
  c.anim.speed(2.0f * steer.magSqrd());

  if (almostZero(steer)) {
    const auto* body = RL_CPHYSICSSYS.body(c.body);
    const auto& v = body->vel;
    if (almostZero(v)) c.fsm.signal(kCharStateEventStopMove, c);
  }
}

void onExitMove(Char& c) noexcept { c.anim.speed(1.0f); }

void onEnterAbility(Char& c) noexcept {
  auto s = c.abilities.activeSlot;
  if (s == kInvalidAbilitySlot) return;

  auto& inst = c.abilities.inst[s];
  inst.stage = AbilityStage::Pending;
  const auto* res = c.abilities.slots[s];
  if (!res) return;

  if (c.abilities.gcd > .0f || inst.cdRemaining > .0f) {
    return;
  }

  programStage(inst);
  inst.flags |= kAbilityInstFlagBitsInUse;
  inst.t = .0f;
  inst.vm = {};
  inst.vm.pc = 0;
  inst.vm.running = true;
}

void onUpdateAbility(Char& c, const FramePacket& f) noexcept {
  auto s = c.abilities.activeSlot;

  if (s == kInvalidAbilitySlot) {
    c.fsm.signal(kCharStateEventAbilityDone, c);
    return;
  }

  auto& inst = c.abilities.inst[s];
  const auto* res = c.abilities.slots[s];

  if (!res || !(inst.flags & kAbilityInstFlagBitsInUse)) {
    c.fsm.signal(kCharStateEventAbilityDone, c);
    return;
  }

  const auto* prog = resolveProgram(res, inst.stage);
  auto ended = !prog || programStep(c, inst.vm, *prog, f);
  inst.t += static_cast<f32>(f.step);
  handleMove(c, !(res->flags & kAbilityFlagBitsLockMovement));

  if (ended) {
    programStage(inst);

    if (inst.stage == AbilityStage::Pending) {
      c.fsm.signal(kCharStateEventAbilityDone, c);
    }
  }
}

void onExitAbility(Char& c) noexcept {
  auto s = c.abilities.activeSlot;

  if (s == kInvalidAbilitySlot) {
    return;
  }

  auto& inst = c.abilities.inst[s];
  inst.flags &= ~kAbilityInstFlagBitsInUse;
  c.abilities.gcd = std::max(c.abilities.gcd, inst.vm.gcd);
  inst.cdRemaining = std::max(inst.cdRemaining, inst.vm.cooldown);
}
}  // namespace rl