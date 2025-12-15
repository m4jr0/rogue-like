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
#include "player_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/vector.h"
#include "engine/event/event_system.h"
#include "engine/input/input_message.h"
#include "engine/input/input_system.h"

namespace rl {
PlayerSystem& PlayerSystem::instance() {
  static PlayerSystem inst;
  return inst;
}

void PlayerSystem::init() {
  RL_LOG_DEBUG("PlayerSystem::init");

  constexpr auto kPlayerCapacity = 4;
  hPlayerPool_.clear();
  hPlayerPool_.reserve(kPlayerCapacity);
  players_.clear();
  players_.reserve(kPlayerCapacity);
  controllerRouting_ = {};
  mainPlayer_ = generate({.tryFindGamepad = true});
  RL_EVENTSYS.on(kInputMessageIdControllerHotPlug, On, this);
}

void PlayerSystem::shutdown() {
  RL_LOG_DEBUG("PlayerSystem::shutdown");
  hPlayerPool_.clear();
  players_.clear();
  mainPlayer_.clear();
  controllerRouting_ = {};
}

PlayerHandle PlayerSystem::generate(const PlayerDesc& desc) {
  auto h = hPlayerPool_.generate();
  ensureCapacity(players_, h.index);
  auto& p = players_[h.index];
  p.handle = h;
  p.slot = static_cast<PlayerSlot>(h.index);
  pushBackUnique(activePlayers_, p.handle);
  if (desc.tryFindGamepad) tryFindGamepad(h);
  return h;
}

void PlayerSystem::destroy(PlayerHandle h) {
  RL_ASSERT(h != mainPlayer_,
            "PlayerSystem::destroy: Cannot destroy main player!");
  if (h == mainPlayer_) return;
  auto* p = player(h);
  unassignGamepad(h);
  eraseUnordered(activePlayers_, h);
  *p = {};
  hPlayerPool_.destroy(h);
}

const Player* PlayerSystem::player(PlayerHandle h) const {
  RL_ASSERT(h && hPlayerPool_.alive(h),
            "PlayerSystem::player: Invalid player handle!");
  if (!h || !hPlayerPool_.alive(h)) return nullptr;
  return &players_[h.index];
}

Player* PlayerSystem::player(PlayerHandle h) {
  RL_ASSERT(h && hPlayerPool_.alive(h),
            "PlayerSystem::player: valid player handle!");
  if (!h || !hPlayerPool_.alive(h)) return nullptr;
  return &players_[h.index];
}

const Player* PlayerSystem::playerFromSlot(PlayerSlot s) const {
  auto idx = static_cast<usize>(s);

  if (idx >= players_.size()) {
    return nullptr;
  }

  const auto& p = players_[idx];
  return hPlayerPool_.alive(p.handle) ? &p : nullptr;
}

Player* PlayerSystem::playerFromSlot(PlayerSlot s) {
  auto idx = static_cast<usize>(s);

  if (idx >= players_.size()) {
    return nullptr;
  }

  auto& p = players_[idx];
  return hPlayerPool_.alive(p.handle) ? &p : nullptr;
}

PlayerSlot PlayerSystem::slot(PlayerHandle h) const {
  RL_ASSERT(h, "PlayerSystem::slot: Invalid player handle!");
  if (!h || !hPlayerPool_.alive(h)) return kInvalidPlayerSlot;
  return players_[h.index].slot;
}

void PlayerSystem::tryFindGamepad(PlayerHandle h) {
  if (hasGamepad(h)) return;
  auto activeControllers = RL_INPUTSYS.activeControllers();

  for (auto cid : activeControllers) {
    if (RL_INPUTSYS.gamepad(cid) && !hasPlayer(cid)) {
      assignGamepad(cid, h);
      return;
    }
  }
}

void PlayerSystem::autoClaimGamepad(InputControllerId cid) {
  if (hasPlayer(cid)) return;

  for (auto& p : players_) {
    if (hasGamepad(p.handle)) continue;
    assignGamepad(cid, p.handle);
  }
}

void PlayerSystem::assignGamepad(InputControllerId cid, PlayerHandle h) {
  if (!validController(cid) || !h || !hPlayerPool_.alive(h)) return;
  unassignGamepad(cid);
  unassignGamepad(h);
  if (!RL_INPUTSYS.gamepadActive(cid)) return;
  auto& r = controllerRouting_;
  r.controllerToPlayer[cid] = h;
  r.playerToController.resize(h.index + 1, kInvalidInputControllerId);
  r.playerToController[h.index] = cid;
}

void PlayerSystem::assignGamepadFromSlot(InputControllerId cid, PlayerSlot s) {
  const auto* p = playerFromSlot(s);
  if (!p) return;
  assignGamepad(cid, p->handle);
}

void PlayerSystem::unassignGamepad(InputControllerId cid) {
  if (!validController(cid)) return;
  auto& r = controllerRouting_;
  auto h = r.controllerToPlayer[cid];
  r.controllerToPlayer[cid].clear();

  if (!h || !hPlayerPool_.alive(h) || h.index >= r.playerToController.size()) {
    return;
  }

  r.playerToController[h.index] = kInvalidInputControllerId;
}

void PlayerSystem::unassignGamepad(PlayerHandle h) {
  if (!h || !hPlayerPool_.alive(h)) return;
  auto& r = controllerRouting_;
  if (h.index >= r.playerToController.size()) return;
  auto cid = r.playerToController[h.index];
  if (cid == kInvalidInputControllerId) return;
  r.playerToController[h.index] = kInvalidInputControllerId;
  r.controllerToPlayer[cid].clear();
}

void PlayerSystem::unassignGamepadFromSlot(PlayerSlot s) {
  const auto* p = playerFromSlot(s);
  if (!p) return;
  unassignGamepad(p->handle);
}

void PlayerSystem::unassignAllGamepads() {
  controllerRouting_.controllerToPlayer.clear();
  controllerRouting_.playerToController.clear();
}

InputControllerId PlayerSystem::gamepad(PlayerHandle h) const noexcept {
  if (!h || !hPlayerPool_.alive(h)) return kInvalidInputControllerId;
  const auto& r = controllerRouting_;
  if (h.index >= r.playerToController.size()) return kInvalidInputControllerId;
  return r.playerToController[h.index];
}

bool PlayerSystem::hasGamepad(PlayerHandle h) const noexcept {
  return gamepad(h) != kInvalidInputControllerId;
}

bool PlayerSystem::hasPlayer(InputControllerId cid) const noexcept {
  const auto& c = controllerRouting_.controllerToPlayer;
  auto it = c.find(cid);
  return it != c.cend() && it->second;
}

PlayerHandle PlayerSystem::playerFromCid(InputControllerId cid) const noexcept {
  if (!validController(cid)) return kInvalidHandle;
  const auto& r = controllerRouting_;
  if (auto it = r.controllerToPlayer.find(cid);
      it != r.controllerToPlayer.cend() || it->second) {
    return it->second;
  }

  return kInvalidHandle;
}

void PlayerSystem::On(const Message& m, void* userData) {
  auto* sys = static_cast<PlayerSystem*>(userData);

  switch (m.id) {
    case kInputMessageIdControllerHotPlug: {
      auto cid = m.get<InputControllerId>(0);
      auto active = m.get<bool>(1);

      if (active) {
        sys->autoClaimGamepad(cid);
      } else {
        sys->unassignGamepad(cid);
      }

      break;
    }
    default: {
      break;
    }
  }
}
}  // namespace rl