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

#ifndef GAME_WORLD_TILE_MAP_H_
#define GAME_WORLD_TILE_MAP_H_

#include "engine/common.h"
#include "engine/math/vec2.h"
#include "game/render/game_render_common.h"
#include "game/world/tile.h"

namespace rl {
using TileExtent = Vec2<TileUnit>;

struct TileMapVisual {
  GameRenderLayer layer{kGameRenderLayerWorldBackground};
  GameRenderPriority priority{kGameRenderPriorityBackground};
};

struct TileMap {
  TileExtent extent{};
  TileMapVisual vis{};
  std::vector<Tile> tiles{};

  TileMap() = default;

  TileMap(TileUnit w, TileUnit h, TileMapVisual vis = {},
          TileKind fillKind = TileKind::Empty, TileVisual fillVis = {})
      : vis{vis} {
    extent.x = w;
    extent.y = h;

    auto count = extent.x * extent.y;
    tiles.assign(count, Tile{fillKind, fillVis});
  }

  bool inBounds(u16 x, u16 y) const { return x < extent.x && y < extent.y; }
  const Tile& at(usize x, usize y) const { return tiles[y * extent.x + x]; }
  Tile& at(usize x, usize y) { return tiles[y * extent.x + x]; }

  bool blocks(u16 x, u16 y) const {
    return !inBounds(x, y) || !at(x, y).passable();
  }
};
}  // namespace rl

#endif  // GAME_WORLD_TILE_MAP_H_
