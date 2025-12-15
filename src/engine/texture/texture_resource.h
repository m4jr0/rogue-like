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

#ifndef ENGINE_TEXTURE_TEXTURE_RESOURCE_H_
#define ENGINE_TEXTURE_TEXTURE_RESOURCE_H_

#include "engine/common.h"
#include "engine/resource/resource_type.h"
#include "engine/texture/texture.h"

namespace rl {
using TextureId = ResourceId<TextureTag>;

struct TextureResource {
  inline constexpr static ResourceVersion kVersion{1};

  TextureId id{kInvalidResourceId};
  TexFormat fmt{TexFormat::Unknown};
  TextureExtent size{};
  std::vector<char> data;
};
}  // namespace rl

#endif  // ENGINE_TEXTURE_TEXTURE_RESOURCE_H_
