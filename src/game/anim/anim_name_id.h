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

#ifndef ENGINE_ANIM_ANIM_NAME_ID_H_
#define ENGINE_ANIM_ANIM_NAME_ID_H_

#include "engine/anim/anim_type.h"
#include "engine/common.h"

namespace rl {
enum AnimId : AnimTag {
  kAnimIdIdle = kDefaultAnimTag,
  kAnimIdWalk,
  kAnimIdRun,
  kAnimIdDash,
  kAnimIdHurt,
  kAnimIdHeal,
  kAnimIdDead,
  kAnimIdAttack0,
  kAnimIdAttack1,
  kAnimIdAttack2,
};
}  // namespace rl

#endif  // ENGINE_ANIM_ANIM_NAME_ID_H_
