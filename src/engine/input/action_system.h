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

#ifndef ENGINE_INPUT_ACTION_SYSTEM_H_
#define ENGINE_INPUT_ACTION_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/type_trait.h"
#include "engine/input/action.h"
#include "engine/math/vec2.h"

namespace rl {
class ActionSystem {
 public:
  static ActionSystem& instance();

  void init();
  void shutdown();

  void set(const ActionSet& set);
  void unset(ActionScope s);
  void unset(ActionScope s, ActionTag t);

  void enable(ActionScope s, bool active = true);
  void disable(ActionScope s);

  template <class Enum>
  bool on(ActionScope scope, Enum action) const {
    auto idx = static_cast<usize>(scope);
    if (!scopeStates_.test(idx)) return false;
    auto key = actionKey(scope, static_cast<ActionTag>(action));
    auto it = digitalBtnBindings_.find(key);
    if (it == digitalBtnBindings_.cend()) return false;
    const auto& bindings = it->second;

    for (const auto& binding : bindings) {
      if (digitalBtnValue(scope, binding)) return true;
    }

    return false;
  }

  template <class Enum>
  f32 onAnalog(ActionScope scope, Enum action) const {
    auto idx = static_cast<usize>(scope);
    if (!scopeStates_.test(idx)) return .0f;
    auto key = actionKey(scope, static_cast<ActionTag>(action));
    auto it = analogBtnBindings_.find(key);
    if (it == analogBtnBindings_.cend()) return .0f;
    const auto& bindings = it->second;
    f32 best = .0f;

    for (const auto& b : bindings) {
      auto v = analogBtnValue(b, scope);
      if (std::fabs(v) > std::fabs(best)) best = v;
    }

    return std::clamp(best, -1.0f, 1.0f);
  }

  template <class Enum>
  Vec2F32 axis(ActionScope scope, Enum action) const {
    auto idx = static_cast<usize>(scope);
    if (!scopeStates_.test(idx)) return Vec2F32::zero();
    auto key = actionKey(scope, static_cast<ActionTag>(action));
    auto it = analogAxisBindings_.find(key);
    if (it == analogAxisBindings_.cend()) return Vec2F32::zero();

    auto best = Vec2F32::zero();
    f32 bestM = .0f;

    for (const auto& b : it->second) {
      auto v = analogAxisValue(b, scope);
      auto m = v.magSqrd();

      if (m > bestM) {
        best = v;
        bestM = m;
      }
    }

    return best;
  }

 private:
  std::bitset<kMaxActionScopeCount> scopeStates_{};
  std::unordered_map<ActionKey, std::vector<ActionDigitalBtnBinding>>
      digitalBtnBindings_{};
  std::unordered_map<ActionKey, std::vector<ActionAnalogBtnBinding>>
      analogBtnBindings_{};
  std::unordered_map<ActionKey, std::vector<ActionAnalogAxisBinding>>
      analogAxisBindings_{};

  ActionSystem() = default;

  template <typename T>
  void setBindings(ActionScope s, std::span<const T> bindings,
                   std::unordered_map<ActionKey, std::vector<T>>& savedTo) {
    std::unordered_map<ActionTag, std::vector<T>> byTag;
    byTag.reserve(bindings.size());

    for (auto& b : bindings) {
      RL_ASSERT(b.actionTag != kInvalidActionTag,
                "ActionSystem::setBindings: Invalid action tag detected!");
      byTag[b.actionTag].push_back(b);
    }

    for (auto& [tag, vec] : byTag) {
      auto key = actionKey(s, tag);
      savedTo[key] = std::move(vec);
    }
  }

  template <typename T>
  void unsetBindings(ActionScope s,
                     std::unordered_map<ActionKey, std::vector<T>>& bindings) {
    for (auto it = bindings.begin(); it != bindings.end();) {
      if (actionScopeFromKey(it->first) == s) {
        it = bindings.erase(it);
      } else {
        ++it;
      }
    }
  }

  bool digitalBtnValue(ActionScope scope,
                       const ActionDigitalBtnBinding& binding) const;

  f32 analogBtnValue(const ActionAnalogBtnBinding& b, ActionScope scope) const;
  Vec2F32 analogAxisValue(const ActionAnalogAxisBinding& b,
                          ActionScope scope) const;

  static f32 analogBtnShape(f32 v, f32 deadzone, f32 saturation, f32 scale,
                            f32 curve);
  static Vec2F32 analogAxisShape(const Vec2F32& v, f32 deadzone, f32 saturation,
                                 f32 scale, f32 curve);
};
}  // namespace rl

#define RL_ACTIONSYS (::rl::ActionSystem::instance())
#define RL_CACTIONSYS \
  (static_cast<const ::rl::ActionSystem&>(::rl::ActionSystem::instance()))

#endif  // ENGINE_INPUT_ACTION_SYSTEM_H_
