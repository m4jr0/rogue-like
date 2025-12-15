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
#include "render_queue.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/value_utils.h"
#include "engine/math/trs.h"
#include "engine/texture/texture_library.h"

namespace rl {
void RenderQueue::nextFrame() {
  seqCounter_ = 0;
  items_.clear();
  instances_.clear();
}

void RenderQueue::clear() {
  seqCounter_ = 0;
  items_.clear();
  items_.shrink_to_fit();
  instances_.clear();
  instances_.shrink_to_fit();
}

void RenderQueue::submit(const DrawQuad& dq) {
  f32 u0 = .0f;
  f32 v0 = .0f;
  f32 u1 = 1.0f;
  f32 v1 = 1.0f;
  const auto& sRect = dq.sprite.rect;

  if (dq.tex && sRect.size.x > 0 && sRect.size.y > 0) {
    TextureExtent size;

    if (RL_TEXLIB.size(dq.tex, size)) {
      auto tw = static_cast<f32>(size.x);
      auto th = static_cast<f32>(size.y);
      // FIXME: Half-texel inset to mask atlas bleeding. This slightly crops the
      // sprite though...
      constexpr f32 kEpsilon = .5f;
      u0 = (sRect.pos.x + kEpsilon) / tw;
      v0 = (sRect.pos.y + kEpsilon) / th;
      u1 = (sRect.pos.x + sRect.size.x - kEpsilon) / tw;
      v1 = (sRect.pos.y + sRect.size.y - kEpsilon) / th;
      // Y is flipped in sprite data (currently).
      std::swap(v0, v1);
    }
  }

  SpriteSizeNormUnit offsetX;
  SpriteSizeNormUnit offsetY;

  if (dq.flags & kDrawFlagBitsFlippedX) {
    std::swap(u0, u1);
    offsetX = dq.sprite.offsetFlipped.x;
  } else {
    offsetX = dq.sprite.offset.x;
  }

  if (dq.flags & kDrawFlagBitsFlippedY) {
    std::swap(v0, v1);
    offsetY = dq.sprite.offsetFlipped.y;
  } else {
    offsetY = dq.sprite.offset.y;
  }

  const auto& trs = dq.trs;
  auto normTint = rgbaToRgbaNorm(dq.tint);

  auto sx = std::max(1, sRect.size.x);
  auto sy = std::max(1, sRect.size.y);

  const auto& T = dq.trs;
  auto rs0x = T.rs0x() * sx;
  auto rs0y = T.rs0y() * sx;
  auto rs1x = T.rs1x() * sy;
  auto rs1y = T.rs1y() * sy;

  auto tx = trs.tx() - (rs0x * offsetX + rs1x * offsetY);
  auto ty = trs.ty() - (rs0y * offsetX + rs1y * offsetY);

  f32 sortY;

  switch (dq.sortYType) {
    using enum SortYType;
    case Pivot:
      sortY = trs.ty();
      break;
    case Bottom:
      sortY = ty + std::min({.0f, rs0y, rs1y, rs0y + rs1y});
      break;
    case NoSort:
    default:
      sortY = 0;
      break;
  }

  items_.push_back({
      .layer = dq.layer,
      .priority = dq.priority,
      .tex = dq.tex,
      .inst =
          {
              .rs0x = rs0x,
              .rs0y = rs0y,
              .rs1x = rs1x,
              .rs1y = rs1y,
              .tx = tx,
              .ty = ty,
              .u0 = u0,
              .v0 = v0,
              .u1 = u1,
              .v1 = v1,
              .r = normTint.r,
              .g = normTint.g,
              .b = normTint.b,
              .a = normTint.a,
              .sortY = sortY,
              .zIndex = dq.zIndex,
              .seq = seqCounter_++,
          },
  });
}

void RenderQueue::batches(std::vector<RenderBatch>& out) {
  std::stable_sort(items_.begin(), items_.end(),
                   [](const auto& a, const auto& b) {
                     if (a.layer != b.layer) return a.layer < b.layer;

                     if (a.inst.zIndex != b.inst.zIndex) {
                       return a.inst.zIndex < b.inst.zIndex;
                     }

                     if (!nearlyEqual(a.inst.sortY, b.inst.sortY)) {
                       return a.inst.sortY > b.inst.sortY;
                     }

                     if (a.priority != b.priority) {
                       return a.priority < b.priority;
                     }

                     // By default, newest is above the other.
                     return a.inst.seq < b.inst.seq;
                   });

  instances_.clear();
  instances_.reserve(items_.size());
  for (const auto& it : items_) instances_.push_back(it.inst);

  out.clear();
  out.reserve(items_.size());
  usize offset = 0;
  auto curLayer = items_[0].layer;
  auto curPri = items_[0].priority;
  auto curTex = items_[0].tex;

  for (usize i = 1; i < items_.size(); ++i) {
    const auto& it = items_[i];
    auto breakBatch =
        (it.layer != curLayer) || (it.priority != curPri) || (it.tex != curTex);

    if (!breakBatch) {
      continue;
    }

    out.push_back({
        .tex = curTex,
        .instances =
            std::span<const DrawInstance>(&instances_[offset], i - offset),
    });

    offset = i;
    curLayer = it.layer;
    curPri = it.priority;
    curTex = it.tex;
  }

  out.push_back(RenderBatch{
      .tex = curTex,
      .instances = std::span<const DrawInstance>(&instances_[offset],
                                                 items_.size() - offset),
  });
}
}  // namespace rl