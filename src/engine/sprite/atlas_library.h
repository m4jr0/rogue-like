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

#ifndef ENGINE_SPRITE_ATLAS_LIBRARY_H_
#define ENGINE_SPRITE_ATLAS_LIBRARY_H_

#include "engine/common.h"
#include "engine/resource/resource_slot.h"
#include "engine/sprite/sprite.h"

namespace rl {
class AtlasLibrary {
 public:
  static AtlasLibrary& instance();

  void init();
  void shutdown();

  const AtlasResource* load(AtlasId id);
  void unload(AtlasId id);
  const AtlasResource* get(AtlasId id) const;

 private:
  ResourceSlots<AtlasTag, AtlasResource> slots_{};

  AtlasLibrary() = default;
};
}  // namespace rl

#define RL_ATLASLIB (::rl::AtlasLibrary::instance())
#define RL_CATLASLIB \
  (static_cast<const ::rl::AtlasLibrary&>(::rl::AtlasLibrary::instance()))

#endif  // ENGINE_SPRITE_ATLAS_LIBRARY_H_
