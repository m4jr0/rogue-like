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

#ifndef ENGINE_INPUT_ACTION_H_
#define ENGINE_INPUT_ACTION_H_

#include "engine/common.h"
#include "engine/input/input.h"
#include "engine/player/player.h"

namespace rl {
using ActionScopeTag = u8;
constexpr auto kMaxActionScopeTagCount = static_cast<u8>(-1) / 2;
using ActionScope = u8;
constexpr auto kMaxActionScopeCount = static_cast<ActionScope>(-1);
constexpr auto kInvalidActionScope = static_cast<ActionScope>(-1);

constexpr auto kDefaultDeadzone = .15f;   // Kill noise.
constexpr auto kDefaultSaturation = .9f;  // When is max hit?
constexpr auto kDefaultScale = 1.0f;
constexpr auto kDefaultCurve = 1.0f;  // Response curve (soft start, etc.).

[[nodiscard]] inline constexpr ActionScope actionScope(ActionScopeTag t,
                                                       PlayerSlot s) noexcept {
  return static_cast<ActionScope>(t & 0xf) << 4 |
         static_cast<ActionScope>(s & 0xf);
}

[[nodiscard]] inline constexpr ActionScopeTag tagFromActionScope(
    ActionScope s) noexcept {
  return static_cast<ActionScopeTag>((s >> 4) & 0xf);
}

[[nodiscard]] inline constexpr PlayerSlot playerSlotFromActionScope(
    ActionScope s) noexcept {
  return static_cast<PlayerSlot>(s & 0xf);
}

using ActionTag = u16;
constexpr auto kInvalidActionTag = static_cast<ActionTag>(-1);

using ActionKey = u32;
constexpr auto kInvalidActionKey = static_cast<ActionKey>(-1);

[[nodiscard]] inline constexpr ActionKey actionKey(ActionScope s,
                                                   ActionTag t) noexcept {
  RL_ASSERT(t != kInvalidActionTag, "actionKey: Invalid action tag provided!");
  return static_cast<ActionKey>(s) << 16 | static_cast<ActionKey>(t);
}

[[nodiscard]] inline constexpr ActionScope actionScopeFromKey(
    ActionKey k) noexcept {
  return static_cast<ActionScope>(k >> 16);
}

[[nodiscard]] inline constexpr ActionTag actionTagFromKey(
    ActionKey k) noexcept {
  return static_cast<ActionTag>(k & 0xffff);
}

struct ActionDigitalBtnBinding {
  ActionTag actionTag{kInvalidActionTag};
  InputDevice device{InputDevice::Unknown};
  InputEdge edge{InputEdge::Unknown};

  union {
    struct {
      InputKeyCode key;
      InputKeyModFlags mods;
    } keyboard;
    struct {
      InputMouseBtn btn;
      InputKeyModFlags mods;
    } mouse;
    struct {
      InputControllerBtn btn;
    } gamepad;
  };

  template <class AT>
    requires std::is_enum_v<AT>
  static constexpr ActionDigitalBtnBinding fromKeyboard(
      AT tag, InputEdge edge, InputKeyCode key,
      InputKeyModFlags mods = kInputKeyModFlagBitsNone) {
    auto t = static_cast<ActionTag>(tag);
    RL_ASSERT(
        t != kInvalidActionTag,
        "ActionDigitalBtnBinding::fromKeyboard: Invalid action tag provided!");
    return {.actionTag = t,
            .device = InputDevice::Keyboard,
            .edge = edge,
            .keyboard = {.key = key, .mods = mods}};
  }

  template <class AT>
    requires std::is_enum_v<AT>
  static constexpr ActionDigitalBtnBinding fromMouse(
      AT tag, InputEdge edge, InputMouseBtn btn,
      InputKeyModFlags mods = kInputKeyModFlagBitsNone) {
    auto t = static_cast<ActionTag>(tag);
    RL_ASSERT(
        t != kInvalidActionTag,
        "ActionDigitalBtnBinding::fromMouse: Invalid action tag provided!");
    return {.actionTag = t,
            .device = InputDevice::Mouse,
            .edge = edge,
            .mouse = {.btn = btn, .mods = mods}};
  }

  template <class AT>
    requires std::is_enum_v<AT>
  static constexpr ActionDigitalBtnBinding fromController(
      AT tag, InputEdge edge, InputControllerBtn btn) {
    auto t = static_cast<ActionTag>(tag);
    RL_ASSERT(t != kInvalidActionTag,
              "ActionDigitalBtnBinding::fromController: Invalid action tag "
              "provided!");
    return {.actionTag = t,
            .device = InputDevice::Controller,
            .edge = edge,
            .gamepad = {.btn = btn}};
  }
};

struct ActionAnalogBtnBinding {
  ActionTag actionTag{kInvalidActionTag};
  InputDevice device{InputDevice::Unknown};
  f32 deadzone{kDefaultDeadzone};
  f32 saturation{kDefaultSaturation};
  f32 scale{kDefaultScale};
  f32 curve{kDefaultCurve};

  union {
    struct {
      InputControllerAxis axis;
      bool positiveOnly;
    } gamepad;

    struct {
      bool horizontal;
    } mouse;
  };

  template <class AT>
    requires std::is_enum_v<AT>
  static constexpr ActionAnalogBtnBinding fromMouse(AT tag, bool h,
                                                    f32 dz = .0f,
                                                    f32 sat = FLT_MAX,
                                                    f32 s = kDefaultScale,
                                                    f32 c = kDefaultCurve) {
    auto t = static_cast<ActionTag>(tag);
    RL_ASSERT(
        t != kInvalidActionTag,
        "ActionAnalogBtnBinding::fromMouse: Invalid action tag provided!");
    RL_ASSERT(dz >= .0f,
              "ActionAnalogBtnBinding::fromMouse: Deadzone can't be negative!");
    RL_ASSERT(sat > dz, "ActionAnalogBtnBinding::fromMouse: Saturation (", sat,
              ") has to be greater than deadzone (", dz, ")!");
    return {.actionTag = t,
            .device = InputDevice::Mouse,
            .deadzone = dz,
            .saturation = sat,
            .scale = s,
            .curve = c,
            .mouse = {.horizontal = h}};
  }

  template <class AT>
    requires std::is_enum_v<AT>
  static constexpr ActionAnalogBtnBinding fromController(
      AT tag, InputControllerAxis a, f32 dz = kDefaultDeadzone,
      f32 sat = kDefaultSaturation, f32 s = kDefaultScale,
      f32 c = kDefaultCurve, bool posOnly = false) {
    auto t = static_cast<ActionTag>(tag);
    RL_ASSERT(
        t != kInvalidActionTag,
        "ActionAnalogBtnBinding::fromController: Invalid action tag provided!");
    RL_ASSERT(
        dz >= .0f,
        "ActionAnalogBtnBinding::fromController: Deadzone can't be negative!");
    RL_ASSERT(sat > dz, "ActionAnalogBtnBinding::fromController: Saturation (",
              sat, ") has to be greater than deadzone (", dz, ")!");
    return {.actionTag = t,
            .device = InputDevice::Controller,
            .deadzone = dz,
            .saturation = sat,
            .scale = s,
            .curve = c,
            .gamepad = {.axis = a, .positiveOnly = posOnly}};
  }
};

struct ActionAnalogAxisBinding {
  ActionTag actionTag{kInvalidActionTag};
  InputDevice device{InputDevice::Unknown};

  f32 deadzone{kDefaultDeadzone};
  f32 saturation{kDefaultSaturation};
  f32 scale{kDefaultScale};
  f32 curve{kDefaultCurve};

  union {
    struct {
      InputControllerAxis x;
      InputControllerAxis y;
      bool invertX;
      bool invertY;
    } gamepad;

    struct {
      bool useDelta;
      f32 sensitivityX;
      f32 sensitivityY;
      bool invertX;
      bool invertY;
    } mouse;
  };

  template <class AT>
    requires std::is_enum_v<AT>
  static constexpr ActionAnalogAxisBinding fromController(
      AT tag, InputControllerAxis ax, InputControllerAxis ay,
      f32 dz = kDefaultDeadzone, f32 sat = kDefaultSaturation,
      f32 s = kDefaultScale, f32 c = kDefaultCurve, bool invX = false,
      bool invY = true) {
    auto t = static_cast<ActionTag>(tag);
    RL_ASSERT(t != kInvalidActionTag,
              "ActionAnalogAxisBinding::fromController: Invalid action tag "
              "provided!");
    RL_ASSERT(
        dz >= .0f,
        "ActionAnalogAxisBinding::fromController: Deadzone can't be negative!");
    RL_ASSERT(sat > dz, "ActionAnalogAxisBinding::fromController: Saturation (",
              sat, ") has to be greater than deadzone (", dz, ")!");
    return {
        .actionTag = t,
        .device = InputDevice::Controller,
        .deadzone = dz,
        .saturation = sat,
        .scale = s,
        .curve = c,
        .gamepad =
            {
                .x = ax,
                .y = ay,
                .invertX = invX,
                .invertY = invY,
            },
    };
  }

  template <class AT>
    requires std::is_enum_v<AT>
  static constexpr ActionAnalogAxisBinding fromMouse(
      AT tag, bool useDelta = true, f32 sensX = 1.0f, f32 sensY = 1.0f,
      f32 dz = kDefaultDeadzone, f32 sat = kDefaultSaturation,
      f32 s = kDefaultScale, f32 c = kDefaultCurve, bool invX = false,
      bool invY = false) {
    auto t = static_cast<ActionTag>(tag);
    RL_ASSERT(
        t != kInvalidActionTag,
        "ActionAnalogAxisBinding::fromMouse: Invalid action tag provided!");
    RL_ASSERT(
        dz >= .0f,
        "ActionAnalogAxisBinding::fromMouse: Deadzone can't be negative!");
    RL_ASSERT(sat > dz, "ActionAnalogAxisBinding::fromMouse: Saturation (", sat,
              ") has to be greater than deadzone (", dz, ")!");
    return {
        .actionTag = t,
        .device = InputDevice::Mouse,
        .deadzone = dz,
        .saturation = sat,
        .scale = s,
        .curve = c,
        .mouse =
            {
                .useDelta = useDelta,
                .sensitivityX = sensX,
                .sensitivityY = sensY,
                .invertX = invX,
                .invertY = invY,
            },
    };
  }
};

struct ActionBindings {
  std::span<const ActionDigitalBtnBinding> digitalBtns{};
  std::span<const ActionAnalogBtnBinding> analogBtns{};
  std::span<const ActionAnalogAxisBinding> analogAxes{};
};

struct ActionSet {
  ActionScope scope{kInvalidActionScope};
  ActionBindings bindings{};
};

PlayerHandle fromScopeToPlayer(ActionScope scope);
InputControllerId fromScopeToController(ActionScope scope);
}  // namespace rl

#endif  // ENGINE_INPUT_ACTION_H_
