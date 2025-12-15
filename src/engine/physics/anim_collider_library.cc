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
#include "anim_collider_library.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/log.h"
#include "engine/physics/collider_serialize.h"
#include "engine/resource/resource_file.h"
#include "engine/resource/resource_type_registry.h"

namespace rl {
AnimColliderLibrary& AnimColliderLibrary::instance() {
  static AnimColliderLibrary inst;
  return inst;
}

void AnimColliderLibrary::init() {
  RL_LOG_DEBUG("AnimColliderLibrary::init");
  RL_RESREG.on(kResourceTypeAnimColliderProfile, "animcolprofile");
  constexpr auto kCapacity = 32;
  slots_.clear();
  slots_.reserve(kCapacity);
}

void AnimColliderLibrary::shutdown() {
  RL_LOG_DEBUG("AnimColliderLibrary::shutdown");
  RL_RESREG.off(kResourceTypeAnimColliderProfile);
  slots_.clear();
}

const AnimColliderProfileResource* AnimColliderLibrary::load(
    AnimColliderProfileId id) {
  return slots_.acquire(id, [&, id] {
    auto* res = new AnimColliderProfileResource{};

    auto ok = loadResourceFile(
        kResourceTypeAnimColliderProfile, id,
        AnimColliderProfileResource::kVersion,
        AnimColliderProfileResource::kVersion,
        [&](std::istream& is) { readColliderProfileResource(is, *res); });

    RL_FASSERT(
        ok,
        "AnimColliderLibrary::load: Failed to load collider profile with id: ",
        id, "!");

    return res;
  });
}

void AnimColliderLibrary::unload(AnimColliderProfileId id) {
  slots_.release(id);
}

const AnimColliderProfileResource* AnimColliderLibrary::get(
    AnimColliderProfileId id) const {
  return slots_.get(id);
}
}  // namespace rl