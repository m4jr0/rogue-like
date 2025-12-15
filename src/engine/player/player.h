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

#ifndef ENGINE_PLAYER_PLAYER_H_
#define ENGINE_PLAYER_PLAYER_H_

#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/input/input.h"

namespace rl {
constexpr auto kMaxPlayerCount = static_cast<u8>(-1) / 2;

struct PlayerDesc {
  bool tryFindGamepad{false};
};

struct PlayerTag {};
using PlayerHandle = Handle<PlayerTag>;

using PlayerSlot = u8;
constexpr auto kInvalidPlayerSlot = static_cast<PlayerSlot>(-1);

struct Player {
  PlayerSlot slot{kInvalidPlayerSlot};
  PlayerHandle handle{kInvalidHandle};
};

namespace internal {
struct PlayerControllerRouting {
  std::vector<InputControllerId> playerToController;
  std::unordered_map<InputControllerId, PlayerHandle> controllerToPlayer;
};
}  // namespace internal
}  // namespace rl

#endif  // ENGINE_PLAYER_PLAYER_H_
