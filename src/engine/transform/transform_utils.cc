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
#include "transform_utils.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/value_utils.h"

namespace rl {
void makeGlobalAsRoot(const Transform& l, GlobalTransform& g) {
  g.trs = Trs::trs(l.pos, l.rot, l.scale);
  g.pos = l.pos;
  g.rot = l.rot;
  g.scale = l.scale;
}

void makeGlobalFromParent(const Transform& l, const GlobalTransform& gParent,
                          GlobalTransform& g) {
  auto localTrs = Trs::trs(l.pos, l.rot, l.scale);
  g.trs = gParent.trs * localTrs;
  g.pos = {g.trs.tx(), g.trs.ty()};
  g.rot = gParent.rot + l.rot;
  g.scale = {gParent.scale.x * l.scale.x, gParent.scale.y * l.scale.y};
}

void makeLocalFromGlobal(const Trs& gParentTrs, const Trs& gChildTrs,
                         Transform& l) {
  auto parentInv = gParentTrs.inverse();
  auto localTrs = parentInv * gChildTrs;
  l.pos = {localTrs.tx(), localTrs.ty()};

  Vec2F32 basisX{localTrs.rs0x(), localTrs.rs0y()};
  Vec2F32 basisY{localTrs.rs1x(), localTrs.rs1y()};

  auto sx = basisX.mag();
  auto sy = basisY.mag();
  f32 rot = .0f;

  if (!almostZero(sx)) {
    rot = std::atan2(basisX.y, basisX.x);
  } else if (!almostZero(sy)) {
    rot = std::atan2(-basisY.x, basisY.y);
  }

  l.scale = {sx, sy};
  l.rot = rot;
  l.dirty = true;
}
}  // namespace rl