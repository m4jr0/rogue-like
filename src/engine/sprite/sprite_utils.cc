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
#include "sprite_utils.h"
////////////////////////////////////////////////////////////////////////////////

namespace rl {
SpritePosition resolvePivot(const SpriteDesc& spriteDesc,
                            const PivotDesc& pivotDesc) {
  const auto& ssz = spriteDesc.sourceSize;
  SpriteSizeUnit px = 0;
  SpriteSizeUnit py = 0;

  switch (pivotDesc.preset) {
    using enum PivotPreset;

    case TopLeft: {
      px = 0;
      py = 0;
      break;
    }

    case TopCenter: {
      px = ssz.x / 2;
      py = 0;
      break;
    }

    case TopRight: {
      px = ssz.x;
      py = 0;
      break;
    }

    case CenterLeft: {
      px = 0;
      py = ssz.y / 2;
      break;
    }

    case Center: {
      px = ssz.x / 2;
      py = ssz.y / 2;
      break;
    }

    case CenterRight: {
      px = ssz.x;
      py = ssz.y / 2;
      break;
    }

    case BottomLeft: {
      px = 0;
      py = ssz.y;
      break;
    }

    case BottomCenter: {
      px = ssz.x / 2;
      py = ssz.y;
      break;
    }

    case BottomRight: {
      px = ssz.x;
      py = ssz.y;
      break;
    }

    case CustomPixels: {
      px = pivotDesc.custom.x;
      py = pivotDesc.custom.y;
      break;
    }

    case CustomNormalized: {
      auto nx = std::clamp(pivotDesc.norm.x, .0f, 1.0f);
      auto ny = std::clamp(pivotDesc.norm.y, .0f, 1.0f);
      px = static_cast<SpriteSizeUnit>(std::lround(nx * ssz.x));
      py = static_cast<SpriteSizeUnit>(std::lround(ny * ssz.y));
      break;
    }

    default: {
      px = 0;
      py = 0;
      break;
    }
  }

  return {px, py};
}

void resolveOffsets(const SpriteDesc& spriteDesc, const PivotDesc& pivotDesc,
                    SpritePosition& offset, SpritePosition& offsetFlipped) {
  const auto& sr = spriteDesc.sourceRect;
  auto pivot = resolvePivot(spriteDesc, pivotDesc);
  offset.x = sr.pos.x - pivot.x;
  offset.y = sr.pos.y - pivot.y;

  const auto& ssz = spriteDesc.sourceSize;
  auto rightInset = ssz.x - (sr.pos.x + sr.size.x);
  auto bottomInset = ssz.y - (sr.pos.y + sr.size.y);

  offsetFlipped.x = rightInset - pivot.x;
  offsetFlipped.y = bottomInset - pivot.y;
}

SpritePosition resolveOffset(const SpriteDesc& spriteDesc,
                             const PivotDesc& pivotDesc) {
  const auto& sr = spriteDesc.sourceRect;
  auto pivot = resolvePivot(spriteDesc, pivotDesc);
  return {sr.pos.x - pivot.x, sr.pos.y - pivot.y};
}

void resolveOffsetsNorm(const SpriteDesc& sRect, const PivotDesc& pivotDesc,
                        SpritePositionNorm& offsetNorm,
                        SpritePositionNorm& offsetFlippedNorm) {
  auto offsetPx = resolveOffset(sRect, pivotDesc);
  auto rw = static_cast<SpriteSizeNormUnit>(std::max(1, sRect.rect.size.x));
  auto rh = static_cast<SpriteSizeNormUnit>(std::max(1, sRect.rect.size.y));

  offsetNorm = {std::clamp(-offsetPx.x / rw, .0f, 1.0f),
                std::clamp(1.0f + offsetPx.y / rh, .0f, 1.0f)};
  offsetFlippedNorm = {1.0f - offsetNorm.x, 1.0f - offsetNorm.y};
}
}  // namespace rl