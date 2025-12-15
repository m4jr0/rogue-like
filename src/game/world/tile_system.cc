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
#include "tile_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/anim/anim_system.h"
#include "engine/camera/camera_system.h"
#include "engine/core/log.h"
#include "engine/core/vector.h"
#include "engine/render/render_submit.h"
#include "engine/texture/texture_library.h"

namespace rl {
TileSystem& TileSystem::instance() {
  static TileSystem inst;
  return inst;
}

void TileSystem::init() {
  RL_LOG_DEBUG("TileSystem::init");
  constexpr auto kSetCapacity = 32;
  hSetPool_.clear();
  hSetPool_.reserve(kSetCapacity);
  sets_.reserve(kSetCapacity);
}

void TileSystem::shutdown() {
  RL_LOG_DEBUG("TileSystem::shutdown");
  hSetPool_.clear();
  sets_.clear();
}

void TileSystem::update(const FramePacket&) {
  if (sets_.empty()) return;

  for (const auto& set : sets_) {
    const auto& map = set.map;
    const auto& offset = set.offset;

    auto trs = Trs::identity();

    for (TileUnit y = 0; y < map.extent.y; ++y) {
      for (TileUnit x = 0; x < map.extent.x; ++x) {
        const Tile& t = map.at(x, y);
        const auto& v = t.vis;
        if (v.kind == TileVisualKind::None) continue;

        auto px = offset.x + x * kTileSizeF32;
        auto py = offset.y + y * kTileSizeF32;
        trs.tx(px);
        trs.ty(py);
        trs.rs0x(kTileSizeF32 / v.sprite.rect.size.x);
        trs.rs1y(kTileSizeF32 / v.sprite.rect.size.x);

        DrawParams dp{
            .layer = map.vis.layer,
            .priority = map.vis.priority,
            .sortYType = SortYType::Bottom,
            .baseTint = v.tint,
        };

        switch (v.kind) {
          case TileVisualKind::SolidColor:
            submitColor({.pos = {0, 0}, .size = {kTileSize, kTileSize}}, trs,
                        dp);
            break;
          case TileVisualKind::Sprite:
            submitSprite(v.tex, v.sprite, trs, dp);
            break;
          case TileVisualKind::Animated:
            submitAnimator(v.animator, trs, dp);
            break;
          default:
            break;
        }
      }
    }
  }
}

TileSetHandle TileSystem::generate(const TileSetDesc& desc) {
  auto h = hSetPool_.generate();
  ensureCapacity(sets_, h.index);
  auto& a = sets_[h.index];
  a.handle = h;
  a.offset = desc.offset;
  a.map = desc.map;
  return h;
}

void TileSystem::destroy(TileSetHandle h) {
  auto* set = tileset(h);

  for (const auto& t : set->map.tiles) {
    if (t.vis.tex) {
      RL_TEXLIB.unload(t.vis.tex);
    }

    if (t.vis.animator) {
      RL_ANIMSYS.destroy(t.vis.animator);
    }
  }

  *set = {};
  hSetPool_.destroy(h);
}

TileSet* TileSystem::tileset(TileSetHandle h) {
  RL_ASSERT(h && hSetPool_.alive(h),
            "TileSystem::tileset: Invalid tile set handle provided!");
  if (!h || !hSetPool_.alive(h)) return nullptr;
  return &sets_[h.index];
}

const TileSet* TileSystem::tileset(TileSetHandle h) const {
  RL_ASSERT(h && hSetPool_.alive(h),
            "TileSystem::tileset: Invalid tile set handle provided!");
  if (!h || !hSetPool_.alive(h)) return nullptr;
  return &sets_[h.index];
}
}  // namespace rl