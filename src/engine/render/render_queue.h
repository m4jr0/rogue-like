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

#ifndef ENGINE_RENDER_RENDER_QUEUE_H_
#define ENGINE_RENDER_RENDER_QUEUE_H_

#include "engine/common.h"
#include "engine/render/render_common.h"
#include "engine/texture/texture_resource.h"

namespace rl {
class RenderQueue {
 public:
  void nextFrame();
  void clear();

  void submit(const DrawQuad& dq);

  void batches(std::vector<RenderBatch>& out);

 private:
  struct DrawItem {
    RenderLayer layer{kRenderLayerLowest};
    RenderPriority priority{kRenderPriorityLowest};
    TextureId tex{kInvalidResourceId};
    DrawInstance inst{};
  };

  u32 seqCounter_{0};
  std::vector<DrawItem> items_{};
  std::vector<DrawInstance> instances_{};
  std::vector<RenderBatch> batches_{};
};
}  // namespace rl

#endif  // ENGINE_RENDER_RENDER_QUEUE_H_
