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
#include "grid.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/physics/collider.h"
#include "engine/physics/physics_utils.h"

namespace rl {
void SpatialGrid::build(const Position& min, const Position& max,
                        GridCellSize cs) {
  origin = min;
  cellSize = cs;

  w = std::max(static_cast<usize>(1),
               static_cast<usize>(std::ceil((max.x - min.x) / cs)));
  h = std::max(static_cast<usize>(1),
               static_cast<usize>(std::ceil((max.y - min.y) / cs)));

  cells.assign(w * h, {});
}

void SpatialGrid::clear() {
  for (auto& c : cells) {
    c.clear();
  }
}

void SpatialGrid::add(const Hitbox& hit) {
  usize x0;
  usize y0;
  usize x1;
  usize y1;

  coveredCells(aabbOf(hit.wCollider), x0, y0, x1, y1);

  for (auto y = y0; y <= y1; ++y) {
    for (auto x = x0; x <= x1; ++x) {
      auto& c = at(x, y);
      c.addHit(hit.handle);
      ++c.seenStamp;
    }
  }
}

void SpatialGrid::add(const Hurtbox& hit) {
  usize x0;
  usize y0;
  usize x1;
  usize y1;

  coveredCells(aabbOf(hit.wCollider), x0, y0, x1, y1);

  for (auto y = y0; y <= y1; ++y) {
    for (auto x = x0; x <= x1; ++x) {
      auto& c = at(x, y);
      c.addHurt(hit.handle);
      ++c.seenStamp;
    }
  }
}

void SpatialGrid::coveredCells(const Aabb& a, usize& x0, usize& y0, usize& x1,
                               usize& y1) const {
  x0 = std::clamp(toCx(a.center.x, origin.x, cellSize), static_cast<usize>(0),
                  w - 1);
  y0 = std::clamp(toCy(a.center.y, origin.y, cellSize), static_cast<usize>(0),
                  h - 1);
  x1 = std::clamp(toCx(a.center.x + a.halfExtents.x, origin.x, cellSize),
                  static_cast<usize>(0), w - 1);
  y1 = std::clamp(toCy(a.center.y + a.halfExtents.y, origin.y, cellSize),
                  static_cast<usize>(0), h - 1);
}
}  // namespace rl