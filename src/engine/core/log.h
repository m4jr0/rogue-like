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

#ifndef ENGINE_CORE_LOG_H_
#define ENGINE_CORE_LOG_H_

#include "engine/common.h"

#ifndef RL_LOG_LEVEL
#ifdef RL_DEBUG
#define RL_LOG_LEVEL 0
#else
#define RL_LOG_LEVEL 1
#endif  // RL_DEBUG
#endif  // RL_LOG_LEVEL

namespace rl {
namespace internal {
enum class LogLevel { Debug = 0, Info = 1, Warn = 2, Error = 3 };

inline const char* getLogLevelStr(LogLevel lvl) {
  switch (lvl) {
    using enum LogLevel;
    case Debug:
      return "DEBUG";
    case Info:
      return "INFO";
    case Warn:
      return "WARN";
    case Error:
      return "ERROR";
    default:
      return "UNKNOWN";
  }
}

template <typename... Args>
void log(LogLevel lvl, Args&&... args) {
  std::cout << "[RL] " << '[' << getLogLevelStr(lvl) << "] ";
  ((std::cout << std::forward<Args>(args)), ...);
  std::cout << '\n';
}
}  // namespace internal
}  // namespace rl

#if RL_LOG_LEVEL <= 0
#define RL_LOG_DEBUG(...) \
  ::rl::internal::log(rl::internal::LogLevel::Debug, __VA_ARGS__)
#else
#define RL_LOG_DEBUG(...) ((void)0)
#endif

#if RL_LOG_LEVEL <= 1
#define RL_LOG_INFO(...) \
  ::rl::internal::log(rl::internal::LogLevel::Info, __VA_ARGS__)
#else
#define RL_LOG_INFO(...) ((void)0)
#endif

#if RL_LOG_LEVEL <= 2
#define RL_LOG_WARN(...) \
  ::rl::internal::log(rl::internal::LogLevel::Warn, __VA_ARGS__)
#else
#define RL_LOG_WARN(...) ((void)0)
#endif

#if RL_LOG_LEVEL <= 3
#define RL_LOG_ERR(...) \
  ::rl::internal::log(rl::internal::LogLevel::Error, __VA_ARGS__)
#else
#define RL_LOG_ERR(...) ((void)0)
#endif

#endif  // ENGINE_CORE_LOG_H_
