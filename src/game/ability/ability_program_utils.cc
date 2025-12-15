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
#include "ability_program_utils.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/physics/hitbox.h"
#include "engine/physics/hitbox_system.h"
#include "engine/physics/physics_system.h"
#include "engine/physics/physics_utils.h"
#include "game/ability/ability.h"
#include "game/ability/ability_library.h"

namespace rl {
const HitShapeByDir* resolveShape(Char& c) {
  const auto* ability = c.abilities.slots[c.abilities.activeSlot];
  if (!ability) return nullptr;
  const auto& hp = ability->hit;
  if (hp.kind != HitShapeKind::Transform) return nullptr;
  const auto* b = RL_CPHYSICSSYS.body(c.body);
  auto dir = dirToCardinalDir(b->dir);
  return &hp.byDir[static_cast<usize>(dir) - 1];
}

void tickHitbox(Char& c, AbilityVM& vm) {
  if (!vm.hitbox) return;
  auto* hit = RL_HITBOXSYS.hitbox(vm.hitbox);
  if (!hit) return;
  const auto& shape = resolveShape(c);
  hit->collider = shape->collider;
}

void destroyHitbox(AbilityVM& vm) {
  if (vm.hitbox) {
    RL_HITBOXSYS.destroy(vm.hitbox);
    vm.hitbox.clear();
  }
}

bool programStep(Char& c, AbilityVM& vm, const AbilityProgram& prog,
                 const FramePacket& f) {
  tickHitbox(c, vm);
  auto step = static_cast<f32>(f.step);

  if (vm.sleepTimer > .0f) {
    vm.sleepTimer = std::max(.0f, vm.sleepTimer - step);
    return false;
  }

  auto budget = kMaxOpsPerFrame;
  const auto& code = prog.code;

  while (budget-- > 0 && vm.pc < code.size()) {
    const auto& ins = code[vm.pc];
    auto fn = RL_ABILITYLIB.op(ins.op);
    auto ended = true;
    if (fn) ended = fn(c, vm, ins.payload, f);
    if (ins.op == AbilityOp::End) return true;
    if (ended) vm.pc++;

    if (vm.sleepTimer > .0f) {
      return false;
    }
  }

  auto finished = (vm.pc >= code.size());
  if (finished) destroyHitbox(vm);
  return finished;
}
}  // namespace rl