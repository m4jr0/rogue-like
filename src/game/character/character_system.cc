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
#include "character_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/log.h"
#include "engine/core/vector.h"
#include "engine/physics/physics.h"
#include "engine/physics/physics_system.h"
#include "engine/render/render_submit.h"
#include "engine/transform/transform_system.h"
#include "game/character/character_factory.h"
#include "game/character/character_utils.h"
#include "game/render/game_render_common.h"

#ifdef RL_DEBUG
#include "engine/render/render_gizmo.h"
#endif  // RL_DEBUG

namespace rl {
CharSystem& CharSystem::instance() {
  static CharSystem inst;
  return inst;
}

void CharSystem::init() {
  RL_LOG_DEBUG("CharSystem::init");
  RL_CHARFACTORY.init();
  constexpr auto kCharCapacity = 64;
  hCharPool_.clear();
  hCharPool_.reserve(kCharCapacity);
  chars_.reserve(kCharCapacity);
  stepHandler_.init(kCharCapacity);
}

void CharSystem::shutdown() {
  RL_LOG_DEBUG("CharSystem::shutdown");
  stepHandler_.shutdown();
  hCharPool_.clear();
  chars_.clear();
  RL_CHARFACTORY.shutdown();
}

void CharSystem::fixedUpdate(const FramePacket& f) {
  for (auto& c : chars_) {
    if (!c.handle) continue;

    if (c.body) {
      const auto* b = RL_CPHYSICSSYS.body(c.body);
      c.dir = b ? b->cardDir : CardinalDir::Unset;
    } else {
      c.dir = CardinalDir::Unset;
    }

    c.fsm.tick(c, f);
  }
}

void CharSystem::update(const FramePacket&) {
  for (auto& c : chars_) {
    if (!c.handle) continue;
    if (!c.anim.animator) continue;
    const auto* t = RL_CTRANSSYS.global(c.trans);
    if (!t) continue;
#ifdef RL_DEBUG
    if (dFlags_ & kCharSystemDebugFlagBitsCharacterPos) {
      submitGizmoPoint(t->pos, kRgbaYellow);
    }
#endif  // RL_DEBUG

    submitAnimator(c.anim.animator, t->trs,
                   {
                       .flags = kDrawFlagBitsNone,
                       .layer = kGameRenderLayerWorld,
                       .priority = charToRenderPriority(c),
                       .sortYType = SortYType::Pivot,
                   });
  }
}

CharHandle CharSystem::generate(const CharDesc& desc) {
  auto h = hCharPool_.generate();
  ensureCapacity(chars_, h.index);
  auto& c = chars_[h.index];
  c.handle = h;
  RL_CHARFACTORY.populate(c, desc);
  stepHandler_.on(c);
  return h;
}

void CharSystem::destroy(CharHandle h) {
  RL_ASSERT(h, "CharSystem::destroy: Invalid character handle provided!");
  if (!h) return;
  auto& c = chars_[h.index];
  stepHandler_.off(c);
  RL_CHARFACTORY.unpopulate(c);
  hCharPool_.destroy(h);
}

const Char* CharSystem::character(CharHandle h) const {
  RL_ASSERT(h && hCharPool_.alive(h),
            "CharSystem::character: Invalid character handle provided!");
  if (!h || !hCharPool_.alive(h)) return nullptr;
  return &chars_[h.index];
}

bool CharSystem::exists(CharHandle h) const { return h && hCharPool_.alive(h); }

Char* CharSystem::character(CharHandle h) {
  RL_ASSERT(h && hCharPool_.alive(h),
            "CharSystem::character: Invalid character handle provided!");
  if (!h || !hCharPool_.alive(h)) return nullptr;
  return &chars_[h.index];
}
}  // namespace rl