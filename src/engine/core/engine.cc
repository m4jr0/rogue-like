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
#include "engine.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/anim/anim_library.h"
#include "engine/anim/anim_system.h"
#include "engine/camera/camera_system.h"
#include "engine/core/core_message.h"
#include "engine/core/log.h"
#include "engine/core/phase_bus.h"
#include "engine/event/event_system.h"
#include "engine/input/action_system.h"
#include "engine/input/input_system.h"
#include "engine/physics/anim_collider_library.h"
#include "engine/physics/anim_collider_sync_system.h"
#include "engine/physics/hitbox_system.h"
#include "engine/physics/physics_system.h"
#include "engine/player/player_system.h"
#include "engine/render/render_system.h"
#include "engine/resource/resource_table.h"
#include "engine/resource/resource_type_registry.h"
#include "engine/scene/scene_system.h"
#include "engine/sound/sound_library.h"
#include "engine/sound/sound_system.h"
#include "engine/sprite/atlas_library.h"
#include "engine/texture/texture_library.h"
#include "engine/time/time_system.h"
#include "engine/transform/transform_system.h"

namespace rl {
Engine& Engine::instance() {
  static Engine inst;
  return inst;
}

void Engine::init() {
  RL_LOG_INFO("Engine::init");

  RL_TIMESYS.init();
  RL_EVENTSYS.init();
  RL_SOUNDSYS.init();
  RL_RENDERSYS.init();
  RL_INPUTSYS.init(RL_RENDERSYS.device()->window());
  RL_ACTIONSYS.init();
  RL_TRANSSYS.init();
  RL_PHYSICSSYS.init();
  RL_HITBOXSYS.init();
  RL_ANIMCOLSYNCSYS.init();
  RL_CAMSYS.init();
  RL_ANIMSYS.init();
  RL_SCENESYS.init();
  RL_PLAYSYS.init();

  RL_RESREG.init();
  RL_RESTAB.init();

  RL_TEXLIB.init();
  RL_ATLASLIB.init();
  RL_ANIMCOLLIB.init();
  RL_ANIMLIB.init();
  RL_SOUNDLIB.init();

  RL_EVENTSYS.on(kCoreMessageIdQuit, On, this);
  RL_PHASEBUS.invoke(LifeCyclePhase::Init);
  shouldExit_ = false;
}

void Engine::shutdown() {
  RL_LOG_INFO("Engine::shutdown");
  RL_PHASEBUS.invoke(LifeCyclePhase::Shutdown);

  RL_SOUNDLIB.shutdown();
  RL_ANIMLIB.shutdown();
  RL_ANIMCOLLIB.shutdown();
  RL_ATLASLIB.shutdown();
  RL_TEXLIB.shutdown();

  RL_RESTAB.shutdown();
  RL_RESREG.shutdown();

  RL_PLAYSYS.shutdown();
  RL_SCENESYS.shutdown();
  RL_ANIMSYS.shutdown();
  RL_CAMSYS.shutdown();
  RL_ANIMCOLSYNCSYS.shutdown();
  RL_HITBOXSYS.shutdown();
  RL_PHYSICSSYS.shutdown();
  RL_TRANSSYS.shutdown();
  RL_ACTIONSYS.shutdown();
  RL_INPUTSYS.shutdown();
  RL_RENDERSYS.shutdown();
  RL_SOUNDSYS.shutdown();
  RL_EVENTSYS.shutdown();
  RL_TIMESYS.shutdown();
  shouldExit_ = true;
}

void Engine::run() {
  RL_LOG_DEBUG("Engine::run");
  RL_SCENESYS.load();
  FramePacket f{};

  while (!shouldExit_) {
    RL_TIMESYS.update(f);
    RL_INPUTSYS.poll();
    RL_PHYSICSSYS.update(f);

    f.alpha = f.step > .0 ? f.lag / f.step : .0;
    RL_ENGINEDEB();
    RL_RENDERSYS.update(f);
    ++f.frame;
  }

  RL_SCENESYS.unload();
  RL_EVENTSYS.flush();
}

#ifdef RL_DEBUG
void Engine::debug() {
  RL_PHYSICSDEB();
  RL_HITBOXDEB();
}
#endif  // RL_DEBUG

void Engine::On(const Message& m, void* userData) {
  auto* e = static_cast<Engine*>(userData);

  switch (m.id) {
    case kCoreMessageIdQuit:
      e->shouldExit_ = true;
      break;
    default:
      break;
  }
}
}  // namespace rl