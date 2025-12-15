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

#ifndef GAME_CHARACTER_CHARACTER_FSM_H_
#define GAME_CHARACTER_CHARACTER_FSM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/physics/physics.h"

namespace rl {
struct Char;

using CharStateId = u16;
constexpr auto kInvalidCharStateId = static_cast<CharStateId>(-1);

using CharStateEventId = u16;
constexpr auto kInvalidCharStateEventId = static_cast<CharStateEventId>(-1);

enum : CharStateId {
  kCharStateIdle = 0,
  kCharStateMove,
  kCharStateAbilityRun,
  kCharStateHurt,
  kCharStateDeath,
};

enum : CharStateEventId {
  kCharStateEventStartMove = 0,
  kCharStateEventStopMove,
  kCharStateEventStartAbility,
  kCharStateEventAbilityDone,
  kCharStateEventHurt,
  kCharStateEventDeath,
};

struct CharState {
  void (*onEnter)(Char&) noexcept {nullptr};
  void (*onUpdate)(Char&, const FramePacket&) noexcept {nullptr};
  void (*onExit)(Char&) noexcept {nullptr};
};

struct CharTransition {
  CharStateId from{};
  CharStateId to{};
  CharStateEventId ev{};
};

struct CharFsm {
  std::vector<CharState> states{};
  std::vector<CharTransition> transitions{};
  CharStateId current{0};

  void signal(CharStateEventId ev, Char& c) noexcept;

  void tick(Char& c, const FramePacket& f) noexcept {
    if (states[current].onUpdate) states[current].onUpdate(c, f);
  }
};

bool tryStartAbility(Char& c) noexcept;
Steering handleMove(const Char& c, bool canMove);

void onEnterIdle(Char& c) noexcept;
void onUpdateIdle(Char& c, const FramePacket&) noexcept;
void onEnterMove(Char& c) noexcept;
void onUpdateMove(Char& c, const FramePacket&) noexcept;
void onExitMove(Char& c) noexcept;
void onEnterAbility(Char& c) noexcept;
void onUpdateAbility(Char& c, const FramePacket& f) noexcept;
void onExitAbility(Char& c) noexcept;
}  // namespace rl

#endif  // GAME_CHARACTER_CHARACTER_FSM_H_
