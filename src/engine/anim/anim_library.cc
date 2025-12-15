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
#include "anim_library.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/anim/anim_serialize.h"
#include "engine/core/log.h"
#include "engine/physics/anim_collider_library.h"
#include "engine/resource/resource_file.h"
#include "engine/resource/resource_type_registry.h"
#include "engine/texture/texture_library.h"

namespace rl {
AnimLibrary& AnimLibrary::instance() {
  static AnimLibrary inst;
  return inst;
}

void AnimLibrary::init() {
  RL_LOG_DEBUG("AnimLibrary::init");
  RL_RESREG.on(kResourceTypeAnimSet, "animset");
  constexpr auto kSetCapacity = 32;
  slots_.reserve(kSetCapacity);
}

void AnimLibrary::shutdown() {
  RL_LOG_DEBUG("AnimLibrary::shutdown");
  RL_RESREG.off(kResourceTypeAnimSet);
  slots_.clear();
}

const AnimSetResource* AnimLibrary::load(AnimSetId id) {
  return slots_.acquire(id, [&, id] {
    auto* res = new AnimSetResource{};

    auto ok = loadResourceFile(
        kResourceTypeAnimSet, id, AnimSetResource::kVersion,
        AnimSetResource::kVersion,
        [&](std::istream& is) { readAnimSetResource(is, *res); });

    RL_FASSERT(
        ok, "AnimLibrary::load: Failed to load anim-set resource with id: ", id,
        "!");

    if (res->tex != kInvalidResourceId) {
      RL_TEXLIB.load(res->tex);
    }

    if (res->colliderProfile != kInvalidResourceId) {
      RL_ANIMCOLLIB.load(res->colliderProfile);
    }

    buildKeyToIdx(res);
    buildAnimSolver(res);
    return res;
  });
}

void AnimLibrary::unload(AnimSetId id) {
  const auto* res = get(id);

  if (res) {
    if (res->tex) {
      RL_TEXLIB.unload(res->tex);
    }

    if (res->colliderProfile) {
      RL_ANIMCOLLIB.unload(res->colliderProfile);
    }
  }

  slots_.release(id);
}

const AnimSetResource* AnimLibrary::get(AnimSetId id) const {
  return slots_.get(id);
}

void AnimLibrary::buildKeyToIdx(AnimSetResource* set) {
  set->keyToIdx.clear();
  set->keyToIdx.reserve(set->anims.size());

  for (usize i = 0; i < set->anims.size(); ++i) {
    const auto& a = set->anims[i];
    [[maybe_unused]] auto [_, inserted] = set->keyToIdx.emplace(a.key, i);
    RL_ASSERT(inserted, "AnimLibrary::rebuildKeyToIdx: duplicate key: ", a.key,
              " at idx: ", i, "!");
  }
}

void AnimLibrary::buildAnimSolver(AnimSetResource* set) {
  std::unordered_map<AnimKey, SolvedAnim> map;
  map.reserve(set->solved.size());

  for (const AnimSolvedEntry& e : set->solved) {
    if (e.fromKey == kInvalidAnimKey || e.toKey == kInvalidAnimKey) {
      continue;
    }

    SolvedAnim solved{
        .flipPolicy = e.flipPolicy,
        .key = e.toKey,
    };

    map[e.fromKey] = solved;
  }

  set->solver = AnimSolver{std::move(map)};
}
}  // namespace rl