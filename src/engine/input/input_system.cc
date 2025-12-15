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
#include "input_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/log.h"
#include "engine/core/value_utils.h"
#include "engine/core/vector.h"
#include "engine/event/event_system.h"
#include "engine/input/input.h"
#include "engine/input/input_message.h"
#include "engine/render/render_system.h"

namespace rl {
namespace internal {
static InputSystem* gSelf = nullptr;
}  // namespace internal

struct InputSystem::PlatformCtx {
  GLFWwindow* window{nullptr};
  GLFWkeyfun onKeyboard{nullptr};
  GLFWmousebuttonfun onMouseBtn{nullptr};
  GLFWcursorposfun onMousePos{nullptr};
  GLFWscrollfun onMouseScroll{nullptr};
  GLFWjoystickfun onControllerHotPlug{nullptr};
  GLFWwindowfocusfun onWindowFocus{nullptr};
  InputMousePos lastMousePos{InputMousePos::zero()};
  bool mouseOob{false};
};

InputSystem& InputSystem::instance() {
  static InputSystem inst;
  return inst;
}

void InputSystem::init(void* window) {
  RL_LOG_DEBUG("InputSystem::init");
  internal::gSelf = this;
  window_ = window;
  platform_ = std::make_unique<PlatformCtx>();
  platform_->window = static_cast<GLFWwindow*>(window_);
  initControllers();
  bindCallbacks();
}

void InputSystem::shutdown() {
  RL_LOG_DEBUG("InputSystem::shutdown");
  unbindCallbacks();
  state_ = {};
  platform_.reset();
  window_ = nullptr;
  internal::gSelf = nullptr;
}

void InputSystem::poll() {
  state_.newFrame();
  glfwPollEvents();
  pollControllers();
  state_.resetMods();
}

bool InputSystem::pressed(InputKeyCode key,
                          InputKeyModFlags mods) const noexcept {
  return (state_.mods & mods) == mods &&
         state_.keyPressed.test(static_cast<usize>(key));
}

bool InputSystem::released(InputKeyCode key,
                           InputKeyModFlags mods) const noexcept {
  return (state_.mods & mods) == mods &&
         state_.keyReleased.test(static_cast<usize>(key));
}

bool InputSystem::down(InputKeyCode key, InputKeyModFlags mods) const noexcept {
  return (state_.mods & mods) == mods &&
         state_.keyDown.test(static_cast<usize>(key));
}

bool InputSystem::on(InputEdge edge, InputKeyCode key,
                     InputKeyModFlags mods) const noexcept {
  switch (edge) {
    using enum InputEdge;

    case Press:
      return pressed(key, mods);
    case Release:
      return released(key, mods);
    case Down:
      return down(key, mods);
    default:
      return false;
  }
}

bool InputSystem::pressed(InputMouseBtn btn,
                          InputKeyModFlags mods) const noexcept {
  return (state_.mods & mods) == mods &&
         state_.mousePressed.test(static_cast<usize>(btn));
}

bool InputSystem::released(InputMouseBtn btn,
                           InputKeyModFlags mods) const noexcept {
  return (state_.mods & mods) == mods &&
         state_.mouseReleased.test(static_cast<usize>(btn));
}

bool InputSystem::down(InputMouseBtn btn,
                       InputKeyModFlags mods) const noexcept {
  return (state_.mods & mods) == mods &&
         state_.mouseDown.test(static_cast<usize>(btn));
}

bool InputSystem::on(InputEdge edge, InputMouseBtn btn,
                     InputKeyModFlags mods) const noexcept {
  switch (edge) {
    using enum InputEdge;

    case Press:
      return pressed(btn, mods);
    case Release:
      return released(btn, mods);
    case Down:
      return down(btn, mods);
    default:
      return false;
  }
}

bool InputSystem::pressed(InputControllerId cid,
                          InputControllerBtn btn) const noexcept {
  return cid != kInvalidInputControllerId && state_.controllers[cid].active &&
         state_.controllers[cid].pressed.test(static_cast<usize>(btn));
}

bool InputSystem::released(InputControllerId cid,
                           InputControllerBtn btn) const noexcept {
  return cid != kInvalidInputControllerId && state_.controllers[cid].active &&
         state_.controllers[cid].released.test(static_cast<usize>(btn));
}

bool InputSystem::down(InputControllerId cid,
                       InputControllerBtn btn) const noexcept {
  return cid != kInvalidInputControllerId && state_.controllers[cid].active &&
         state_.controllers[cid].down.test(static_cast<usize>(btn));
}

bool InputSystem::on(InputEdge edge, InputControllerId cid,
                     InputControllerBtn btn) const noexcept {
  switch (edge) {
    using enum InputEdge;

    case Press:
      return pressed(cid, btn);
    case Release:
      return released(cid, btn);
    case Down:
      return down(cid, btn);
    default:
      return false;
  }
}

InputControllerAxisPos InputSystem::axis(InputControllerId cid,
                                         InputControllerAxis a) const noexcept {
  if (cid == kInvalidInputControllerId) return .0f;
  const auto& c = state_.controllers[cid];
  if (!c.active) return .0f;

  switch (a) {
    using enum InputControllerAxis;

    case LeftX:
      return c.axes[0];
    case LeftY:
      return c.axes[1];
    case RightX:
      return c.axes[2];
    case RightY:
      return c.axes[3];
    case LT:
      return c.axes[4];
    case RT:
      return c.axes[5];
    default:
      return .0f;
  }
}

bool InputSystem::controllerActive(InputControllerId cid) const noexcept {
  if (!validController(cid)) return false;
  return state_.controllers[cid].active;
}

bool InputSystem::gamepadActive(InputControllerId cid) const noexcept {
  if (!validController(cid)) return false;
  const auto& c = state_.controllers[cid];
  return c.active && c.gamepad;
}

bool InputSystem::gamepad(InputControllerId cid) const noexcept {
  if (!validController(cid)) return false;
  const auto& c = state_.controllers[cid];
  return c.gamepad;
}

void InputSystem::cursorMode(InputMouseCursorMode m) {
  if (!platform_ || !platform_->window) return;
  auto glfw = static_cast<s32>(internal::mouseCursorModeToPlatform(m));
  if (glfwGetInputMode(platform_->window, GLFW_CURSOR) == glfw) return;

  glfwSetInputMode(platform_->window, GLFW_CURSOR, glfw);

  if (m == InputMouseCursorMode::Disabled &&
      glfwRawMouseMotionSupported() == GLFW_TRUE) {
    glfwSetInputMode(platform_->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  } else {
    glfwSetInputMode(platform_->window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
  }
}

InputMouseCursorMode InputSystem::cursorMode() const noexcept {
  if (!platform_ || !platform_->window) return InputMouseCursorMode::Unknown;
  return internal::platformMouseCursorMode(
      static_cast<u64>(glfwGetInputMode(platform_->window, GLFW_CURSOR)));
}

void InputSystem::OnControllerHotPlug(InputControllerId cid) {
  if (!validController(cid) || !internal::gSelf) return;
  auto& s = internal::gSelf->state_;
  auto& c = s.controllers[cid];
  internal::gSelf->initController(cid, c);
  internal::gSelf->handleControllerHotplug(!s.activeControllers.empty());
}

void InputSystem::initControllers() {
  auto controllerMode = false;

  for (auto jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; ++jid) {
    auto cid = static_cast<InputControllerId>(jid);
    if (!validController(cid)) continue;
    auto& c = state_.controllers[cid];
    initController(cid, c);
    controllerMode |= c.active;
  }

  state_.controllerMode = controllerMode;
}

void InputSystem::initController(InputControllerId cid,
                                 internal::InputControllerState& c) {
  c = {};
  c.id = cid;
  auto glfwId = GLFW_JOYSTICK_1 + static_cast<s32>(cid);

  c.active = glfwJoystickPresent(glfwId) == GLFW_TRUE;
  c.gamepad = glfwJoystickIsGamepad(glfwId) == GLFW_TRUE;
  c.lastActivity = glfwGetTime();

  if (c.active) {
    if (const auto* guid = glfwGetJoystickGUID(glfwId)) c.guid = guid;
    if (const auto* name = glfwGetJoystickName(glfwId)) c.name = name;
    pushBackUnique(state_.activeControllers, c.id);
  } else {
    eraseUnordered(state_.activeControllers, c.id);
  }

  RL_EVENTSYS.dispatch(kInputMessageIdControllerHotPlug, c.id, c.active);
}

void InputSystem::bindCallbacks() {
  auto* window = platform_->window;
  glfwSetWindowUserPointer(window, this);

  platform_->onKeyboard = glfwSetKeyCallback(
      window, [](auto* window, auto gKey, auto, auto gEdge, auto) {
        auto* sys = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
        if (!sys) return;

        auto key = internal::platformToInputKeyCode(static_cast<u64>(gKey));
        auto edge = internal::platformToInputEdge(static_cast<u64>(gEdge));
        sys->handleControllerHotplug(false);

        switch (edge) {
          using enum InputEdge;
          case Press: {
            sys->state_.keyPressed.set(static_cast<usize>(key));
            sys->state_.keyDown.set(static_cast<usize>(key));
            break;
          }
          case Release: {
            sys->state_.keyReleased.set(static_cast<usize>(key));
            sys->state_.keyDown.reset(static_cast<usize>(key));
            break;
          }
          case Down:
          case Unknown:
          default:
            break;
        }
      });

  platform_->onMouseBtn = glfwSetMouseButtonCallback(
      window, [](auto* window, auto gBtn, auto gEdge, auto) {
        auto* sys = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
        if (!sys) return;

        auto btn = internal::platformToInputMouseBtn(static_cast<u64>(gBtn));
        auto edge = internal::platformToInputEdge(static_cast<u64>(gEdge));
        sys->handleControllerHotplug(false);

        switch (edge) {
          using enum InputEdge;
          case Press: {
            sys->state_.mousePressed.set(static_cast<usize>(btn));
            sys->state_.mouseDown.set(static_cast<usize>(btn));
            break;
          }
          case Release: {
            sys->state_.mouseReleased.set(static_cast<usize>(btn));
            sys->state_.mouseDown.reset(static_cast<usize>(btn));
            break;
          }
          case Down:
          case Unknown:
          default:
            break;
        }
      });

  f64 x;
  f64 y;
  glfwGetCursorPos(platform_->window, &x, &y);

  platform_->lastMousePos = {static_cast<InputMousePosUnit>(x),
                             static_cast<InputMousePosUnit>(y)};
  state_.mousePos = platform_->lastMousePos;

  platform_->onMousePos =
      glfwSetCursorPosCallback(window, [](auto* window, auto x, auto y) {
        auto* sys = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
        if (!sys) return;

        InputMousePos cur{static_cast<InputMousePosUnit>(x),
                          static_cast<InputMousePosUnit>(y)};
        const auto& viewport = RL_RENDERSYS.viewport();
        const auto& offset = viewport.pos;
        const auto& size = viewport.size;
        cur.x -= offset.x;
        cur.y -= offset.y;
        auto mouseOob = false;
        // Prevent updates out of viewport.
        auto prevMouseOob = sys->platform_->mouseOob;

        if (cur.x < 0) {
          cur.x = 0;
          if (prevMouseOob) return;
          mouseOob = true;
        }

        if (cur.y < 0) {
          cur.y = 0;
          if (prevMouseOob) return;
          mouseOob = true;
        }

        if (cur.x > size.x) {
          cur.x = size.x;
          if (prevMouseOob) return;
          mouseOob = true;
        }

        if (cur.y > size.y) {
          cur.y = size.y;
          if (prevMouseOob) return;
          mouseOob = true;
        }

        InputMousePos prev = sys->platform_->lastMousePos;
        if (nearlyEqual(cur, prev)) return;

        sys->state_.mouseDelta.x += (cur.x - prev.x);
        sys->state_.mouseDelta.y += (cur.y - prev.y);
        sys->state_.mousePos = cur;
        sys->platform_->lastMousePos = cur;
        sys->platform_->mouseOob = mouseOob;
      });

  platform_->onMouseScroll =
      glfwSetScrollCallback(window, [](auto* window, auto sx, auto sy) {
        auto* sys = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
        if (!sys) return;

        sys->state_.mouseScroll.x += static_cast<InputMouseScrollPos>(sx);
        sys->state_.mouseScroll.y += static_cast<InputMouseScrollPos>(sy);
      });

  platform_->onControllerHotPlug = glfwSetJoystickCallback([](auto gCid, auto) {
    auto cid = static_cast<InputControllerId>(gCid);
    InputSystem::OnControllerHotPlug(cid);
  });

  platform_->onWindowFocus =
      glfwSetWindowFocusCallback(window, [](auto*, auto focused) {
        RL_EVENTSYS.dispatch(kInputMessageIdWindowFocus,
                             static_cast<bool>(focused));
      });
}

void InputSystem::unbindCallbacks() {
  if (!platform_) return;
  auto* window = platform_->window;
  if (!window) return;

  glfwSetKeyCallback(window, nullptr);
  glfwSetMouseButtonCallback(window, nullptr);
  glfwSetCursorPosCallback(window, nullptr);
  glfwSetScrollCallback(window, nullptr);
  glfwSetWindowFocusCallback(window, nullptr);
  glfwSetJoystickCallback(nullptr);
  glfwSetWindowUserPointer(window, nullptr);

  platform_->onKeyboard = nullptr;
  platform_->onMouseBtn = nullptr;
  platform_->onMousePos = nullptr;
  platform_->onMouseScroll = nullptr;
  platform_->onControllerHotPlug = nullptr;
  platform_->onWindowFocus = nullptr;
}

void InputSystem::pollControllers() {
  for (InputControllerId cid = 0; cid < kMaxControllerCount; ++cid) {
    auto& c = state_.controllers[cid];
    auto glfwId = GLFW_JOYSTICK_1 + static_cast<s32>(cid);
    if (!c.active || !c.gamepad) continue;
    if (!glfwJoystickPresent(glfwId)) continue;  // Wait for hotplug callback.

    RL_ASSERT(
        glfwJoystickPresent(glfwId) == (c.active ? GLFW_TRUE : GLFW_FALSE),
        "InputSystem::pollControllers: Active controllers mismatch!");

    GLFWgamepadstate s{};
    RL_FASSERT(glfwGetGamepadState(glfwId, &s) == GLFW_TRUE,
               "InputSystem::pollControllers: Unable to get controller #", cid,
               " state!");

    auto& state = state_.controllers[cid];
    auto activity = false;

    for (usize btn = 1;
         btn < static_cast<usize>(InputControllerBtn::LeftStickUp); ++btn) {
      auto active =
          s.buttons[static_cast<s32>(internal::inputToPlatformControllerBtn(
              static_cast<InputControllerBtn>(btn)))] != 0;

      state.updateBtn(static_cast<InputControllerBtn>(btn), active);
      activity |= active;
    }

    for (usize axis = 0; axis < internal::kMaxControllerAxisCount; ++axis) {
      auto v = s.axes[axis];
      state.axes[axis] = v;
      activity |=
          std::fabs(state.axes[axis]) > internal::kControllerStickDeadZone;

      if (axis < 4) {
        InputControllerBtn start;
        InputControllerBtn end;

        if (!internal::axisToStickControllerBtn(axis, start, end)) {
          continue;
        }

        state.updateBtn(start, state.stickActive(start, -v));
        state.updateBtn(end, state.stickActive(end, v));
      } else {
        InputControllerBtn trigBtn;

        if (!internal::axisToTriggerControllerBtn(axis, trigBtn)) {
          continue;
        }

        state.updateBtn(trigBtn, v > internal::kControllerTriggerDeadzone);
      }
    }

    if (activity) c.lastActivity = glfwGetTime();
  }
}

void InputSystem::handleControllerHotplug(bool controllerMode) {
  if (state_.controllerMode != controllerMode) {
    state_.controllerMode = controllerMode;
    RL_EVENTSYS.dispatch(kInputMessageIdControllerMode, state_.controllerMode);
  }
}
}  // namespace rl