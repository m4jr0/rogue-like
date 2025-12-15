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

#ifndef ENGINE_EVENT_MESSAGE_H_
#define ENGINE_EVENT_MESSAGE_H_

#include "engine/common.h"
#include "engine/core/param_set.h"

namespace rl {
using MessageId = u64;
constexpr auto kInvalidMessageId = static_cast<MessageId>(-1);

struct Message : ParamSet<5> {
  MessageId id{kInvalidMessageId};

  template <typename... Ps>
  Message(MessageId mid, Ps&&... ps)
      : ParamSet<5>{std::forward<Ps>(ps)...}, id{mid} {}
};

enum class MessageCategory : u8 {
  Anim,
  Core,
  Event,
  Input,
  FixedUpdate,
  Player,
  Update,
  Resource,
  Scene
};

using MessageLocalId = u32;

constexpr MessageId mid(MessageCategory cat, MessageLocalId lid) {
  return static_cast<MessageId>(cat) << 32 | static_cast<MessageId>(lid);
}
}  // namespace rl

#endif  // ENGINE_EVENT_MESSAGE_H_
