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

#ifndef GAME_WORLD_TILE_H_
#define GAME_WORLD_TILE_H_

#include "engine/anim/anim_runtime.h"
#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/resource/resource_type.h"
#include "engine/sprite/sprite.h"
#include "engine/texture/texture_resource.h"

namespace rl {
using TileUnit = s32;
using TileCoord = Vec2<TileUnit>;
using TileSize = TileUnit;

enum class TileKind : u8 { Empty, Floor, Wall, Prop, Trap };
constexpr TileSize kTileSize = 32;
constexpr f32 kTileSizeF32 = static_cast<f32>(kTileSize);

enum class TileVisualKind : u8 { None, SolidColor, Sprite, Animated };

struct TileVisual {
  TileVisualKind kind{TileVisualKind::None};
  Rgba tint{kRgbaWhite};
  AnimatorHandle animator{kInvalidHandle};
  TextureId tex{kInvalidResourceId};
  Sprite sprite{};
};

struct Tile {
  TileKind kind{TileKind::Empty};
  TileVisual vis{};

  bool passable() const {
    return kind != TileKind::Wall && kind != TileKind::Prop;
  }
};
}  // namespace rl

#endif  // GAME_WORLD_TILE_H_
