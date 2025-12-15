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

#ifndef ENGINE_TEXTURE_TEXTURE_H_
#define ENGINE_TEXTURE_TEXTURE_H_

#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/math/vec2.h"

namespace rl {
using TextureSize = u16;
using TextureExtent = Vec2<TextureSize>;

struct TextureTag {};

// Only one format is supported currently.
enum class TexFormat : u8 { Unknown = 0, Rgba8 };

using TextureHandle = Handle<TextureTag>;

struct Texture {
  TextureHandle handle{kInvalidHandle};
  TextureExtent size{};
  TexFormat fmt{TexFormat::Unknown};
};
}  // namespace rl

#endif  // ENGINE_TEXTURE_TEXTURE_H_
