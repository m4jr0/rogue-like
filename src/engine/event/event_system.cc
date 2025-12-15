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
#include "event_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/log.h"

namespace rl {
EventSystem& EventSystem::instance() {
  static EventSystem inst;
  return inst;
}

void EventSystem::init() { RL_LOG_DEBUG("EventSystem::init"); }

void EventSystem::shutdown() {
  RL_LOG_DEBUG("EventSystem::shutdown");

  for (auto& queue : queues_) {
    queue.clear();
  }

  handlers_.clear();
}

void EventSystem::tick() {
  auto& queue{queues_[active_queues_]};
  active_queues_ = static_cast<u8>(!active_queues_);

  for (const auto& m : queue) {
    deliver(m);
  }

  queue.clear();

  for (auto hid : off_pending_) {
    offInternal(hid);
  }

  off_pending_.clear();
}

void EventSystem::dispatch(const Message& m) {
  queues_[active_queues_].push_back(m);
}

void EventSystem::dispatchNow(const Message& m) { deliver(m); }

EventHandlerId EventSystem::on(MessageId mid, EventHandlerFn fn,
                               void* userData) {
  auto& v = handlers_[mid];
  const EventHandlerId hid = event_handler_id_counter_++;
  v.push_back({hid, fn, userData});
  handler_to_mid_[hid] = {mid, v.size() - 1};
  return hid;
}

void EventSystem::off(EventHandlerId id) { off_pending_.push_back(id); }

void EventSystem::deliver(const Message& m) {
  auto it = handlers_.find(m.id);
  if (it == handlers_.cend()) return;
  auto& handlers = it->second;

  for (auto& h : handlers) {
    h.fn(m, h.userData);
  }
}

void EventSystem::offInternal(EventHandlerId id) {
  auto it = handler_to_mid_.find(id);
  if (it == handler_to_mid_.cend()) return;

  auto [mid, idx] = it->second;
  auto hit = handlers_.find(mid);

  if (hit == handlers_.end()) {
    handler_to_mid_.erase(it);
    return;
  }

  auto& v = hit->second;
  auto last = v.size() - 1;

  if (idx != last) {
    std::swap(v[idx], v[last]);
    handler_to_mid_[v[idx].id].second = idx;
  }

  v.pop_back();
  handler_to_mid_.erase(it);

  if (v.empty()) handlers_.erase(hit);
}
}  // namespace rl