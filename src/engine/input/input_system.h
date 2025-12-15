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

#ifndef ENGINE_INPUT_INPUT_SYSTEM_H_
#define ENGINE_INPUT_INPUT_SYSTEM_H_

#include "engine/common.h"
#include "engine/input/input.h"

namespace rl {
class InputSystem {
 public:
  static InputSystem& instance();

  void init(void* window);
  void shutdown();

  void poll();

  bool pressed(InputKeyCode key,
               InputKeyModFlags mods = kInputKeyModFlagBitsNone) const noexcept;
  bool released(InputKeyCode key, InputKeyModFlags mods =
                                      kInputKeyModFlagBitsNone) const noexcept;
  bool down(InputKeyCode key,
            InputKeyModFlags mods = kInputKeyModFlagBitsNone) const noexcept;
  bool on(InputEdge edge, InputKeyCode key,
          InputKeyModFlags mods = kInputKeyModFlagBitsNone) const noexcept;

  bool pressed(InputMouseBtn btn,
               InputKeyModFlags mods = kInputKeyModFlagBitsNone) const noexcept;
  bool released(InputMouseBtn btn, InputKeyModFlags mods =
                                       kInputKeyModFlagBitsNone) const noexcept;
  bool down(InputMouseBtn btn,
            InputKeyModFlags mods = kInputKeyModFlagBitsNone) const noexcept;
  bool on(InputEdge edge, InputMouseBtn btn,
          InputKeyModFlags mods = kInputKeyModFlagBitsNone) const noexcept;

  bool pressed(InputControllerId cid, InputControllerBtn btn) const noexcept;
  bool released(InputControllerId cid, InputControllerBtn btn) const noexcept;
  bool down(InputControllerId cid, InputControllerBtn btn) const noexcept;
  bool on(InputEdge edge, InputControllerId cid,
          InputControllerBtn btn) const noexcept;

  InputMousePos mousePosition() const noexcept { return state_.mousePos; }
  InputMousePos mouseDelta() const noexcept { return state_.mouseDelta; }
  InputMouseScroll scrollDelta() const noexcept { return state_.mouseScroll; }

  bool controllerMode() const noexcept { return state_.controllerMode; }

  std::span<const InputControllerId> activeControllers() const noexcept {
    return state_.activeControllers;
  }

  InputControllerAxisPos axis(InputControllerId cid,
                              InputControllerAxis a) const noexcept;

  bool controllerActive(InputControllerId cid) const noexcept;
  bool gamepadActive(InputControllerId cid) const noexcept;
  bool gamepad(InputControllerId cid) const noexcept;

  void cursorMode(InputMouseCursorMode m);
  InputMouseCursorMode cursorMode() const noexcept;

  void enableCursor(bool active = true) {
    cursorMode(active ? InputMouseCursorMode::Normal
                      : InputMouseCursorMode::Disabled);
  }

  void disableCursor() { enableCursor(false); }

  void showCursor(bool active = true) {
    cursorMode(active ? InputMouseCursorMode::Normal
                      : InputMouseCursorMode::Hidden);
  }

  void hideCursor() { showCursor(false); }

 private:
  struct PlatformCtx;
  internal::InputState state_{};
  std::unique_ptr<PlatformCtx> platform_{nullptr};
  void* window_{nullptr};

  static void OnControllerHotPlug(InputControllerId cid);

  InputSystem() = default;

  void initControllers();
  void initController(InputControllerId cid, internal::InputControllerState& c);
  void bindCallbacks();
  void unbindCallbacks();
  void pollControllers();
  void handleControllerHotplug(bool controllerMode);
};
}  // namespace rl

#define RL_INPUTSYS (::rl::InputSystem::instance())
#define RL_CINPUTSYS \
  (static_cast<const ::rl::InputSystem&>(::rl::InputSystem::instance()))

#endif  // ENGINE_INPUT_INPUT_SYSTEM_H_
