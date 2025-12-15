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

#ifndef GAME_ABILITY_ABILITY_LIBRARY_H_
#define GAME_ABILITY_ABILITY_LIBRARY_H_

#include "engine/common.h"
#include "engine/resource/resource_slot.h"
#include "game/ability/ability.h"
#include "game/ability/ability_resource.h"

namespace rl {
class AbilityLibrary {
 public:
  static AbilityLibrary& instance();

  void init();
  void shutdown();

  const AbilityResource* load(AbilityId id);
  void unload(AbilityId id);
  const AbilityResource* get(AbilityId id) const;

  void registerOp(AbilityOp op, AbilityOpFn fn);
  void unregisterOp(AbilityOp op);
  AbilityOpFn op(AbilityOp op) const noexcept;

 private:
  ResourceSlots<AbilityTag, AbilityResource> slots_{};
  std::array<AbilityOpFn, static_cast<usize>(AbilityOp::Count)> opFns_{};

  AbilityLibrary() = default;
};
}  // namespace rl

#define RL_ABILITYLIB (::rl::AbilityLibrary::instance())
#define RL_CABILITYLIB \
  (static_cast<const ::rl::AbilityLibrary&>(::rl::AbilityLibrary::instance()))

#endif  // GAME_ABILITY_ABILITY_LIBRARY_H_
