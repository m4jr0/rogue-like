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

#ifndef GAME_COMBAT_COMBAT_SYSTEM_H_
#define GAME_COMBAT_COMBAT_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/core/handle.h"

namespace rl {
class CombatSystem {
 public:
  static CombatSystem& instance();

  void init();
  void shutdown();

  void fixedUpdate(const FramePacket&);

 private:
  CombatSystem() = default;
};
}  // namespace rl

#define RL_COMBATSYS (::rl::CombatSystem::instance())
#define RL_CCOMBATSYS \
  (static_cast<const ::rl::CombatSystem&>(::rl::CombatSystem::instance()))

#endif  // GAME_COMBAT_COMBAT_SYSTEM_H_
