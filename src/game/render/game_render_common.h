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

#ifndef GAME_RENDER_GAME_RENDER_COMMON_H_
#define GAME_RENDER_GAME_RENDER_COMMON_H_

#include "engine/common.h"
#include "engine/render/render_common.h"

namespace rl {

enum GameRenderLayer : RenderLayer {
  kGameRenderLayerLowest = kRenderLayerLowest,
  kGameRenderLayerWorldBackground = 1,
  kGameRenderLayerWorldInteractive = 2,
  kGameRenderLayerWorld = 3,
  kGameRenderLayerUi = 4,
  kGameRenderLayerOverlay = 5,
  kGameRenderLayerTooltip = 6,
  kGameRenderLayerDebug = 7
};

enum GameRenderPriority : RenderPriority {
  kGameRenderPriorityLowest = kRenderPriorityLowest,
  kGameRenderPriorityBackground = 10,
  kGameRenderPriorityDecor = 50,
  kGameRenderPriorityInteractive = 80,
  kGameRenderPriorityChar = 89,
  kGameRenderPriorityNpc = 90,
  kGameRenderPriorityMonster = 100,
  kGameRenderPriorityPlayer = 110,
  kGameRenderPriorityUi = 150,
  kGameRenderPriorityOverlay = 200,
  kGameRenderPriorityDebug = kRenderPriorityMax,
};
}  // namespace rl

#endif  // GAME_RENDER_GAME_RENDER_COMMON_H_
