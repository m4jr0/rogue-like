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

#ifndef ENGINE_RENDER_RENDER_COMMON_H_
#define ENGINE_RENDER_RENDER_COMMON_H_

#include "engine/common.h"
#include "engine/core/color.h"
#include "engine/math/trs.h"
#include "engine/sprite/sprite.h"
#include "engine/texture/texture_resource.h"

namespace rl {
using RenderLayer = u8;
constexpr RenderLayer kRenderLayerLowest = 0;
constexpr auto kRenderLayerMax = static_cast<RenderLayer>(-1);
using RenderPriority = u8;
constexpr RenderPriority kRenderPriorityLowest = 0;
constexpr auto kRenderPriorityMax = static_cast<RenderPriority>(-1);

struct Vertex {
  f32 x;
  f32 y;
  f32 u;
  f32 v;
  u32 rgba;
};

using Index = u16;

enum class MeshKind : u8 { Unknown = 0, Quad, Arbitrary };

struct MeshDesc {
  MeshKind kind{MeshKind::Unknown};
  std::span<const Vertex> verts;
  std::span<const Index> indices;
};

using ScaleUnit = f32;
using Scale = Vec2<ScaleUnit>;

using WindowSize = u16;

struct Viewport {
  Vec2<WindowSize> pos{};
  Vec2<WindowSize> size{};
  Vec2<WindowSize> ref{};
};

using DrawFlags = u8;

enum DrawFlagBits : DrawFlags {
  kDrawFlagBitsNone = 0x0,
  kDrawFlagBitsFlippedX = 0x1,
  kDrawFlagBitsFlippedY = 0x2,
  kDrawFlagBitsAll = static_cast<DrawFlags>(-1),
};

using ZIndex = u16;
constexpr ZIndex kDefaultZIndex = 0;

enum class SortYType : u8 { NoSort = 0, Pivot, Bottom };

struct DrawQuad {
  DrawFlags flags{kDrawFlagBitsNone};
  RenderLayer layer{kRenderLayerLowest};
  RenderPriority priority{kRenderPriorityLowest};
  SortYType sortYType{SortYType::NoSort};
  Rgba tint{kRgbaWhite};
  TextureId tex{kInvalidResourceId};
  Sprite sprite{};
  Trs trs{Trs::identity()};
  ZIndex zIndex{kDefaultZIndex};
};

struct DrawInstance {
  // RS (Rotation/Scale).
  f32 rs0x{.0f};
  f32 rs0y{.0f};
  f32 rs1x{.0f};
  f32 rs1y{.0f};
  // T (Translation).
  f32 tx{.0f};
  f32 ty{.0f};

  f32 u0{.0f};
  f32 v0{.0f};
  f32 u1{.0f};
  f32 v1{.0f};
  f32 r{.0f};
  f32 g{.0f};
  f32 b{.0f};
  f32 a{.0f};

  f32 sortY{.0f};
  ZIndex zIndex{kDefaultZIndex};
  u32 seq{0};
};

struct RenderBatch {
  TextureId tex{kInvalidResourceId};
  std::span<const DrawInstance> instances{};
};

struct DrawParams {
  DrawFlags flags{kDrawFlagBitsNone};
  RenderLayer layer{kRenderLayerLowest};
  RenderPriority priority{kRenderPriorityLowest};
  SortYType sortYType{SortYType::NoSort};
  Rgba baseTint{kRgbaWhite};
  ZIndex zIndex{kDefaultZIndex};
};
}  // namespace rl

#endif  // ENGINE_RENDER_RENDER_COMMON_H_
