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

#ifndef GAME_PHYSICS_GRID_H_
#define GAME_PHYSICS_GRID_H_

#include "engine/common.h"
#include "engine/physics/hitbox.h"
#include "engine/physics/physics.h"

namespace rl {
struct GridCell {
  std::vector<HitboxHandle> hitboxes{};
  std::vector<HurtboxHandle> hurtboxes{};
  usize seenStamp{0};

  void addHit(HitboxHandle h) { hitboxes.push_back(h); }
  void addHurt(HurtboxHandle h) { hurtboxes.push_back(h); }

  void clear() {
    hitboxes.clear();
    hurtboxes.clear();
  }
};

using GridCellSize = Distance;

class SpatialGrid {
 public:
  void build(const Position& min, const Position& max,
             GridCellSize cs = kDefaultCellSize_);
  void clear();

  void add(const Hitbox& hit);
  void add(const Hurtbox& hit);

  GridCell& at(usize cx, usize cy) { return cells[cy * w + cx]; }
  const GridCell& at(usize cx, usize cy) const { return cells[cy * w + cx]; }

  void coveredCells(const Aabb& a, usize& x0, usize& y0, usize& x1,
                    usize& y1) const;

 private:
  inline static constexpr GridCellSize kDefaultCellSize_ = 64.0;

  Position origin{Position::zero()};
  GridCellSize cellSize{kDefaultCellSize_};
  usize w{0};
  usize h{0};
  std::vector<GridCell> cells{};

  static usize toCx(Distance x, Distance originX, Distance cs) {
    return static_cast<usize>(std::floor((x - originX) / cs));
  }

  static usize toCy(Distance y, Distance originY, Distance cs) {
    return static_cast<usize>(std::floor((y - originY) / cs));
  }
};
}  // namespace rl

#endif  // GAME_PHYSICS_GRID_H_
