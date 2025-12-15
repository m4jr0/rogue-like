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

#ifndef GAME_CHARACTER_CHARACTER_SYSTEM_H_
#define GAME_CHARACTER_CHARACTER_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/core/handle.h"
#include "game/character/character.h"
#include "game/character/character_step_handler.h"

namespace rl {
using CharSystemDebugFlags = u8;

enum CharSystemDebugFlagBits : CharSystemDebugFlags {
  kCharSystemDebugFlagBitsNone = 0x0,
  kCharSystemDebugFlagBitsCharacterPos = 0x1,
  kCharSystemDebugFlagBitsAll = static_cast<CharSystemDebugFlags>(-1),
};

class CharSystem {
 public:
  static CharSystem& instance();

  void init();
  void shutdown();

  void fixedUpdate(const FramePacket& f);
  void update(const FramePacket&);

  [[nodiscard]] CharHandle generate(const CharDesc& desc);
  void destroy(CharHandle h);

  const Char* character(CharHandle h) const;
  bool exists(CharHandle h) const;

#ifdef RL_DEBUG
  void debugOn(CharSystemDebugFlags flags) { dFlags_ |= flags; }
  void debugOff(CharSystemDebugFlags flags) { dFlags_ &= ~flags; }
#endif  // RL_DEBUG

 private:
#ifdef RL_DEBUG
  CharSystemDebugFlags dFlags_{kCharSystemDebugFlagBitsNone};
#endif  // RL_DEBUG
  HandlePool<CharTag> hCharPool_{};
  std::vector<Char> chars_{};
  CharacterStepHandler stepHandler_{};

  CharSystem() = default;

  Char* character(CharHandle h);
};
}  // namespace rl

#define RL_CHARSYS (::rl::CharSystem::instance())
#define RL_CCHARSYS \
  (static_cast<const ::rl::CharSystem&>(::rl::CharSystem::instance()))

#ifdef RL_DEBUG
#define RL_CHARSYSDEBON(flags) (::rl::CharSystem::instance().debugOn(flags))
#define RL_CHARSYSDEBOFF(flags) (::rl::CharSystem::instance().debugOff(flags))
#else
#define RL_CHARSYSDEBON(cond, ...) ((void)0)
#define RL_CHARSYSDEBOFF(cond, ...) ((void)0)
#endif  // RL_DEBUG

#endif  // GAME_CHARACTER_CHARACTER_SYSTEM_H_
