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
#include "render_utils.h"
////////////////////////////////////////////////////////////////////////////////

namespace rl {
Viewport fitInside(WindowSize winW, WindowSize winH, WindowSize refW,
                   WindowSize refH) {
  auto winRatio =
      (winH > 0) ? static_cast<f64>(winW) / static_cast<f64>(winH) : 1.0;
  auto refRatio = static_cast<f64>(refW) / static_cast<f64>(refH);

  auto vW = winW;
  auto vH = winH;

  if (winRatio > refRatio) {
    vH = winH;
    vW = static_cast<WindowSize>(vH * refRatio + .5);
  } else {
    vW = winW;
    vH = static_cast<WindowSize>(vW / refRatio + .5);
  }

  auto vX = static_cast<WindowSize>((winW - vW) / 2);
  auto vY = static_cast<WindowSize>((winH - vH) / 2);

  return {
      .pos = {vX, vY},
      .size = {vW, vH},
      .ref = {refW, refH},
  };
}
}  // namespace rl