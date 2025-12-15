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

#ifndef GAME_WORLD_TILE_SET_H_
#define GAME_WORLD_TILE_SET_H_

#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/transform/transform.h"
#include "game/world/tile_map.h"

namespace rl {
struct TileSetTag {};
using TileSetHandle = Handle<TileSetTag>;

struct TileSetDesc {
  Position offset{};
  TileMap map{};
};

struct TileSet {
  TileSetHandle handle{kInvalidHandle};
  Position offset{};
  TileMap map{};
};
}  // namespace rl

#endif  // GAME_WORLD_TILE_SET_H_
