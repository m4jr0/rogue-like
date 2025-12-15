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
#include "ability_operation_bindings.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/anim/anim_type.h"
#include "engine/physics/hitbox_system.h"
#include "engine/physics/physics_system.h"
#include "engine/time/time_system.h"
#include "game/ability/ability_library.h"
#include "game/ability/ability_operation.h"
#include "game/ability/ability_program.h"
#include "game/ability/ability_program_utils.h"
#include "game/character/character.h"

namespace rl {
void bindAbilityOperations() {
  auto& l = RL_ABILITYLIB;

  l.registerOp(AbilityOp::NoOp,
               [](Char&, AbilityVM&, const AbilityPayload&,
                  const FramePacket&) -> bool { return true; });

  l.registerOp(AbilityOp::PlayAnim,
               [](Char& c, AbilityVM&, const AbilityPayload& p,
                  const FramePacket&) -> bool {
                 auto anim = p.get<AnimTag>(0);
                 c.anim.play(anim);
                 return true;
               });

  l.registerOp(AbilityOp::WaitAnim,
               [](Char& c, AbilityVM&, const AbilityPayload& p,
                  const FramePacket&) -> bool {
                 auto anim = p.get<AnimTag>(0);
                 return !c.anim.playing(anim);
               });

  l.registerOp(AbilityOp::Dash,
               [](Char& c, AbilityVM&, const AbilityPayload& p,
                  const FramePacket&) -> bool {
                 RL_PHYSICSSYS.dash(c.body, p.get<f32>(0), p.get<f32>(1));
                 return true;
               });

  l.registerOp(AbilityOp::SpawnHitbox,
               [](Char& c, AbilityVM& vm, const AbilityPayload& p,
                  const FramePacket&) -> bool {
                 // WIP Code.
                 auto category = p.get<CollisionFlags>(0);
                 auto mask = p.get<CollisionFlags>(1);
                 auto duration = p.get<f64>(2);
                 const auto* shape = resolveShape(c);

                 vm.hitbox = RL_HITBOXSYS.generate({
                     .active = true,
                     .filter =
                         {
                             .category = category,
                             .mask = mask,
                         },
                     .maxHitCount = kInvalidCollisionHitCount,
                     .collider = shape->collider,
                     .endTime = RL_CTIMESYS.now() + duration,
                     .ref =
                         {
                             .type = SpatialRefType::Transform,
                             .trans = c.trans,
                         },
                 });

                 return true;
               });

  l.registerOp(AbilityOp::SpawnStickyHitbox,
               [](Char& c, AbilityVM& vm, const AbilityPayload& p,
                  const FramePacket&) -> bool {
                 // WIP Code.
                 auto category = p.get<CollisionFlags>(0);
                 auto mask = p.get<CollisionFlags>(1);
                 const auto* shape = resolveShape(c);

                 vm.hitbox = RL_HITBOXSYS.generate({
                     .active = true,
                     .filter =
                         {
                             .category = category,
                             .mask = mask,
                         },
                     .maxHitCount = kInvalidCollisionHitCount,
                     .collider = shape->collider,
                     .endTime = .0,
                     .ref =
                         {
                             .type = SpatialRefType::Transform,
                             .trans = c.trans,
                         },
                 });

                 return true;
               });

  l.registerOp(AbilityOp::Sleep,
               [](Char&, AbilityVM& v, const AbilityPayload& p,
                  const FramePacket&) -> bool {
                 v.sleepTimer = std::max(v.sleepTimer, p.get<f32>(0));
                 return false;
               });
}
}  // namespace rl