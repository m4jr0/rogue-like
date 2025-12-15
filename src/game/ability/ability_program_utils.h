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

#ifndef GAME_ABILITY_ABILITY_PROGRAM_UTILS_H_
#define GAME_ABILITY_ABILITY_PROGRAM_UTILS_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "game/ability/ability_program.h"
#include "game/ability/ability_resource.h"
#include "game/ability/ability_runtime.h"
#include "game/character/character.h"

namespace rl {
inline const AbilityProgram* resolveProgram(const AbilityResource* res,
                                            AbilityStage stage) {
  switch (stage) {
    case AbilityStage::Start:
      return &res->onBegin;
    case AbilityStage::Tick:
      return &res->onTick;
    case AbilityStage::End:
      return &res->onEnd;
    default:
      return nullptr;
  }
}

inline void programStage(AbilityInst& inst) {
  switch (inst.stage) {
    case AbilityStage::Pending:
      inst.stage = AbilityStage::Start;
      break;
    case AbilityStage::Start:
      inst.stage = AbilityStage::Tick;
      break;
    case AbilityStage::Tick:
      inst.stage = AbilityStage::End;
      break;
    default:
    case AbilityStage::End:
      inst.stage = AbilityStage::Pending;
      break;
  }
}

const HitShapeByDir* resolveShape(Char& c);
void tickHitbox(Char& c, AbilityVM& vm);
void destroyHitbox(AbilityVM& vm);
bool programStep(Char& c, AbilityVM& vm, const AbilityProgram& prog,
                 const FramePacket& f);
}  // namespace rl

#endif  // GAME_ABILITY_ABILITY_PROGRAM_UTILS_H_
