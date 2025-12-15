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
#include "render_submit.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/anim/anim_system.h"
#include "engine/core/color.h"
#include "engine/render/render_system.h"

namespace rl {
void submitColor(const SpriteRect& rect, const Trs& trs, const DrawParams& p) {
  RL_RENDERSYS.queue()->submit({
      .flags = p.flags,
      .layer = p.layer,
      .priority = p.priority,
      .sortYType = p.sortYType,
      .tint = p.baseTint,
      .tex = kInvalidResourceId,
      .sprite =
          {
              .rect = rect,
              .offset = {.5f, .5f},
              .offsetFlipped = {.5f, .5f},
          },
      .trs = trs,
      .zIndex = p.zIndex,
  });
}

void submitSprite(const TextureId& tex, const Sprite& sprite, const Trs& trs,
                  const DrawParams& p) {
  if (!tex) return;

  RL_RENDERSYS.queue()->submit({
      .flags = p.flags,
      .layer = p.layer,
      .priority = p.priority,
      .sortYType = p.sortYType,
      .tint = p.baseTint,
      .tex = tex,
      .sprite = sprite,
      .trs = trs,
      .zIndex = p.zIndex,
  });
}

void submitSprite(const TextureId& tex, const Sprite& sprite, const Trs& trs,
                  const DrawParams& p, Rgba mod) {
  if (!tex) return;

  RL_RENDERSYS.queue()->submit({
      .flags = p.flags,
      .layer = p.layer,
      .priority = p.priority,
      .sortYType = p.sortYType,
      .tint = mixMultiply(p.baseTint, mod),
      .tex = tex,
      .sprite = sprite,
      .trs = trs,
      .zIndex = p.zIndex,
  });
}

void submitAnimator(AnimatorHandle h, const Trs& trs, DrawParams p) {
  if (!h) return;
  AnimatorFrame f;
  if (!RL_ANIMSYS.resolveFrame(h, f)) return;

  if (f.flipped) {
    p.flags |= kDrawFlagBitsFlippedX;
  }

  if (f.colorMod == kRgbaWhite) {
    submitSprite(f.tex, f.sprite, trs, p);
  } else {
    submitSprite(f.tex, f.sprite, trs, p, f.colorMod);
  }
}
}  // namespace rl