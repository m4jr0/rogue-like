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
#include "game.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/engine.h"
#include "engine/core/param_traversal.h"
#include "engine/core/phase_bus.h"
#include "engine/event/event_system.h"
#include "engine/scene/scene_message.h"
#include "game/ability/ability_library.h"
#include "game/camera/player_camera_system.h"
#include "game/character/character_archetype_library.h"
#include "game/character/character_system.h"
#include "game/combat/combat_system.h"
#include "game/demo.h"
#include "game/physics/material_library.h"
#include "game/physics/surface_system.h"
#include "game/world/tile_system.h"

namespace rl {
static DemoData demo{};

Game& Game::instance() {
  static Game inst;
  return inst;
}

void Game::run() {
  RL_PHASEBUS.on(LifeCyclePhase::Init, [] {
    RL_MATERIALLIB.init();
    RL_SURFACESYS.init();
    RL_TILESYS.init();
    RL_PLAYCAMSYS.init();
    RL_CHARSYS.init();
    RL_COMBATSYS.init();

    RL_ABILITYLIB.init();
    RL_CHARARCHLIB.init();

    // Load demo from scene events.
    RL_EVENTSYS.on(kSceneMessageIdSceneLoaded, On);
    RL_EVENTSYS.on(kSceneMessageIdSceneUnloaded, On);
  });

  RL_PHASEBUS.on(LifeCyclePhase::Shutdown, [] {
    RL_CHARARCHLIB.shutdown();
    RL_ABILITYLIB.shutdown();

    RL_COMBATSYS.shutdown();
    RL_CHARSYS.shutdown();
    RL_PLAYCAMSYS.shutdown();
    RL_TILESYS.shutdown();
    RL_SURFACESYS.shutdown();
    RL_MATERIALLIB.shutdown();
  });

  RL_PHASEBUS.on(TickPhase::FixedUpdate, [](const FramePacket& f) {
    RL_COMBATSYS.fixedUpdate(f);
    RL_CHARSYS.fixedUpdate(f);
    RL_PLAYCAMSYS.fixedUpdate(f);
  });

  RL_PHASEBUS.on(TickPhase::Update, [](const FramePacket& f) {
    RL_CHARSYS.update(f);
    RL_TILESYS.update(f);
  });

  RL_ENGINE.init();
  RL_ENGINE.run();
  RL_ENGINE.shutdown();
}

void Game::On(const Message& m, void*) {
  switch (m.id) {
    case kSceneMessageIdSceneLoaded:
      demo.seed = 1231031;
      internal::loadDemo(demo);
      break;
    case kSceneMessageIdSceneUnloaded:
      demo.seed = 0;
      internal::unloadDemo(demo);
      break;
    default:
      break;
  }
}
}  // namespace rl