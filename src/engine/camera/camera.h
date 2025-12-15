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

#ifndef ENGINE_CAMERA_CAMERA_H_
#define ENGINE_CAMERA_CAMERA_H_

#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/math/trs.h"
#include "engine/math/vec2.h"
#include "engine/physics/physics.h"
#include "engine/render/render_common.h"

namespace rl {
struct CameraViewport {
  f32 zoom{1.0f};
  Vec2<WindowSize> offset{Vec2<WindowSize>::zero()};
  Vec2F32 size{Vec2F32::zero()};
};

using CameraZoom = f32;

struct CameraDesc {
  CameraZoom zoom{1.0f};
  Position pos{Position::zero()};
};

struct CameraTag {};
using CameraHandle = Handle<CameraTag>;

struct Camera {
  CameraHandle handle{kInvalidHandle};
  CameraZoom zoom{1.0f};
  CameraViewport viewport{};
  Position pos{};
  Trs viewProj;
};
}  // namespace rl

#endif  // ENGINE_CAMERA_CAMERA_H_
