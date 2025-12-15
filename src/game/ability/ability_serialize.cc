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
#include "ability_serialize.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/serialize.h"

namespace rl {
void writeAbilityPayload(std::ostream& os, const AbilityPayload& ps) {
  writePod(os, ps);
}

void readAbilityPayload(std::istream& is, AbilityPayload& ps) {
  readPod(is, ps);
}

void writeAbilityProgram(std::ostream& os, const AbilityProgram& prog) {
  writeVectorWith(os, prog.code,
                  [](std::ostream& os, const AbilityInstruction& inst) {
                    const u32 op = static_cast<u32>(inst.op);
                    writePod(os, op);
                    writeAbilityPayload(os, inst.payload);
                  });
}

void readAbilityProgram(std::istream& is, AbilityProgram& prog) {
  readVectorWith(is, prog.code, [](std::istream& is, AbilityInstruction& inst) {
    u32 op;
    readPod(is, op);
    inst.op = static_cast<AbilityOp>(op);
    readAbilityPayload(is, inst.payload);
  });
}

void writeAbilityResource(std::ostream& os, const AbilityResource& r) {
  writePod(os, r.id);
  writePod(os, r.cost.stamina);
  writePod(os, r.cost.mana);
  writePod(os, r.flags);
  writeAbilityProgram(os, r.onBegin);
  writeAbilityProgram(os, r.onTick);
  writeAbilityProgram(os, r.onEnd);
}

void readAbilityResource(std::istream& is, AbilityResource& r) {
  readPod(is, r.id);
  readPod(is, r.cost.stamina);
  readPod(is, r.cost.mana);
  readPod(is, r.flags);
  readAbilityProgram(is, r.onBegin);
  readAbilityProgram(is, r.onTick);
  readAbilityProgram(is, r.onEnd);
}
}  // namespace rl