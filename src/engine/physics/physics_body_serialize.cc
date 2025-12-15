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
#include "physics_body_serialize.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/physics/collider_serialize.h"

namespace rl {
void writePhysicsBodyDesc(std::ostream& os, const PhysicsBodyDesc& p) {
  writePod(os, p.dynamic);

  writePod(os, p.initVel.x);
  writePod(os, p.initVel.y);

  writePod(os, p.maxVel.x);
  writePod(os, p.maxVel.y);

  writePod(os, p.acc.x);
  writePod(os, p.acc.y);

  writePod(os, p.dec.x);
  writePod(os, p.dec.y);

  writeCollider(os, p.collider);
}

void readPhysicsBodyDesc(std::istream& is, PhysicsBodyDesc& p) {
  readPod(is, p.dynamic);

  readPod(is, p.initVel.x);
  readPod(is, p.initVel.y);

  readPod(is, p.maxVel.x);
  readPod(is, p.maxVel.y);

  readPod(is, p.acc.x);
  readPod(is, p.acc.y);

  readPod(is, p.dec.x);
  readPod(is, p.dec.y);

  readCollider(is, p.collider);
}
}  // namespace rl