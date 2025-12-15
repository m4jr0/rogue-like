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

#ifndef GAME_CHARACTER_CHARACTER_FACTORY_H_
#define GAME_CHARACTER_CHARACTER_FACTORY_H_

#include "engine/common.h"
#include "game/character/character.h"

namespace rl {
namespace internal {
using GenerateCharFn = void (*)(Char&, const CharArchetypeResource*);
using DestroyCharFn = void (*)(Char&, const CharArchetypeResource*);

struct CharVTable {
  GenerateCharFn generateChar{nullptr};
  DestroyCharFn destroyChar{nullptr};
};
}  // namespace internal

class CharFactory {
 public:
  static CharFactory& instance();

  void init();
  void shutdown();

  bool populate(Char& c, const CharDesc& desc) const;
  void unpopulate(Char& c) const;

 private:
  std::unordered_map<CharArchetypeId, internal::CharVTable> table_{};

  const internal::CharVTable* get(CharArchetypeId key) const;
  void generateTable();
};
}  // namespace rl

#define RL_CHARFACTORY (::rl::CharFactory::instance())
#define RL_CCHARFACTORY \
  (static_cast<const ::rl::CharFactory&>(::rl::CharFactory::instance()))

#endif  // GAME_CHARACTER_CHARACTER_FACTORY_H_
