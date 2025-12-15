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

#ifndef GAME_ABILITY_ABILITY_PROGRAM_H_
#define GAME_ABILITY_ABILITY_PROGRAM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/core/handle.h"
#include "engine/core/param_set.h"
#include "engine/physics/hitbox.h"
#include "game/ability/ability_operation.h"

namespace rl {
struct Char;

using AbilityPayload = ParamSet<5>;

struct AbilityInstruction {
  AbilityOp op;
  AbilityPayload payload;
};

struct AbilityProgram {
  std::vector<AbilityInstruction> code;
};

constexpr usize kMaxOpsPerFrame = 16;
using AbilityPC = u16;

using AbilityVMFlags = u32;

enum AbilityVMFlagBits : AbilityVMFlags {
  kAbilityVMFlagBitsNone = 0x0,
  kAbilityVMFlagBitsAll = static_cast<AbilityVMFlags>(-1),
};

struct AbilityVMTag {};
using AbilityVMHandle = Handle<AbilityVMTag>;

struct AbilityVM {
  bool running{false};
  AbilityVMHandle handle{kInvalidHandle};
  AbilityVMFlags flags{kAbilityVMFlagBitsNone};
  AbilityPC pc{0};
  u16 labelBase{0};
  f32 sleepTimer{0};
  u32 animEventHash{0};
  f32 cooldown{0};
  f32 gcd{0};
  HitboxHandle hitbox{kInvalidHandle};
};

using AbilityOpFn = bool (*)(Char&, AbilityVM&, const AbilityPayload&,
                             const FramePacket&);
}  // namespace rl

#endif  // GAME_ABILITY_ABILITY_PROGRAM_H_
