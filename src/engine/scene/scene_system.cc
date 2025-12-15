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
#include "scene_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/log.h"
#include "engine/event/event_system.h"
#include "engine/scene/scene_message.h"

namespace rl {
SceneSystem& SceneSystem::instance() {
  static SceneSystem inst;
  return inst;
}

void SceneSystem::init() { RL_LOG_DEBUG("SceneSystem::init"); }

void SceneSystem::shutdown() { RL_LOG_DEBUG("SceneSystem::shutdown"); }

void SceneSystem::load() { RL_EVENTSYS.dispatch(kSceneMessageIdSceneLoaded); }

void SceneSystem::unload() {
  RL_EVENTSYS.dispatch(kSceneMessageIdSceneUnloaded);
}
}  // namespace rl