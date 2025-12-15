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

#ifndef GAME_CHARACTER_CLASS_CHARACTER_CLASS_DEFAULT_BINDINGS_H_
#define GAME_CHARACTER_CLASS_CHARACTER_CLASS_DEFAULT_BINDINGS_H_

#include "game/action_scope.h"
#include "game/character/character.h"

namespace rl {
namespace internal {
static constexpr ActionDigitalBtnBinding kKeyboardDigitalBtn[] = {
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Up, InputEdge::Down,
                                          InputKeyCode::W),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Down,
                                          InputEdge::Down, InputKeyCode::S),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Left,
                                          InputEdge::Down, InputKeyCode::A),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Right,
                                          InputEdge::Down, InputKeyCode::D),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Up, InputEdge::Down,
                                          InputKeyCode::Up),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Down,
                                          InputEdge::Down, InputKeyCode::Down),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Left,
                                          InputEdge::Down, InputKeyCode::Left),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Right,
                                          InputEdge::Down, InputKeyCode::Right),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Ability0,
                                          InputEdge::Press, InputKeyCode::J),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Ability1,
                                          InputEdge::Press, InputKeyCode::K),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Ability2,
                                          InputEdge::Press, InputKeyCode::L),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Ability3,
                                          InputEdge::Press, InputKeyCode::N),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Ability4,
                                          InputEdge::Press, InputKeyCode::M),
    ActionDigitalBtnBinding::fromKeyboard(
        CharInputAction::Ability5, InputEdge::Press, InputKeyCode::Comma),
    ActionDigitalBtnBinding::fromKeyboard(
        CharInputAction::Ability6, InputEdge::Press, InputKeyCode::LeftBracket),
    ActionDigitalBtnBinding::fromKeyboard(CharInputAction::Ability7,
                                          InputEdge::Press,
                                          InputKeyCode::RightBracket),
};

static constexpr ActionDigitalBtnBinding kMouseDigitalBtn[] = {
    ActionDigitalBtnBinding::fromMouse(CharInputAction::Ability0,
                                       InputEdge::Press, InputMouseBtn::Left),
    ActionDigitalBtnBinding::fromMouse(CharInputAction::Ability1,
                                       InputEdge::Press, InputMouseBtn::Right),
};

static constexpr ActionDigitalBtnBinding kControllerDigitalBtn[] = {
    ActionDigitalBtnBinding::fromController(
        CharInputAction::Up, InputEdge::Down, InputControllerBtn::DPadUp),
    ActionDigitalBtnBinding::fromController(
        CharInputAction::Down, InputEdge::Down, InputControllerBtn::DPadDown),
    ActionDigitalBtnBinding::fromController(
        CharInputAction::Left, InputEdge::Down, InputControllerBtn::DPadLeft),
    ActionDigitalBtnBinding::fromController(
        CharInputAction::Right, InputEdge::Down, InputControllerBtn::DPadRight),
    ActionDigitalBtnBinding::fromController(
        CharInputAction::Ability0, InputEdge::Press, InputControllerBtn::A),
    ActionDigitalBtnBinding::fromController(
        CharInputAction::Ability1, InputEdge::Press, InputControllerBtn::B),
    ActionDigitalBtnBinding::fromController(
        CharInputAction::Ability2, InputEdge::Press, InputControllerBtn::X),
    ActionDigitalBtnBinding::fromController(
        CharInputAction::Ability3, InputEdge::Press, InputControllerBtn::Y),
    ActionDigitalBtnBinding::fromController(CharInputAction::Ability4,
                                            InputEdge::Press,
                                            InputControllerBtn::LeftBumper),
    ActionDigitalBtnBinding::fromController(CharInputAction::Ability5,
                                            InputEdge::Press,
                                            InputControllerBtn::RightBumper),
    ActionDigitalBtnBinding::fromController(CharInputAction::Ability6,
                                            InputEdge::Press,
                                            InputControllerBtn::LeftTrigger),
    ActionDigitalBtnBinding::fromController(CharInputAction::Ability7,
                                            InputEdge::Press,
                                            InputControllerBtn::RightTrigger),
};

static constexpr ActionAnalogAxisBinding kControllerAnalogAxis[] = {
    ActionAnalogAxisBinding::fromController(CharInputAction::MoveAxis,
                                            InputControllerAxis::LeftX,
                                            InputControllerAxis::LeftY),
};

inline std::span<const ActionDigitalBtnBinding> keyboardDigitalBtnDefaults() {
  return kKeyboardDigitalBtn;
}

inline std::span<const ActionDigitalBtnBinding> mouseDigitalBtnDefaults() {
  return kMouseDigitalBtn;
}

inline std::span<const ActionDigitalBtnBinding> controllerDigitalBtnDefaults() {
  return kControllerDigitalBtn;
}

template <class... Spans>
static std::vector<ActionDigitalBtnBinding> concat(Spans... ss) {
  std::vector<ActionDigitalBtnBinding> out;
  auto total = (ss.size() + ... + 0);
  out.reserve(total);
  (out.insert(out.end(), ss.begin(), ss.end()), ...);
  return out;
}

inline std::span<const ActionDigitalBtnBinding> defaultDigitalBtnBindings(
    PlayerSlot s) {
  static std::once_flag once;
  static std::vector<ActionDigitalBtnBinding> p0;
  static std::vector<ActionDigitalBtnBinding> pOther;

  std::call_once(once, [] {
    p0 = internal::concat(internal::keyboardDigitalBtnDefaults(),
                          internal::mouseDigitalBtnDefaults(),
                          internal::controllerDigitalBtnDefaults());
    pOther = internal::concat(internal::controllerDigitalBtnDefaults());
  });

  return (s == kPlayer0Slot) ? std::span<const ActionDigitalBtnBinding>(p0)
                             : std::span<const ActionDigitalBtnBinding>(pOther);
}

inline std::span<const ActionAnalogBtnBinding> defaultAnalogBtnBindings(
    PlayerSlot) {
  return {};
}

inline std::span<const ActionAnalogAxisBinding> defaultAnalogAxisBindings(
    PlayerSlot) {
  return kControllerAnalogAxis;
}
}  // namespace internal

inline ActionBindings defaultBindings(PlayerSlot s) {
  ActionBindings bindings{
      .digitalBtns = internal::defaultDigitalBtnBindings(s),
      .analogBtns = internal::defaultAnalogBtnBindings(s),
      .analogAxes = internal::defaultAnalogAxisBindings(s),
  };

  return bindings;
}
}  // namespace rl

#endif  // GAME_CHARACTER_CLASS_CHARACTER_CLASS_DEFAULT_BINDINGS_H_
