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
#include "material_serialize.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/serialize.h"

namespace rl {
void writeMaterialResource(std::ostream& os, const MaterialResource& r) {
  writePod(os, r.id);
  writePod(os, r.moveScale);
  writePod(os, r.groundDamping);
  writePod(os, r.friction);
  writePod(os, r.restitution);
  writePod(os, r.airDrag);
}

void readMaterialResource(std::istream& is, MaterialResource& r) {
  readPod(is, r.id);
  readPod(is, r.moveScale);
  readPod(is, r.groundDamping);
  readPod(is, r.friction);
  readPod(is, r.restitution);
  readPod(is, r.airDrag);
}
}  // namespace rl