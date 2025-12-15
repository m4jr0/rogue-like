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

// Precompiled. ////////////////////////////////////////////////////////////////
#include "precompiled.h"
////////////////////////////////////////////////////////////////////////////////

// Header. /////////////////////////////////////////////////////////////////////
#include "character_step_handler.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/spatial_ref.h"
#include "engine/core/vector.h"
#include "engine/resource/resource_table.h"
#include "engine/sound/sound.h"
#include "engine/sound/sound_library.h"
#include "engine/sound/sound_resource.h"
#include "engine/sound/sound_system.h"
#include "game/character/character_anim.h"

namespace rl {
void CharacterStepHandler::init(usize capacity) {
  sfxs_.clear();
  sfxs_.reserve(capacity);
  sfxBankId_ = RL_CRESTAB.rid<SoundBankId>("soundbank.character.walkHit");

  if (RL_SOUNDLIB.loadBank(sfxBankId_)) {
    hasSfx_ = true;
  } else {
    RL_LOG_WARN(
        "CharacterStepHandler::init: No footstep SFX bank: footsteps "
        "disabled.");
  }
}

void CharacterStepHandler::shutdown() {
  if (hasSfx_) {
    RL_SOUNDLIB.unloadBank(sfxBankId_);
    hasSfx_ = false;
  }

  sfxBankId_ = kInvalidResourceId;
  sfxs_.clear();
}

void CharacterStepHandler::on(Char& c) {
  if (!hasSfx_) return;
  auto index = c.handle.index;
  ensureCapacity(sfxs_, index);
  auto& sfx = sfxs_[index];
  sfx.bank = sfxBankId_;
  sfx.trans = c.trans;
  sfx.listenerId =
      c.anim.on(onFootstep, kCharAnimKeyTagHitWalk, kInvalidAnimTag, &sfx);
}

void CharacterStepHandler::off(Char& c) {
  if (!hasSfx_) return;
  auto index = c.handle.index;
  if (index >= sfxs_.size()) return;
  auto& sfx = sfxs_[index];

  if (sfx.listenerId != kInvalidAnimEventListenerId) {
    c.anim.off(sfx.listenerId);
  }

  sfx = {};
}

void CharacterStepHandler::onFootstep(const AnimEvent&, void* userData) {
  auto* sfx = static_cast<FootstepSfxData*>(userData);
  if (!sfx || !sfx->trans || !sfx->bank) return;
  const auto* bank = RL_CSOUNDLIB.getBank(sfx->bank);
  if (!bank || bank->sounds.empty()) return;
  auto soundId = bank->sounds[sfx->soundIndex];
  ++sfx->soundIndex %= bank->sounds.size();

  RL_SOUNDSYS.playOnce({
      .soundId = soundId,
      .bus = kSoundBusSfx,
      .spatialRef =
          {
              .type = SpatialRefType::Transform,
              .trans = sfx->trans,
          },
  });
}
}  // namespace rl