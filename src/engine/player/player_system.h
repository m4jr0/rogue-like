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

#ifndef ENGINE_PLAYER_PLAYER_SYSTEM_H_
#define ENGINE_PLAYER_PLAYER_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/event/message.h"
#include "engine/player/player.h"

namespace rl {
class PlayerSystem {
 public:
  static PlayerSystem& instance();

  void init();
  void shutdown();

  [[nodiscard]] PlayerHandle generate(const PlayerDesc& desc);
  void destroy(PlayerHandle h);

  const Player* player(PlayerHandle h) const;
  Player* player(PlayerHandle h);

  const Player* main() const { return player(mainPlayer_); }

  Player* main() { return player(mainPlayer_); }

  std::span<const PlayerHandle> activePlayers() const noexcept {
    return activePlayers_;
  }

  const Player* playerFromSlot(PlayerSlot s) const;
  Player* playerFromSlot(PlayerSlot s);
  PlayerSlot slot(PlayerHandle h) const;

  void tryFindGamepad(PlayerHandle h);
  void autoClaimGamepad(InputControllerId cid);
  void assignGamepad(InputControllerId cid, PlayerHandle h);
  void assignGamepadFromSlot(InputControllerId cid, PlayerSlot s);
  void unassignGamepad(InputControllerId cid);
  void unassignGamepad(PlayerHandle h);
  void unassignGamepadFromSlot(PlayerSlot s);
  void unassignAllGamepads();

  InputControllerId gamepad(PlayerHandle h) const noexcept;
  bool hasGamepad(PlayerHandle h) const noexcept;
  bool hasPlayer(InputControllerId cid) const noexcept;
  PlayerHandle playerFromCid(InputControllerId cid) const noexcept;

 private:
  PlayerHandle mainPlayer_{kInvalidHandle};
  internal::PlayerControllerRouting controllerRouting_{};
  HandlePool<PlayerTag> hPlayerPool_{};
  std::vector<Player> players_{};
  std::vector<PlayerHandle> activePlayers_{};

  static void On(const Message& m, void* userData);

  PlayerSystem() = default;
};
}  // namespace rl

#define RL_PLAYSYS (::rl::PlayerSystem::instance())
#define RL_CPLAYSYS \
  (static_cast<const ::rl::PlayerSystem&>(::rl::PlayerSystem::instance()))

#endif  // ENGINE_PLAYER_PLAYER_SYSTEM_H_
