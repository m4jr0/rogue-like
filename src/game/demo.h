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

#ifndef ENGINE_GAME_DEMO_H_
#define ENGINE_GAME_DEMO_H_

#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/physics/anim_collider_sync.h"
#include "engine/resource/resource_type.h"
#include "engine/sound/sound.h"
#include "engine/sprite/sprite.h"
#include "game/character/character.h"
#include "game/world/tile_set.h"

namespace rl {
struct DemoData {
  u64 seed{0};

  AtlasId backgroundAtlas{kInvalidResourceId};
  AtlasId trapAtlas{kInvalidResourceId};

  TileSetHandle backgroundSet{kInvalidHandle};
  TileSetHandle interactiveSet{kInvalidHandle};

  AnimColliderRigHandle peaksColRig{kInvalidHandle};

  CharHandle adventurer{kInvalidHandle};
  CharHandle swordsman{kInvalidHandle};
  CharHandle beholder{kInvalidHandle};

  PlayerHandle player2{kInvalidHandle};
};

namespace internal {
void loadBackgroundTilesets(DemoData& data);
void loadInteractiveTilesets(DemoData& data);
void loadPlayers(DemoData& data);
void loadCharacters(DemoData& data);

void unloadCharacters(DemoData& data);
void unloadPlayers(DemoData& data);
void unloadInteractiveTilesets(DemoData& data);
void unloadBackgroundTilesets(DemoData& data);

void loadDemo(DemoData& data);
void unloadDemo(DemoData& data);
}  // namespace internal
}  // namespace rl

#endif  // ENGINE_GAME_DEMO_H_
