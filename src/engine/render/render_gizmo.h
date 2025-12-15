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

#ifndef ENGINE_RENDER_RENDER_GIZMO_H_
#define ENGINE_RENDER_RENDER_GIZMO_H_

#include "engine/common.h"
#include "engine/core/color.h"
#include "engine/physics/collider.h"
#include "engine/physics/physics.h"
#include "engine/render/render_common.h"

namespace rl {
void submitGizmoPoint(const Position& pos, const Rgba& color,
                      ScaleUnit scale = 1.0f,
                      RenderLayer layer = kRenderLayerMax,
                      RenderPriority priority = kRenderPriorityMax,
                      ZIndex zIndex = kDefaultZIndex);
void submitGizmoAabb(const Aabb& wAabb, const Rgba& color,
                     RenderLayer layer = kRenderLayerMax,
                     RenderPriority priority = kRenderPriorityMax,
                     ZIndex zIndex = kDefaultZIndex);
void submitGizmoCircle(const Circle& wCircle, const Rgba& color,
                       RenderLayer layer = kRenderLayerMax,
                       RenderPriority priority = kRenderPriorityMax,
                       ZIndex zIndex = kDefaultZIndex);
}  // namespace rl

#endif  // ENGINE_RENDER_RENDER_GIZMO_H_
