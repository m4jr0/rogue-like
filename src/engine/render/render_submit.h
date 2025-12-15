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

#ifndef ENGINE_RENDER_RENDER_SUBMIT_H_
#define ENGINE_RENDER_RENDER_SUBMIT_H_

#include "engine/anim/anim_runtime.h"
#include "engine/common.h"
#include "engine/math/trs.h"
#include "engine/render/render_common.h"
#include "engine/sprite/sprite.h"
#include "engine/texture/texture_resource.h"

namespace rl {
void submitColor(const SpriteRect& rect, const Trs& trs, const DrawParams& p);
void submitSprite(const TextureId& tex, const Sprite& sprite, const Trs& trs,
                  const DrawParams& p);
void submitSprite(const TextureId& tex, const Sprite& sprite, const Trs& trs,
                  const DrawParams& p, Rgba mod);
void submitAnimator(AnimatorHandle h, const Trs& trs, DrawParams p);
}  // namespace rl

#endif  // ENGINE_RENDER_RENDER_SUBMIT_H_
