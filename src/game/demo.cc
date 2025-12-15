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
#include "demo.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/anim/anim_system.h"
#include "engine/core/random.h"
#include "engine/physics/anim_collider_sync_system.h"
#include "engine/player/player_system.h"
#include "engine/resource/resource_table.h"
#include "engine/sprite/atlas_library.h"
#include "engine/transform/transform.h"
#include "game/action_scope.h"
#include "game/camera/player_camera_system.h"
#include "game/character/character_system.h"
#include "game/render/game_render_common.h"
#include "game/world/tile_system.h"

namespace rl {
namespace internal {
void loadBackgroundTilesets(DemoData& data) {
  constexpr TileUnit kTileWidth = 16;
  constexpr TileUnit kTileHeight = 16;
  const auto* atlas =
      RL_ATLASLIB.load(RL_CRESTAB.rid<AtlasId>("atlas.background"));
  if (!atlas) return;
  data.backgroundAtlas = atlas->id;
  auto fillRect = *atlas->at("ground0");

  TileMap m = {kTileWidth,
               kTileHeight,
               {},
               TileKind::Floor,
               TileVisual{.kind = TileVisualKind::Sprite,
                          .tint = kRgbaWhite,
                          .tex = atlas->tex,
                          .sprite = fillRect}};

  std::array grounds = {"ground0", "ground1", "ground2"};
  usize i = 0;
  auto gen = Splitmix64(data.seed);

  for (auto& t : m.tiles) {
    if (t.kind != TileKind::Floor || t.vis.kind != TileVisualKind::Sprite) {
      ++i;
      continue;
    }

    auto idx = gen.next<u32>(0, static_cast<u32>(grounds.size() - 1));
    t.vis.sprite = *atlas->at(grounds[idx]);
    ++i;
  }

  data.backgroundSet = RL_TILESYS.generate({{42.0f, 42.0f}, m});
}

void loadInteractiveTilesets(DemoData& data) {
  constexpr TileUnit kTileWidth = 16;
  constexpr TileUnit kTileHeight = 16;
  const auto* atlas = RL_ATLASLIB.load(RL_CRESTAB.rid<AtlasId>("atlas.traps"));
  if (!atlas) return;
  data.trapAtlas = atlas->id;

  TileMap m = {kTileWidth,
               kTileHeight,
               {.layer = kGameRenderLayerWorldInteractive,
                .priority = kGameRenderPriorityInteractive},
               TileKind::Empty,
               TileVisual{}};

  auto& trap = m.at(3, 3);
  trap.kind = TileKind::Trap;
  trap.vis.kind = TileVisualKind::Animated;
  trap.vis.sprite = *atlas->at("peaks_front_000");
  trap.vis.animator = RL_ANIMSYS.generate(
      {.animSet = RL_CRESTAB.rid<AnimSetId>("animset.trap_peaks")});

  Position pos = {42.0f, 42.0f};
  data.interactiveSet = RL_TILESYS.generate({pos, m});

  data.peaksColRig = RL_ANIMCOLSYNCSYS.generate({
      .ref =
          {
              .type = SpatialRefType::Position,
              .pos = {126.0f, 126.0f},
          },
      .animator = trap.vis.animator,
  });
}

void loadPlayers(DemoData& data) { data.player2 = RL_PLAYSYS.generate({}); }

void loadCharacters(DemoData& data) {
  data.adventurer = RL_CHARSYS.generate({
      .kind = CharKind::Player,
      .archetype = RL_CRESTAB.rid<CharArchetypeId>("chararch.adventurer"),
      .action =
          {
              .control = CharControlKind::Player,
              .data =
                  {
                      .player =
                          {
                              .handle = RL_PLAYSYS.playerFromSlot(kPlayer0Slot)
                                            ->handle,
                              .scope = kPlayer0Char,
                          },
                  },
          },
      .pos = {380.0f, 10.0f},
      .scale = {2.0f, 2.0f},
  });

  RL_PLAYCAMSYS.follow(data.adventurer);

  data.swordsman = RL_CHARSYS.generate({
      .kind = CharKind::Player,
      .archetype = RL_CRESTAB.rid<CharArchetypeId>("chararch.swordsman"),
      .action =
          {
              .control = CharControlKind::Player,
              .data =
                  {
                      .player =
                          {
                              .handle = RL_PLAYSYS.playerFromSlot(kPlayer1Slot)
                                            ->handle,
                              .scope = kPlayer1Char,
                          },
                  },
          },
      .pos = {380.0f, 50.0f},
      .scale = {2.0f, 2.0f},
  });

  data.beholder = RL_CHARSYS.generate({
      .kind = CharKind::Monster,
      .archetype = RL_CRESTAB.rid<CharArchetypeId>("chararch.beholder"),
      .action =
          {
              .control = CharControlKind::AI,
              .data = {},
          },
      .pos = {256.0f, 50.0f},
      .scale = {3.0f, 3.0f},
  });
}

void unloadCharacters(DemoData& data) {
  if (data.adventurer) {
    RL_CHARSYS.destroy(data.adventurer);
    data.adventurer.clear();
  }

  if (data.swordsman) {
    RL_CHARSYS.destroy(data.swordsman);
    data.swordsman.clear();
  }

  if (data.beholder) {
    RL_CHARSYS.destroy(data.beholder);
    data.beholder.clear();
  }
}

void unloadPlayers(DemoData& data) {
  if (data.player2) {
    RL_PLAYSYS.destroy(data.player2);
    data.player2.clear();
  }
}

void unloadInteractiveTilesets(DemoData& data) {
  if (data.interactiveSet) {
    RL_TILESYS.destroy(data.interactiveSet);
    data.interactiveSet.clear();
  }

  if (data.trapAtlas) {
    RL_ATLASLIB.unload(data.trapAtlas);
  }
}

void unloadBackgroundTilesets(DemoData& data) {
  if (data.backgroundSet) {
    RL_TILESYS.destroy(data.backgroundSet);
    data.backgroundSet.clear();
  }

  if (data.backgroundAtlas) {
    RL_ATLASLIB.unload(data.backgroundAtlas);
  }
}

void loadDemo(DemoData& data) {
  loadBackgroundTilesets(data);
  loadInteractiveTilesets(data);
  loadPlayers(data);
  loadCharacters(data);
}

void unloadDemo(DemoData& data) {
  unloadCharacters(data);
  unloadPlayers(data);
  unloadInteractiveTilesets(data);
  unloadBackgroundTilesets(data);
}
}  // namespace internal
}  // namespace rl
