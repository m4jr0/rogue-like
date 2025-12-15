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

#ifndef ENGINE_CORE_ENGINE_H_
#define ENGINE_CORE_ENGINE_H_

#include "engine/common.h"
#include "engine/event/message.h"

namespace rl {
class Engine {
 public:
  static Engine& instance();

  void init();
  void shutdown();

  void run();

 private:
  bool shouldExit_ = true;

  Engine() = default;

#ifdef RL_DEBUG
  void debug();
#define RL_ENGINEDEB() debug();
#else
#define RL_ENGINEDEB(...) ((void)0)
#endif  // RL_DEBUG

  static void On(const Message& m, void* userData);
};
}  // namespace rl

#define RL_ENGINE (::rl::Engine::instance())
#define RL_CENGINE (static_cast<const ::rl::Engine&>(::rl::Engine::instance()))

#endif  // ENGINE_CORE_ENGINE_H_
