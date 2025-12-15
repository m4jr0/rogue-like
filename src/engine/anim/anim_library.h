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

#ifndef ENGINE_ANIM_ANIM_LIBRARY_H_
#define ENGINE_ANIM_ANIM_LIBRARY_H_

#include "engine/anim/anim_resource.h"
#include "engine/anim/anim_type.h"
#include "engine/common.h"
#include "engine/resource/resource_slot.h"

namespace rl {
class AnimLibrary {
 public:
  static AnimLibrary& instance();

  void init();
  void shutdown();

  const AnimSetResource* load(AnimSetId id);
  void unload(AnimSetId id);
  const AnimSetResource* get(AnimSetId id) const;

 private:
  ResourceSlots<AnimSetTag, AnimSetResource> slots_{};

  AnimLibrary() = default;

  void buildKeyToIdx(AnimSetResource* set);
  void buildAnimSolver(AnimSetResource* set);
};
}  // namespace rl

#define RL_ANIMLIB (::rl::AnimLibrary::instance())
#define RL_CANIMLIB \
  (static_cast<const ::rl::AnimLibrary&>(::rl::AnimLibrary::instance()))

#endif  // ENGINE_ANIM_ANIM_LIBRARY_H_
