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
#include "ability_library.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/log.h"
#include "engine/resource/resource_file.h"
#include "engine/resource/resource_type_registry.h"
#include "game/ability/ability_operation_bindings.h"
#include "game/ability/ability_serialize.h"
#include "game/resource/game_resource.h"

namespace rl {
AbilityLibrary& AbilityLibrary::instance() {
  static AbilityLibrary inst;
  return inst;
}

void AbilityLibrary::init() {
  RL_LOG_DEBUG("AbilityLibrary::init");
  RL_RESREG.on(kGameResourceTypeAbility, "ability");
  constexpr auto kAbilityCapacity = 256;
  slots_.reserve(kAbilityCapacity);
  opFns_ = {};
  bindAbilityOperations();
}

void AbilityLibrary::shutdown() {
  RL_LOG_DEBUG("AbilityLibrary::shutdown");
  slots_.clear();
  opFns_ = {};
}

const AbilityResource* AbilityLibrary::load(AbilityId id) {
  return slots_.acquire(id, [&, id] {
    auto* res = new AbilityResource{};

    auto ok = loadResourceFile(
        kGameResourceTypeAbility, id, AbilityResource::kVersion,
        AbilityResource::kVersion,
        [&](std::istream& is) { readAbilityResource(is, *res); });

    RL_FASSERT(
        ok,
        "AbilityLibrary::load: Failed to load ability resource with id: ", id,
        "!");

    return res;
  });
}

void AbilityLibrary::unload(AbilityId id) { slots_.release(id); }

const AbilityResource* AbilityLibrary::get(AbilityId id) const {
  return slots_.get(id);
}

void AbilityLibrary::registerOp(AbilityOp op, AbilityOpFn fn) {
  RL_ASSERT(op < AbilityOp::Count,
            "AbilityLibrary::registerOp: Invalid ability operation!");
  opFns_[static_cast<usize>(op)] = fn;
}

void AbilityLibrary::unregisterOp(AbilityOp op) {
  RL_ASSERT(op < AbilityOp::Count,
            "AbilityLibrary::registerOp: Invalid ability operation!");
  opFns_[static_cast<usize>(op)] = nullptr;
}

AbilityOpFn AbilityLibrary::op(AbilityOp op) const noexcept {
  RL_ASSERT(op < AbilityOp::Count,
            "AbilityLibrary::op: Invalid ability operation!");
  return opFns_[static_cast<usize>(op)];
}
}  // namespace rl