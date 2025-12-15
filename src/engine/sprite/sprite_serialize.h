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

#ifndef ENGINE_SPRITE_SPRITE_SERIALIZE_H_
#define ENGINE_SPRITE_SPRITE_SERIALIZE_H_

#include "engine/common.h"
#include "engine/core/serialize.h"
#include "engine/sprite/sprite.h"

namespace rl {
static_assert(TriviallySerializable<SpriteRect>);
static_assert(TriviallySerializable<SpritePositionNorm>);
static_assert(TriviallySerializable<SpriteExtent>);

void writeSprite(std::ostream& os, const Sprite& s);
void readSprite(std::istream& is, Sprite& s);
void writeAtlasResource(std::ostream& os, const AtlasResource& a);
void readAtlasResource(std::istream& is, AtlasResource& a);
}  // namespace rl

#endif  // ENGINE_SPRITE_SPRITE_SERIALIZE_H_
