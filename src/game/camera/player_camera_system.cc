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
#include "player_camera_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/camera/camera.h"
#include "engine/camera/camera_system.h"
#include "engine/core/value_utils.h"
#include "engine/input/input_system.h"
#include "engine/transform/transform_system.h"
#include "game/character/character_system.h"

namespace rl {
PlayerCameraSystem& PlayerCameraSystem::instance() {
  static PlayerCameraSystem inst;
  return inst;
}

void PlayerCameraSystem::init() { RL_LOG_DEBUG("PlayerCameraSystem::init"); }

void PlayerCameraSystem::shutdown() {
  RL_LOG_DEBUG("PlayerCameraSystem::shutdown");
}

void PlayerCameraSystem::fixedUpdate(const FramePacket& f) {
  // Temporary code.
  if (!followed) return;
  if (!RL_CCHARSYS.exists(followed)) return;
  const auto* c = RL_CCHARSYS.character(followed);
  if (!c || !c->trans) return;
  const auto* t = RL_CTRANSSYS.global(c->trans);
  if (!t) return;

  RL_CAMSYS.moveTo(t->pos);
  auto delta = RL_INPUTSYS.scrollDelta().y * f.step;

  if (almostZero(delta)) {
    return;
  }

  RL_CAMSYS.zoomDelta(static_cast<CameraZoom>(delta), &t->pos);
}
}  // namespace rl