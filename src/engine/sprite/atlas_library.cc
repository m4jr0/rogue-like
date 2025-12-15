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
#include "atlas_library.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/log.h"
#include "engine/resource/resource_file.h"
#include "engine/resource/resource_type_registry.h"
#include "engine/sprite/sprite_serialize.h"
#include "engine/texture/texture_library.h"
#include "engine/texture/texture_resource.h"

namespace rl {
AtlasLibrary& AtlasLibrary::instance() {
  static AtlasLibrary inst;
  return inst;
}

void AtlasLibrary::init() {
  RL_LOG_DEBUG("AtlasLibrary::init");
  RL_RESREG.on(kResourceTypeAtlas, "atlas");
  constexpr auto kAtlasCapacity = 32;
  slots_.reserve(kAtlasCapacity);
}

void AtlasLibrary::shutdown() {
  RL_LOG_DEBUG("AtlasLibrary::shutdown");
  RL_RESREG.off(kResourceTypeAtlas);
  slots_.clear();
}

const AtlasResource* AtlasLibrary::load(AtlasId id) {
  return slots_.acquire(id, [&, id] {
    auto* res = new AtlasResource{};

    auto ok = loadResourceFile(kResourceTypeAtlas, id, AtlasResource::kVersion,
                               AtlasResource::kVersion, [&](std::istream& is) {
                                 readAtlasResource(is, *res);
                               });

    RL_FASSERT(
        ok, "AtlasLibrary::load: Failed to load atlas resource with id: ", id,
        "!");

    if (res->tex != kInvalidResourceId) {
      RL_TEXLIB.load(res->tex);
    }

    return res;
  });
}

void AtlasLibrary::unload(AtlasId id) {
  const auto* res = get(id);
  TextureId tex = kInvalidResourceId;

  if (res && res->tex) {
    tex = res->tex;
  }

  auto released = slots_.release(id);

  if (released && tex != kInvalidResourceId) {
    RL_TEXLIB.unload(tex);
  }
}

const AtlasResource* AtlasLibrary::get(AtlasId id) const {
  return slots_.get(id);
}
}  // namespace rl