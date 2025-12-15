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
#include "surface_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/log.h"
#include "engine/core/vector.h"

namespace rl {
SurfaceSystem& SurfaceSystem::instance() {
  static SurfaceSystem inst;
  return inst;
}

void SurfaceSystem::init() {
  RL_LOG_DEBUG("SurfaceSystem::init");
  constexpr auto kSurfaceCapacity = 32;
  hSurfacePool_.clear();
  hSurfacePool_.reserve(kSurfaceCapacity);
  surfaces_.reserve(kSurfaceCapacity);
}

void SurfaceSystem::shutdown() {
  RL_LOG_DEBUG("SurfaceSystem::shutdown");
  hSurfacePool_.clear();
  surfaces_.clear();
}

SurfaceHandle SurfaceSystem::generate(const SurfaceDesc&) {
  auto h = hSurfacePool_.generate();
  ensureCapacity(surfaces_, h.index);
  auto& s = surfaces_[h.index];
  s.handle = h;
  return h;
}

void SurfaceSystem::destroy(SurfaceHandle h) {
  auto* s = surface(h);
  *s = {};
  hSurfacePool_.destroy(h);
}

Surface* SurfaceSystem::surface(SurfaceHandle h) {
  RL_ASSERT(h && hSurfacePool_.alive(h),
            "SurfaceSystem::surface: Invalid surface handle provided!");
  if (!h || !hSurfacePool_.alive(h)) return nullptr;
  return &surfaces_[h.index];
}

const Surface* SurfaceSystem::surface(SurfaceHandle h) const {
  RL_ASSERT(h && hSurfacePool_.alive(h),
            "SurfaceSystem::surface: Invalid surface handle provided!");
  if (!h || !hSurfacePool_.alive(h)) return nullptr;
  return &surfaces_[h.index];
}
}  // namespace rl