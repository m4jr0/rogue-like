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

#ifndef GAME_CHARACTER_CHARACTER_ARCHETYPE_LIBRARY_H_
#define GAME_CHARACTER_CHARACTER_ARCHETYPE_LIBRARY_H_

#include "engine/common.h"
#include "engine/resource/resource_slot.h"
#include "game/character/character_archetype_resource.h"

namespace rl {
class CharArchetypeLibrary {
 public:
  static CharArchetypeLibrary& instance();

  void init();
  void shutdown();

  const CharArchetypeResource* load(CharArchetypeId id);
  void unload(CharArchetypeId id);
  const CharArchetypeResource* get(CharArchetypeId id) const;

 private:
  ResourceSlots<CharArchetypeTag, CharArchetypeResource> slots_{};

  CharArchetypeLibrary() = default;
};
}  // namespace rl

#define RL_CHARARCHLIB (::rl::CharArchetypeLibrary::instance())
#define RL_CCHARARCHLIB                            \
  (static_cast<const ::rl::CharArchetypeLibrary&>( \
      ::rl::CharArchetypeLibrary::instance()))

#endif  // GAME_CHARACTER_CHARACTER_ARCHETYPE_LIBRARY_H_
