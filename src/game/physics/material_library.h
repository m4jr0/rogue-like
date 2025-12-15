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

#ifndef GAME_PHYSICS_MATERIAL_LIBRARY_H_
#define GAME_PHYSICS_MATERIAL_LIBRARY_H_

#include "engine/common.h"
#include "engine/resource/resource_slot.h"
#include "game/physics/material_resource.h"

namespace rl {
class MaterialLibrary {
 public:
  static MaterialLibrary& instance();

  void init();
  void shutdown();

  const MaterialResource* load(MaterialId id);
  void unload(MaterialId id);
  const MaterialResource* get(MaterialId id) const;

 private:
  ResourceSlots<MaterialTag, MaterialResource> slots_{};

  MaterialLibrary() = default;
};
}  // namespace rl

#define RL_MATERIALLIB (::rl::MaterialLibrary::instance())
#define RL_CMATERIALLIB \
  (static_cast<const ::rl::MaterialLibrary&>(::rl::MaterialLibrary::instance()))

#endif  // GAME_PHYSICS_MATERIAL_LIBRARY_H_
