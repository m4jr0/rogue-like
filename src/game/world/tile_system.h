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

#ifndef GAME_WORLD_TILE_SYSTEM_H_
#define GAME_WORLD_TILE_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/core/handle.h"
#include "game/world/tile_set.h"

namespace rl {
class TileSystem {
 public:
  static TileSystem& instance();

  void init();
  void shutdown();
  void update(const FramePacket&);

  [[nodiscard]] TileSetHandle generate(const TileSetDesc& desc);
  void destroy(TileSetHandle h);

  TileSet* tileset(TileSetHandle h);
  const TileSet* tileset(TileSetHandle h) const;

 private:
  HandlePool<TileSetTag> hSetPool_{};
  std::vector<TileSet> sets_{};

  TileSystem() = default;
};
}  // namespace rl

#define RL_TILESYS (::rl::TileSystem::instance())
#define RL_CTILESYS \
  (static_cast<const ::rl::TileSystem&>(::rl::TileSystem::instance()))

#endif  // GAME_WORLD_TILE_SYSTEM_H_
