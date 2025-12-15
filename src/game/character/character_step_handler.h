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

#ifndef GAME_CHARACTER_CHARACTER_STEP_HANDLER_H_
#define GAME_CHARACTER_CHARACTER_STEP_HANDLER_H_

#include "engine/anim/anim_runtime.h"
#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/resource/resource_type.h"
#include "engine/sound/sound_resource.h"
#include "engine/transform/transform.h"
#include "game/character/character.h"

namespace rl {
class CharacterStepHandler {
 public:
  void init(usize capacity);
  void shutdown();

  void on(Char& c);
  void off(Char& c);

 private:
  struct FootstepSfxData {
    SoundBankId bank{kInvalidResourceId};
    TransformHandle trans{kInvalidHandle};
    AnimEventListenerId listenerId{kInvalidAnimEventListenerId};
    usize soundIndex{0};
  };

  bool hasSfx_{false};
  SoundBankId sfxBankId_{kInvalidResourceId};
  std::vector<FootstepSfxData> sfxs_{};

  static void onFootstep(const AnimEvent&, void* userData);
};
}  // namespace rl

#endif  // GAME_CHARACTER_CHARACTER_STEP_HANDLER_H_
