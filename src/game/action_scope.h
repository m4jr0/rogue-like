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

#ifndef GAME_ACTION_SCOPE_H_
#define GAME_ACTION_SCOPE_H_

#include "engine/common.h"
#include "engine/input/action.h"
#include "engine/player/player.h"

namespace rl {
constexpr PlayerSlot kMaxGamePlayerCount = 4;

constexpr PlayerSlot kPlayer0Slot = 0;
constexpr PlayerSlot kPlayer1Slot = 1;
constexpr PlayerSlot kPlayer2Slot = 2;
constexpr PlayerSlot kPlayer3Slot = 3;

constexpr ActionScopeTag kActionScopeTagGlobal = 0;
constexpr ActionScopeTag kActionScopeTagChar = 1;
constexpr ActionScopeTag kActionScopeTagUi = 2;

constexpr ActionScope kPlayer0Global =
    actionScope(kActionScopeTagGlobal, kPlayer0Slot);
constexpr ActionScope kPlayer1Global =
    actionScope(kActionScopeTagGlobal, kPlayer1Slot);
constexpr ActionScope kPlayer2Global =
    actionScope(kActionScopeTagGlobal, kPlayer2Slot);
constexpr ActionScope kPlayer3Global =
    actionScope(kActionScopeTagGlobal, kPlayer3Slot);

constexpr ActionScope kPlayer0Char =
    actionScope(kActionScopeTagChar, kPlayer0Slot);
constexpr ActionScope kPlayer1Char =
    actionScope(kActionScopeTagChar, kPlayer1Slot);
constexpr ActionScope kPlayer2Char =
    actionScope(kActionScopeTagChar, kPlayer2Slot);
constexpr ActionScope kPlayer3Char =
    actionScope(kActionScopeTagChar, kPlayer3Slot);

constexpr ActionScope kPlayer0Ui = actionScope(kActionScopeTagUi, kPlayer0Slot);
constexpr ActionScope kPlayer1Ui = actionScope(kActionScopeTagUi, kPlayer1Slot);
constexpr ActionScope kPlayer2Ui = actionScope(kActionScopeTagUi, kPlayer2Slot);
constexpr ActionScope kPlayer3Ui = actionScope(kActionScopeTagUi, kPlayer3Slot);
}  // namespace rl

#endif  // GAME_ACTION_SCOPE_H_
