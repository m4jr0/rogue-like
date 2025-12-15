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
#include "input.h"
////////////////////////////////////////////////////////////////////////////////

namespace rl {
namespace internal {
InputKeyCode platformToInputKeyCode(u64 platform) {
  auto glfw = static_cast<s32>(platform);

  switch (glfw) {
    using enum InputKeyCode;
    case GLFW_KEY_UNKNOWN:
      return Unknown;
    case GLFW_KEY_SPACE:
      return Space;
    case GLFW_KEY_APOSTROPHE:
      return Apostrophe;
    case GLFW_KEY_COMMA:
      return Comma;
    case GLFW_KEY_MINUS:
      return Minus;
    case GLFW_KEY_PERIOD:
      return Period;
    case GLFW_KEY_SLASH:
      return Slash;
    case GLFW_KEY_0:
      return Zero;
    case GLFW_KEY_1:
      return One;
    case GLFW_KEY_2:
      return Two;
    case GLFW_KEY_3:
      return Three;
    case GLFW_KEY_4:
      return Four;
    case GLFW_KEY_5:
      return Five;
    case GLFW_KEY_6:
      return Six;
    case GLFW_KEY_7:
      return Seven;
    case GLFW_KEY_8:
      return Eight;
    case GLFW_KEY_9:
      return Nine;
    case GLFW_KEY_SEMICOLON:
      return Semicolon;
    case GLFW_KEY_EQUAL:
      return Equal;
    case GLFW_KEY_A:
      return A;
    case GLFW_KEY_B:
      return B;
    case GLFW_KEY_C:
      return C;
    case GLFW_KEY_D:
      return D;
    case GLFW_KEY_E:
      return E;
    case GLFW_KEY_F:
      return F;
    case GLFW_KEY_G:
      return G;
    case GLFW_KEY_H:
      return H;
    case GLFW_KEY_I:
      return I;
    case GLFW_KEY_J:
      return J;
    case GLFW_KEY_K:
      return K;
    case GLFW_KEY_L:
      return L;
    case GLFW_KEY_M:
      return M;
    case GLFW_KEY_N:
      return N;
    case GLFW_KEY_O:
      return O;
    case GLFW_KEY_P:
      return P;
    case GLFW_KEY_Q:
      return Q;
    case GLFW_KEY_R:
      return R;
    case GLFW_KEY_S:
      return S;
    case GLFW_KEY_T:
      return T;
    case GLFW_KEY_U:
      return U;
    case GLFW_KEY_V:
      return V;
    case GLFW_KEY_W:
      return W;
    case GLFW_KEY_X:
      return X;
    case GLFW_KEY_Y:
      return Y;
    case GLFW_KEY_Z:
      return Z;
    case GLFW_KEY_LEFT_BRACKET:
      return LeftBracket;
    case GLFW_KEY_BACKSLASH:
      return Backslash;
    case GLFW_KEY_RIGHT_BRACKET:
      return RightBracket;
    case GLFW_KEY_GRAVE_ACCENT:
      return GraveAcent;
    case GLFW_KEY_WORLD_1:
      return World1;
    case GLFW_KEY_WORLD_2:
      return World2;
    case GLFW_KEY_ESCAPE:
      return Escape;
    case GLFW_KEY_ENTER:
      return Enter;
    case GLFW_KEY_TAB:
      return Tab;
    case GLFW_KEY_BACKSPACE:
      return Backspace;
    case GLFW_KEY_INSERT:
      return Insert;
    case GLFW_KEY_DELETE:
      return Del;
    case GLFW_KEY_RIGHT:
      return Right;
    case GLFW_KEY_LEFT:
      return Left;
    case GLFW_KEY_DOWN:
      return Down;
    case GLFW_KEY_UP:
      return Up;
    case GLFW_KEY_PAGE_UP:
      return PageUp;
    case GLFW_KEY_PAGE_DOWN:
      return PageDown;
    case GLFW_KEY_HOME:
      return Home;
    case GLFW_KEY_END:
      return End;
    case GLFW_KEY_CAPS_LOCK:
      return CapsLock;
    case GLFW_KEY_SCROLL_LOCK:
      return ScrollLock;
    case GLFW_KEY_NUM_LOCK:
      return NumLock;
    case GLFW_KEY_PRINT_SCREEN:
      return PrintScreen;
    case GLFW_KEY_PAUSE:
      return Pause;
    case GLFW_KEY_F1:
      return F1;
    case GLFW_KEY_F2:
      return F2;
    case GLFW_KEY_F3:
      return F3;
    case GLFW_KEY_F4:
      return F4;
    case GLFW_KEY_F5:
      return F5;
    case GLFW_KEY_F6:
      return F6;
    case GLFW_KEY_F7:
      return F7;
    case GLFW_KEY_F8:
      return F8;
    case GLFW_KEY_F9:
      return F9;
    case GLFW_KEY_F10:
      return F10;
    case GLFW_KEY_F11:
      return F11;
    case GLFW_KEY_F12:
      return F12;
    case GLFW_KEY_F13:
      return F13;
    case GLFW_KEY_F14:
      return F14;
    case GLFW_KEY_F15:
      return F15;
    case GLFW_KEY_F16:
      return F16;
    case GLFW_KEY_F17:
      return F17;
    case GLFW_KEY_F18:
      return F18;
    case GLFW_KEY_F19:
      return F19;
    case GLFW_KEY_F20:
      return F20;
    case GLFW_KEY_F21:
      return F21;
    case GLFW_KEY_F22:
      return F22;
    case GLFW_KEY_F23:
      return F23;
    case GLFW_KEY_F24:
      return F24;
    case GLFW_KEY_F25:
      return F25;
    case GLFW_KEY_KP_0:
      return KP0;
    case GLFW_KEY_KP_1:
      return KeyPad1;
    case GLFW_KEY_KP_2:
      return KeyPad2;
    case GLFW_KEY_KP_3:
      return KeyPad3;
    case GLFW_KEY_KP_4:
      return KeyPad4;
    case GLFW_KEY_KP_5:
      return KeyPad5;
    case GLFW_KEY_KP_6:
      return KeyPad6;
    case GLFW_KEY_KP_7:
      return KeyPad7;
    case GLFW_KEY_KP_8:
      return KeyPad8;
    case GLFW_KEY_KP_9:
      return KeyPad9;
    case GLFW_KEY_KP_DECIMAL:
      return KeyPadDecimal;
    case GLFW_KEY_KP_DIVIDE:
      return KeyPadDivide;
    case GLFW_KEY_KP_MULTIPLY:
      return KeyPadMultiply;
    case GLFW_KEY_KP_SUBTRACT:
      return KeyPadSubtract;
    case GLFW_KEY_KP_ADD:
      return KeyPadAdd;
    case GLFW_KEY_KP_ENTER:
      return KeyPadEnter;
    case GLFW_KEY_KP_EQUAL:
      return KeyPadEqual;
    case GLFW_KEY_LEFT_SHIFT:
      return LeftShift;
    case GLFW_KEY_LEFT_CONTROL:
      return LeftControl;
    case GLFW_KEY_LEFT_ALT:
      return LeftAlt;
    case GLFW_KEY_LEFT_SUPER:
      return LeftSuper;
    case GLFW_KEY_RIGHT_SHIFT:
      return RightShift;
    case GLFW_KEY_RIGHT_CONTROL:
      return RightControl;
    case GLFW_KEY_RIGHT_ALT:
      return RightAlt;
    case GLFW_KEY_RIGHT_SUPER:
      return RightSuper;
    case GLFW_KEY_MENU:
      return Menu;
    default:
      return Unknown;
  }
}

InputMouseBtn platformToInputMouseBtn(u64 platform) {
  auto glfw = static_cast<s32>(platform);

  switch (glfw) {
    using enum InputMouseBtn;
    case GLFW_MOUSE_BUTTON_1:
      return Left;
    case GLFW_MOUSE_BUTTON_2:
      return Right;
    case GLFW_MOUSE_BUTTON_3:
      return Middle;
    case GLFW_MOUSE_BUTTON_4:
      return Other1;
    case GLFW_MOUSE_BUTTON_5:
      return Other2;
    case GLFW_MOUSE_BUTTON_6:
      return Other3;
    case GLFW_MOUSE_BUTTON_7:
      return Other4;
    default:
      return Unknown;
  }
}

InputControllerBtn platformToInputControllerBtn(u64 platform) {
  auto glfw = static_cast<s32>(platform);

  switch (glfw) {
    using enum InputControllerBtn;
    case GLFW_GAMEPAD_BUTTON_A:
      return A;
    case GLFW_GAMEPAD_BUTTON_B:
      return B;
    case GLFW_GAMEPAD_BUTTON_X:
      return X;
    case GLFW_GAMEPAD_BUTTON_Y:
      return Y;
    case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:
      return LeftBumper;
    case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:
      return RightBumper;
    case GLFW_GAMEPAD_BUTTON_BACK:
      return Back;
    case GLFW_GAMEPAD_BUTTON_START:
      return Start;
    case GLFW_GAMEPAD_BUTTON_GUIDE:
      return Guide;
    case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:
      return LeftStick;
    case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:
      return RightStick;
    case GLFW_GAMEPAD_BUTTON_DPAD_UP:
      return DPadUp;
    case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:
      return DPadLeft;
    case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:
      return DPadRight;
    case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:
      return DPadDown;
    default:
      return Unknown;
  }
}

bool axisToStickControllerBtn(usize axis, InputControllerBtn& start,
                              InputControllerBtn& end) {
  switch (axis) {
    case 0:
      start = InputControllerBtn::LeftStickLeft;
      end = InputControllerBtn::LeftStickRight;
      return true;
    case 1:
      start = InputControllerBtn::LeftStickUp;
      end = InputControllerBtn::LeftStickDown;
      return true;
    case 2:
      start = InputControllerBtn::RightStickLeft;
      end = InputControllerBtn::RightStickRight;
      return true;
    case 3:
      start = InputControllerBtn::RightStickUp;
      end = InputControllerBtn::RightStickDown;
      return true;
    default:
      start = InputControllerBtn::Unknown;
      end = InputControllerBtn::Unknown;
      return false;
  }
}

bool axisToTriggerControllerBtn(usize axis, InputControllerBtn& btn) {
  switch (axis) {
    case 4:
      btn = InputControllerBtn::LeftTrigger;
      return true;
    case 5:
      btn = InputControllerBtn::RightTrigger;
      return true;
    default:
      btn = InputControllerBtn::Unknown;
      return false;
  }
}

u64 inputToPlatformControllerBtn(InputControllerBtn btn) {
  switch (btn) {
    using enum InputControllerBtn;
    case A:
      return GLFW_GAMEPAD_BUTTON_A;
    case B:
      return GLFW_GAMEPAD_BUTTON_B;
    case X:
      return GLFW_GAMEPAD_BUTTON_X;
    case Y:
      return GLFW_GAMEPAD_BUTTON_Y;
    case LeftBumper:
      return GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
    case RightBumper:
      return GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
    case Back:
      return GLFW_GAMEPAD_BUTTON_BACK;
    case Start:
      return GLFW_GAMEPAD_BUTTON_START;
    case Guide:
      return GLFW_GAMEPAD_BUTTON_GUIDE;
    case LeftStick:
      return GLFW_GAMEPAD_BUTTON_LEFT_THUMB;
    case RightStick:
      return GLFW_GAMEPAD_BUTTON_RIGHT_THUMB;
    case DPadUp:
      return GLFW_GAMEPAD_BUTTON_DPAD_UP;
    case DPadLeft:
      return GLFW_GAMEPAD_BUTTON_DPAD_LEFT;
    case DPadRight:
      return GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;
    case DPadDown:
      return GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
    default:
      return GLFW_GAMEPAD_BUTTON_LAST;
  }
}

InputEdge platformToInputEdge(u64 platform) {
  auto glfw = static_cast<s32>(platform);

  switch (glfw) {
    case GLFW_PRESS:
      return InputEdge::Press;
    case GLFW_RELEASE:
      return InputEdge::Release;
    default:
      return InputEdge::Unknown;
  }
}

InputKeyModFlags platformToInputKeyMods(u64 platform) {
  auto glfw = static_cast<s32>(platform);

  InputKeyModFlags m = kInputKeyModFlagBitsNone;
  if (glfw & GLFW_MOD_SHIFT) m |= kInputKeyModFlagBitsShift;
  if (glfw & GLFW_MOD_CONTROL) m |= kInputKeyModFlagBitsControl;
  if (glfw & GLFW_MOD_ALT) m |= kInputKeyModFlagBitsAlt;
  if (glfw & GLFW_MOD_SUPER) m |= kInputKeyModFlagBitsSuper;
  if (glfw & GLFW_MOD_CAPS_LOCK) m |= kInputKeyModFlagBitsCapsLock;
  if (glfw & GLFW_MOD_NUM_LOCK) m |= kInputKeyModFlagBitsNumLock;
  return m;
}

InputMouseCursorMode platformMouseCursorMode(u64 platform) {
  auto glfw = static_cast<s32>(platform);

  switch (glfw) {
    using enum InputMouseCursorMode;

    case GLFW_CURSOR_NORMAL:
      return Normal;
    case GLFW_CURSOR_DISABLED:
      return Disabled;
    case GLFW_CURSOR_HIDDEN:
      return Hidden;
    default:
      return Unknown;
  }
}

u64 mouseCursorModeToPlatform(InputMouseCursorMode m) {
  switch (m) {
    using enum InputMouseCursorMode;

    case Normal:
      return GLFW_CURSOR_NORMAL;
    case Disabled:
      return GLFW_CURSOR_DISABLED;
    case Hidden:
      return GLFW_CURSOR_HIDDEN;
    default:
      return GLFW_CURSOR_NORMAL;
  }
}
}  // namespace internal
}  // namespace rl