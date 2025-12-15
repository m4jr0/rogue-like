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

#ifndef ENGINE_EVENT_EVENT_SYSTEM_H_
#define ENGINE_EVENT_EVENT_SYSTEM_H_

#include "engine/common.h"
#include "engine/event/message.h"

namespace rl {
using EventHandlerId = u32;
constexpr auto kInvalidEventHandlerId = static_cast<EventHandlerId>(-1);
using EventHandlerFn = void (*)(const Message&, void* userData);

class EventSystem {
 public:
  static EventSystem& instance();

  void init();
  void shutdown();

  void tick();
  void flush() { tick(); }

  void dispatch(const Message& m);
  void dispatchNow(const Message& m);

  template <typename... Ps>
  void dispatch(MessageId mid, Ps&&... ps) {
    dispatch(Message{mid, ps...});
  }

  template <typename... Ps>
  void dispatchNow(MessageId mid, Ps&&... ps) {
    dispatchNow(Message{mid, ps...});
  }

  EventHandlerId on(MessageId mid, EventHandlerFn fn, void* userData = nullptr);
  void off(EventHandlerId id);

 private:
  struct Handler {
    EventHandlerId id{kInvalidEventHandlerId};
    EventHandlerFn fn;
    void* userData{nullptr};
  };

  u8 active_queues_{0};
  std::vector<Message> queues_[2]{};
  std::unordered_map<MessageId, std::vector<Handler>> handlers_{};
  std::unordered_map<EventHandlerId, std::pair<MessageId, usize>>
      handler_to_mid_;
  std::vector<EventHandlerId> off_pending_{};
  EventHandlerId event_handler_id_counter_{0};

  EventSystem() = default;

  void deliver(const Message& m);
  void offInternal(EventHandlerId id);
};
}  // namespace rl

#define RL_EVENTSYS (::rl::EventSystem::instance())
#define RL_CEVENTSYS \
  (static_cast<const ::rl::EventSystem&>(::rl::EventSystem::instance()))

#endif  // ENGINE_EVENT_EVENT_SYSTEM_H_
