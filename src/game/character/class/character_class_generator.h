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

#ifndef GAME_CHARACTER_CLASS_CHARACTER_CLASS_GENERATOR_H_
#define GAME_CHARACTER_CLASS_CHARACTER_CLASS_GENERATOR_H_

#include "game/character/character.h"
#include "game/character/character_archetype.h"

namespace rl {
namespace internal {
void generateFsmCommon(Char& c, const CharArchetypeResource*);
void generateAnimatorCommon(Char& c, const CharArchetypeResource* arch);
void generateInputCommon(Char& c, const CharArchetypeResource*);
void generateSoundsCommon(Char&, const CharArchetypeResource* arch);
void generateAnimColRigCommon(Char& c, const CharArchetypeResource*);

void destroyFsmCommon(Char& c, const CharArchetypeResource*);
void destroyAnimatorCommon(Char& c, const CharArchetypeResource*);
void destroyInputCommon(Char& c, const CharArchetypeResource*);
void destroySoundsCommon(Char&, const CharArchetypeResource* arch);
void destroyAnimColRigCommon(Char& c, const CharArchetypeResource*);
}  // namespace internal

void generateCommon(Char& c, const CharArchetypeResource* arch);
void destroyCommon(Char& c, const CharArchetypeResource* arch);
}  // namespace rl

#endif  // GAME_CHARACTER_CLASS_CHARACTER_CLASS_GENERATOR_H_
