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

#ifndef ENGINE_SPRITE_SPRITE_H_
#define ENGINE_SPRITE_SPRITE_H_

#include "engine/common.h"
#include "engine/core/string.h"
#include "engine/math/vec2.h"
#include "engine/resource/resource_type.h"
#include "engine/texture/texture_resource.h"

namespace rl {
using SpriteSizeUnit = s32;
using SpritePosition = Vec2<SpriteSizeUnit>;
using SpriteExtent = Vec2<SpriteSizeUnit>;

using SpriteSizeNormUnit = f32;
using SpritePositionNorm = Vec2<SpriteSizeNormUnit>;
using SpriteExtentNorm = Vec2<SpriteSizeNormUnit>;

using SpriteIndex = u16;
constexpr auto kInvalidSpriteIndex = static_cast<SpriteIndex>(-1);

struct SpriteRect {
  SpritePosition pos;
  SpriteExtent size;
};

struct SpriteDesc {
  SpriteRect rect{};
  SpriteRect sourceRect{};
  SpriteExtent sourceSize{};
};

struct Sprite {
  SpriteRect rect{};
  SpritePositionNorm offset{};
  SpritePositionNorm offsetFlipped{};
  SpriteExtent sourceSize{};
};

struct AtlasTag {};
using AtlasId = ResourceId<AtlasTag>;

struct AtlasResource {
  using Desc = std::pair<std::string_view, Sprite>;

  inline constexpr static ResourceVersion kVersion{1};

  AtlasId id{kInvalidResourceId};
  TextureId tex{kInvalidResourceId};
  StringMap<SpriteIndex> nameToIdx{};
  std::vector<Sprite> sprites;

  void rebuild(std::initializer_list<Desc> descs) {
    rebuildImpl(descs.begin(), descs.end(), descs.size());
  }

  void rebuild(std::span<const Desc> descs) {
    rebuildImpl(descs.begin(), descs.end(), descs.size());
  }

  template <usize N>
  void rebuild(const std::array<Desc, N>& arr) {
    rebuild(std::span<const Desc>{arr});
  }

  usize add(std::string_view name, const Sprite& r) {
    auto idx = static_cast<SpriteIndex>(sprites.size());
    sprites.push_back(r);
    nameToIdx.emplace(name, idx);
    return idx;
  }

  const Sprite* at(std::string_view name) const {
    if (auto it = nameToIdx.find(name); it != nameToIdx.cend()) {
      return &sprites[it->second];
    }

    return nullptr;
  }

  const Sprite* at(SpriteIndex idx) const {
    RL_ASSERT(idx < sprites.size(),
              "Sprite::at: index provided on atlas: ", idx, "!");
    if (idx >= sprites.size()) return nullptr;
    return &sprites[idx];
  }

 private:
  template <class It>
  void rebuildImpl(It first, It last, usize count) {
    sprites.clear();
    nameToIdx.clear();
    if (count == 0) return;

    sprites.reserve(count);
    nameToIdx.reserve(count);

    for (; first != last; ++first) {
      const auto& [name, r] = *first;
      add(name, r);
    }
  }
};

enum class PivotPreset : u8 {
  TopLeft,
  TopCenter,
  TopRight,
  CenterLeft,
  Center,
  CenterRight,
  BottomLeft,
  BottomCenter,
  BottomRight,
  CustomPixels,
  CustomNormalized,
};

struct PivotDesc {
  PivotPreset preset{PivotPreset::BottomCenter};

  union {
    struct {
      SpriteSizeUnit x;
      SpriteSizeUnit y;
    } custom;

    struct {
      f32 x;
      f32 y;
    } norm;
  };
};
}  // namespace rl

#endif  // ENGINE_SPRITE_SPRITE_H_
