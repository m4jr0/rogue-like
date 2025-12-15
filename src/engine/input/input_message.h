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

#ifndef ENGINE_INPUT_INPUT_MESSAGE_H_
#define ENGINE_INPUT_INPUT_MESSAGE_H_

#include "engine/common.h"
#include "engine/event/message.h"

namespace rl {
// Dumb and fast way to generate scoped events.
constexpr auto kInputMessageIdWindowFocus = mid(MessageCategory::Input, 0);
constexpr auto kInputMessageIdControllerHotPlug =
    mid(MessageCategory::Input, 1);
constexpr auto kInputMessageIdControllerMode = mid(MessageCategory::Input, 2);
}  // namespace rl

#endif  // ENGINE_INPUT_INPUT_MESSAGE_H_
