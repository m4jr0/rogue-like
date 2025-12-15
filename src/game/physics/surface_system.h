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

#ifndef GAME_PHYSICS_SURFACE_SYSTEM_H_
#define GAME_PHYSICS_SURFACE_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/handle.h"
#include "game/physics/surface.h"

namespace rl {
class SurfaceSystem {
 public:
  static SurfaceSystem& instance();

  void init();
  void shutdown();

  [[nodiscard]] SurfaceHandle generate(const SurfaceDesc&);
  void destroy(SurfaceHandle h);

  Surface* surface(SurfaceHandle h);
  const Surface* surface(SurfaceHandle h) const;

 private:
  HandlePool<SurfaceTag> hSurfacePool_{};
  std::vector<Surface> surfaces_{};

  SurfaceSystem() = default;
};
}  // namespace rl

#define RL_SURFACESYS (::rl::SurfaceSystem::instance())
#define RL_CSURFACESYS \
  (static_cast<const ::rl::SurfaceSystem&>(::rl::SurfaceSystem::instance()))

#endif  // GAME_PHYSICS_SURFACE_SYSTEM_H_
