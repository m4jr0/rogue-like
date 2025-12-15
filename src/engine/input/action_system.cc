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
#include "action_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/type_trait.h"
#include "engine/input/input_system.h"

namespace rl {
ActionSystem& ActionSystem::instance() {
  static ActionSystem inst;
  return inst;
}

void ActionSystem::init() {
  RL_LOG_DEBUG("ActionSystem::init");
  digitalBtnBindings_.reserve(128);
  digitalBtnBindings_.clear();
  analogBtnBindings_.clear();
  analogAxisBindings_.clear();
  scopeStates_.reset();
}

void ActionSystem::shutdown() {
  RL_LOG_DEBUG("ActionSystem::shutdown");
  digitalBtnBindings_.clear();
  analogBtnBindings_.clear();
  analogAxisBindings_.clear();
  scopeStates_.reset();
}

void ActionSystem::set(const ActionSet& set) {
  if (set.scope == kInvalidActionScope) return;
  const auto& bindings = set.bindings;
  setBindings(set.scope, bindings.digitalBtns, digitalBtnBindings_);
  setBindings(set.scope, bindings.analogBtns, analogBtnBindings_);
  setBindings(set.scope, bindings.analogAxes, analogAxisBindings_);
}

void ActionSystem::unset(ActionScope s) {
  unsetBindings(s, digitalBtnBindings_);
  unsetBindings(s, analogBtnBindings_);
  unsetBindings(s, analogAxisBindings_);
}

void ActionSystem::unset(ActionScope s, ActionTag t) {
  auto k = actionKey(s, t);
  digitalBtnBindings_.erase(k);
  analogBtnBindings_.erase(k);
  analogAxisBindings_.erase(k);
}

void ActionSystem::enable(ActionScope s, bool active) {
  if (s == kInvalidActionScope) return;
  auto idx = static_cast<usize>(s);

  if (active) {
    scopeStates_.set(idx);
  } else {
    scopeStates_.reset(idx);
  }
}

void ActionSystem::disable(ActionScope s) { enable(s, false); }

bool ActionSystem::digitalBtnValue(
    ActionScope scope, const ActionDigitalBtnBinding& binding) const {
  switch (binding.device) {
    using enum InputDevice;

    case Keyboard: {
      return RL_INPUTSYS.on(binding.edge, binding.keyboard.key,
                            binding.keyboard.mods);
    }

    case Mouse: {
      return RL_INPUTSYS.on(binding.edge, binding.mouse.btn,
                            binding.mouse.mods);
    }

    case Controller: {
      auto cid = fromScopeToController(scope);
      if (cid == kInvalidInputControllerId) return false;
      return RL_INPUTSYS.on(binding.edge, cid, binding.gamepad.btn);
    }

    default: {
      return false;
    }
  }
}

f32 ActionSystem::analogBtnValue(const ActionAnalogBtnBinding& b,
                                 ActionScope scope) const {
  switch (b.device) {
    using enum InputDevice;

    case Controller: {
      auto cid = fromScopeToController(scope);
      if (cid == kInvalidInputControllerId) return .0f;
      auto raw = RL_INPUTSYS.axis(cid, b.gamepad.axis);
      if (b.gamepad.positiveOnly) raw = std::max(.0f, raw);
      return analogBtnShape(raw, b.deadzone, b.saturation, b.scale, b.curve);
    }

    case Mouse: {
      auto raw = b.mouse.horizontal ? RL_INPUTSYS.mouseDelta().x
                                    : RL_INPUTSYS.mouseDelta().y;
      return analogBtnShape(raw, b.deadzone, b.saturation, b.scale, b.curve);
    }

    default: {
      return .0f;
    }
  }
}

Vec2F32 ActionSystem::analogAxisValue(const ActionAnalogAxisBinding& b,
                                      ActionScope scope) const {
  switch (b.device) {
    using enum InputDevice;

    case Controller: {
      auto cid = fromScopeToController(scope);
      if (cid == kInvalidInputControllerId) return Vec2F32::zero();
      float x = RL_INPUTSYS.axis(cid, b.gamepad.x);
      float y = RL_INPUTSYS.axis(cid, b.gamepad.y);
      if (b.gamepad.invertX) x = -x;
      if (b.gamepad.invertY) y = -y;
      return analogAxisShape({x, y}, b.deadzone, b.saturation, b.scale,
                             b.curve);
    }

    case Mouse: {
      auto raw = b.mouse.useDelta ? RL_INPUTSYS.mouseDelta()
                                  : RL_INPUTSYS.mousePosition();
      auto x = raw.x * b.mouse.sensitivityX * (b.mouse.invertX ? -1.0f : 1.0f);
      auto y = raw.y * b.mouse.sensitivityY * (b.mouse.invertY ? -1.0f : 1.0f);
      return analogAxisShape({x, y}, b.deadzone, b.saturation, b.scale,
                             b.curve);
    }

    default:
      return Vec2F32::zero();
  }
}

f32 ActionSystem::analogBtnShape(f32 v, f32 deadzone, f32 saturation, f32 scale,
                                 f32 curve) {
  auto sign = (v >= .0f) ? 1.0f : -1.0f;
  auto a = std::fabs(v);
  if (a <= deadzone) return .0f;
  a = (a - deadzone) / std::max(1e-6f, (saturation - deadzone));
  a = std::clamp(a, .0f, 1.0f);
  if (curve != 1.0f) a = std::pow(a, curve);
  return sign * (a * scale);
}

Vec2F32 ActionSystem::analogAxisShape(const Vec2F32& v, f32 deadzone,
                                      f32 saturation, f32 scale, f32 curve) {
  auto mx = v.x;
  auto my = v.y;
  auto mag = v.mag();
  if (mag <= deadzone) return Vec2F32::zero();

  auto t = (mag - deadzone) / std::max(1e-6f, (saturation - deadzone));
  t = std::clamp(t, .0f, 1.0f);
  if (curve != 1.0f) t = std::pow(t, curve);
  auto out = t * scale;
  auto invMag = 1.0f / mag;
  return {mx * invMag * out, my * invMag * out};
}
}  // namespace rl
