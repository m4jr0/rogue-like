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
#include "render_gizmo.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/math/geometry.h"
#include "engine/math/trs.h"
#include "engine/render/render_submit.h"

namespace rl {
void submitGizmoPoint(const Position& pos, const Rgba& color, ScaleUnit scale,
                      RenderLayer layer, RenderPriority priority,
                      ZIndex zIndex) {
  submitColor(
      {
          .pos = {},
          .size = {4, 4},
      },
      Trs::translation(pos.x, pos.y).scale(scale),
      {
          .flags = kDrawFlagBitsNone,
          .layer = layer,
          .priority = priority,
          .baseTint = color,
          .zIndex = zIndex,
      });
}

void submitGizmoAabb(const Aabb& wAabb, const Rgba& color, RenderLayer layer,
                     RenderPriority priority, ZIndex zIndex) {
  submitColor(
      {},
      Trs::translation(wAabb.center.x, wAabb.center.y)
          .scale(wAabb.halfExtents.x * 2.0f, wAabb.halfExtents.y * 2.0f),
      {
          .flags = kDrawFlagBitsNone,
          .layer = layer,
          .priority = priority,
          .baseTint = color,
          .zIndex = zIndex,
      });
}

void submitGizmoCircle(const Circle& wCircle, const Rgba& color,
                       RenderLayer layer, RenderPriority priority,
                       ZIndex zIndex) {
  if (almostZero(wCircle.radius)) return;
  constexpr usize kMinSegments = 20;
  constexpr usize kMaxSegments = 64;

  auto radius = wCircle.radius;
  auto segments = static_cast<usize>(radius * .25f);
  segments = std::clamp(segments, kMinSegments, kMaxSegments);
  if (segments <= 0) return;

  constexpr f32 thickness = 2.0f;
  auto kTwoPi = kPiF32 * 2.0f;
  auto step = kTwoPi / static_cast<f32>(segments);

  for (usize i = 0; i < segments; ++i) {
    auto a0 = step * static_cast<f32>(i);
    auto a1 = step * static_cast<f32>(i + 1);

    auto x0 = wCircle.center.x + std::cos(a0) * radius;
    auto y0 = wCircle.center.y + std::sin(a0) * radius;
    auto x1 = wCircle.center.x + std::cos(a1) * radius;
    auto y1 = wCircle.center.y + std::sin(a1) * radius;

    auto mx = (x0 + x1) * .5f;
    auto my = (y0 + y1) * .5f;

    auto dx = x1 - x0;
    auto dy = y1 - y0;
    auto len = std::sqrt(dx * dx + dy * dy);

    if (almostZero(len)) continue;

    auto angle = std::atan2(dy, dx);
    auto trs = Trs::identity();
    auto c = std::cos(angle);
    auto s = std::sin(angle);

    trs.rs0x(c * len);
    trs.rs0y(s * len);
    trs.rs1x(-s * thickness);
    trs.rs1y(c * thickness);
    trs.tx(mx);
    trs.ty(my);

    submitColor(
        {
            .pos = {},
            .size = {1, 1},
        },
        trs,
        {
            .flags = kDrawFlagBitsNone,
            .layer = layer,
            .priority = priority,
            .baseTint = color,
            .zIndex = zIndex,
        });
  }
}
}  // namespace rl