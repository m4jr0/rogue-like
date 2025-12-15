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

#ifndef ENGINE_INPUT_INPUT_H_
#define ENGINE_INPUT_INPUT_H_

#include "engine/common.h"
#include "engine/math/vec2.h"

namespace rl {
enum class InputMouseCursorMode {
  Unknown = 0,
  Normal,
  Disabled,
  Hidden,
};

enum class InputKeyCode {
  Unknown = 0,
  Space,
  Apostrophe,
  Comma,
  Minus,
  Period,
  Slash,
  Zero,
  One,
  Two,
  Three,
  Four,
  Five,
  Six,
  Seven,
  Eight,
  Nine,
  Semicolon,
  Equal,
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  LeftBracket,
  Backslash,
  RightBracket,
  GraveAcent,
  World1,
  World2,
  Escape,
  Enter,
  Tab,
  Backspace,
  Insert,
  Del,
  Right,
  Left,
  Down,
  Up,
  PageUp,
  PageDown,
  Home,
  End,
  CapsLock,
  ScrollLock,
  NumLock,
  PrintScreen,
  Pause,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  F13,
  F14,
  F15,
  F16,
  F17,
  F18,
  F19,
  F20,
  F21,
  F22,
  F23,
  F24,
  F25,
  KP0,
  KeyPad1,
  KeyPad2,
  KeyPad3,
  KeyPad4,
  KeyPad5,
  KeyPad6,
  KeyPad7,
  KeyPad8,
  KeyPad9,
  KeyPadDecimal,
  KeyPadDivide,
  KeyPadMultiply,
  KeyPadSubtract,
  KeyPadAdd,
  KeyPadEnter,
  KeyPadEqual,
  LeftShift,
  LeftControl,
  LeftAlt,
  LeftSuper,
  RightShift,
  RightControl,
  RightAlt,
  RightSuper,
  Menu,
  Count,
};

enum class InputMouseBtn {
  Unknown = 0,
  Left,
  Right,
  Middle,
  Last,
  Other1,
  Other2,
  Other3,
  Other4,
  Count,
};

enum class InputControllerBtn {
  Unknown = 0,
  A,
  B,
  X,
  Y,
  LeftBumper,
  RightBumper,
  Back,
  Start,
  Guide,
  LeftStick,
  RightStick,
  DPadUp,
  DPadLeft,
  DPadRight,
  DPadDown,
  LeftStickUp,
  LeftStickDown,
  LeftStickLeft,
  LeftStickRight,
  RightStickUp,
  RightStickDown,
  RightStickLeft,
  RightStickRight,
  LeftTrigger,
  RightTrigger,
  Count,
};

using InputControllerId = u32;
constexpr auto kInvalidInputControllerId = static_cast<InputControllerId>(-1);
constexpr InputControllerId kMaxControllerCount = 16;

inline constexpr bool validController(InputControllerId cid) {
  return cid < kMaxControllerCount;
}

enum class InputEdge { Unknown = 0, Press, Release, Down };

using InputKeyModFlags = u8;

enum InputKeyModFlagBits : InputKeyModFlags {
  kInputKeyModFlagBitsNone = 0x0,
  kInputKeyModFlagBitsShift = 0x1,
  kInputKeyModFlagBitsControl = 0x2,
  kInputKeyModFlagBitsAlt = 0x4,
  kInputKeyModFlagBitsSuper = 0x8,
  kInputKeyModFlagBitsCapsLock = 0x10,
  kInputKeyModFlagBitsNumLock = 0x20,
  kInputKeyModFlagBitsAll = static_cast<InputKeyModFlags>(-1),
};

using InputMousePosUnit = f32;
using InputMousePos = Point<InputMousePosUnit>;
using InputMouseScrollPos = f32;
using InputMouseScroll = Point<InputMouseScrollPos>;

using InputControllerAxisPos = f32;

enum class InputControllerAxis : u8 { LeftX, LeftY, RightX, RightY, LT, RT };

enum class InputDevice { Unknown = 0, Keyboard, Mouse, Controller };

namespace internal {
constexpr auto kMaxKeyCount = static_cast<usize>(InputKeyCode::Count);
constexpr auto kMaxMouseBtnCount = static_cast<usize>(InputMouseBtn::Count);
constexpr auto kMaxControllerBtnCount =
    static_cast<usize>(InputControllerBtn::Count);
constexpr auto kMaxControllerAxisCount = 6;
constexpr auto kControllerStickDeadZone = .25f;
constexpr float kControllerStickHysteresis = .05f;
constexpr float kControllerTriggerDeadzone = .20f;

struct InputControllerState {
  bool active{false};
  bool gamepad{false};

  InputControllerId id{kInvalidInputControllerId};
  std::string guid{};
  std::string name{};
  f64 lastActivity{.0};

  std::bitset<kMaxControllerBtnCount> down;
  std::bitset<kMaxControllerBtnCount> pressed;
  std::bitset<kMaxControllerBtnCount> released;

  std::array<InputControllerAxisPos, kMaxControllerAxisCount> axes{};

  void newFrame() {
    pressed.reset();
    released.reset();
  }

  void updateBtn(InputControllerBtn btn, bool nowActive) {
    auto cBtn = static_cast<usize>(btn);
    auto wasActive = down.test(cBtn);

    if (nowActive && !wasActive) {
      pressed.set(cBtn);
      down.set(cBtn);
    }

    if (!nowActive && wasActive) {
      released.set(cBtn);
      down.reset(cBtn);
    }
  }

  bool stickActive(InputControllerBtn btn, f32 v) const {
    auto wasActive = down.test(static_cast<usize>(btn));

    if (!wasActive) {
      auto enter = internal::kControllerStickDeadZone;
      return v > enter;
    }

    auto exit = std::max(.0f, internal::kControllerStickDeadZone -
                                  internal::kControllerStickHysteresis);
    return v > exit;
  }
};

struct InputState {
  bool controllerMode{false};

  InputKeyModFlags mods{kInputKeyModFlagBitsNone};
  InputMousePos mousePos{InputMousePos::zero()};
  InputMousePos mouseDelta{InputMousePos::zero()};
  InputMouseScroll mouseScroll{InputMouseScroll::zero()};

  std::bitset<kMaxKeyCount> keyDown;
  std::bitset<kMaxKeyCount> keyPressed;
  std::bitset<kMaxKeyCount> keyReleased;

  std::bitset<kMaxMouseBtnCount> mouseDown;
  std::bitset<kMaxMouseBtnCount> mousePressed;
  std::bitset<kMaxMouseBtnCount> mouseReleased;

  std::array<InputControllerState, kMaxControllerCount> controllers{};
  std::vector<InputControllerId> activeControllers{};

  void newFrame() {
    keyPressed.reset();
    keyReleased.reset();
    mousePressed.reset();
    mouseReleased.reset();

    for (auto& state : controllers) {
      state.newFrame();
    }

    mouseDelta = InputMousePos::zero();
    mouseScroll = InputMouseScroll::zero();
  }

  void resetMods() {
    mods = kInputKeyModFlagBitsNone;

    if (keyDown.test((usize)InputKeyCode::LeftShift) ||
        keyDown.test((usize)InputKeyCode::RightShift))
      mods |= kInputKeyModFlagBitsShift;
    if (keyDown.test((usize)InputKeyCode::LeftControl) ||
        keyDown.test((usize)InputKeyCode::RightControl))
      mods |= kInputKeyModFlagBitsControl;
    if (keyDown.test((usize)InputKeyCode::LeftAlt) ||
        keyDown.test((usize)InputKeyCode::RightAlt))
      mods |= kInputKeyModFlagBitsAlt;
    if (keyDown.test((usize)InputKeyCode::LeftSuper) ||
        keyDown.test((usize)InputKeyCode::RightSuper))
      mods |= kInputKeyModFlagBitsSuper;
  }
};

InputKeyCode platformToInputKeyCode(u64 platform);
InputMouseBtn platformToInputMouseBtn(u64 platform);
InputControllerBtn platformToInputControllerBtn(u64 platform);
bool axisToStickControllerBtn(usize axis, InputControllerBtn& start,
                              InputControllerBtn& end);
bool axisToTriggerControllerBtn(usize axis, InputControllerBtn& btn);
u64 inputToPlatformControllerBtn(InputControllerBtn btn);
InputEdge platformToInputEdge(u64 platform);
InputKeyModFlags platformToInputKeyMods(u64 platform);
InputMouseCursorMode platformMouseCursorMode(u64 platform);
u64 mouseCursorModeToPlatform(InputMouseCursorMode m);
}  // namespace internal
}  // namespace rl

#endif  // ENGINE_INPUT_INPUT_H_
