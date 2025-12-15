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
#include "material_library.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/log.h"
#include "engine/resource/resource_file.h"
#include "engine/resource/resource_type_registry.h"
#include "game/physics/material_serialize.h"
#include "game/resource/game_resource.h"

namespace rl {
MaterialLibrary& MaterialLibrary::instance() {
  static MaterialLibrary inst;
  return inst;
}

void MaterialLibrary::init() {
  RL_LOG_DEBUG("MaterialLibrary::init");
  RL_RESREG.on(kGameResourceTypeMaterial, "mat");
  constexpr auto kMaterialCapacity = 32;
  slots_.reserve(kMaterialCapacity);
}

void MaterialLibrary::shutdown() {
  RL_LOG_DEBUG("MaterialLibrary::shutdown");
  RL_RESREG.off(kGameResourceTypeMaterial);
  slots_.clear();
}

const MaterialResource* MaterialLibrary::load(MaterialId id) {
  return slots_.acquire(id, [&, id] {
    auto* res = new MaterialResource{};

    auto ok = loadResourceFile(
        kGameResourceTypeMaterial, id, MaterialResource::kVersion,
        MaterialResource::kVersion,
        [&](std::istream& is) { readMaterialResource(is, *res); });

    RL_FASSERT(
        ok,
        "MaterialLibrary::load: Failed to load material resource with id: ", id,
        "!");

    return res;
  });
}

void MaterialLibrary::unload(MaterialId id) { slots_.release(id); }

const MaterialResource* MaterialLibrary::get(MaterialId id) const {
  return slots_.get(id);
}
}  // namespace rl