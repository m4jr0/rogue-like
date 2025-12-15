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

#ifndef GAME_SCENE_SCENE_SYSTEM_H_
#define GAME_SCENE_SCENE_SYSTEM_H_

#include "engine/common.h"

namespace rl {
class SceneSystem {
 public:
  static SceneSystem& instance();

  void init();
  void shutdown();

  void load();
  void unload();

 private:
  SceneSystem() = default;
};
}  // namespace rl

#define RL_SCENESYS (::rl::SceneSystem::instance())
#define RL_CSCENESYS \
  (static_cast<const ::rl::SceneSystem&>(::rl::SceneSystem::instance()))
#endif  // GAME_SCENE_SCENE_SYSTEM_H_
