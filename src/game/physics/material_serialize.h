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

#ifndef GAME_PHYSICS_MATERIAL_SERIALIZE_H_
#define GAME_PHYSICS_MATERIAL_SERIALIZE_H_

#include "engine/common.h"
#include "game/physics/material_resource.h"

namespace rl {
void writeMaterialResource(std::ostream& os, const MaterialResource& r);
void readMaterialResource(std::istream& is, MaterialResource& r);
}  // namespace rl

#endif  // GAME_PHYSICS_MATERIAL_SERIALIZE_H_
