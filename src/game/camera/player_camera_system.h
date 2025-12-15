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

#ifndef GAME_CAMERA_PLAYER_CAMERA_SYSTEM_H_
#define GAME_CAMERA_PLAYER_CAMERA_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/core/handle.h"
#include "game/character/character.h"

namespace rl {
class PlayerCameraSystem {
 public:
  static PlayerCameraSystem& instance();

  void init();
  void shutdown();
  void fixedUpdate(const FramePacket& f);

  void follow(CharHandle h) { followed = h; }
  void unfollow() { followed.clear(); }

 private:
  CharHandle followed{kInvalidHandle};

  PlayerCameraSystem() = default;
};
}  // namespace rl

#define RL_PLAYCAMSYS (::rl::PlayerCameraSystem::instance())
#define RL_CPLAYCAMSYS                           \
  (static_cast<const ::rl::PlayerCameraSystem&>( \
      ::rl::PlayerCameraSystem::instance()))

#endif  // GAME_CAMERA_PLAYER_CAMERA_SYSTEM_H_
