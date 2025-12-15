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

#ifndef ENGINE_CORE_COLOR_H_
#define ENGINE_CORE_COLOR_H_

#include "engine/common.h"

namespace rl {
namespace internal {
inline constexpr u32 kPackedShiftR = 0;
inline constexpr u32 kPackedShiftG = 8;
inline constexpr u32 kPackedShiftB = 16;
inline constexpr u32 kPackedShiftA = 24;

inline constexpr f32 kInv255 = 1.0f / 255.0f;

[[nodiscard]] inline constexpr u8 normalizedToU8(f32 v) noexcept {
  return static_cast<u8>((v < .0f ? .0f : (v > 1.0f ? 1.0f : v)) * 255.0f +
                         .5f);
}
}  // namespace internal

struct Rgba {
  u8 r{255};
  u8 g{255};
  u8 b{255};
  u8 a{255};

  constexpr bool operator==(const Rgba& o) const noexcept {
    return r == o.r && g == o.g && b == o.b && a == o.a;
  }

  constexpr bool operator!=(const Rgba& o) const noexcept {
    return !(*this == o);
  }
};

struct RgbaNorm {
  f32 r{1.0f};
  f32 g{1.0f};
  f32 b{1.0f};
  f32 a{1.0f};

  constexpr bool operator==(const Rgba& o) const noexcept {
    return r == o.r && g == o.g && b == o.b && a == o.a;
  }

  constexpr bool operator!=(const Rgba& o) const noexcept {
    return !(*this == o);
  }
};

[[nodiscard]] inline constexpr u32 rgbaToPacked(const Rgba& c) noexcept {
  return (static_cast<u32>(c.r) << internal::kPackedShiftR) |
         (static_cast<u32>(c.g) << internal::kPackedShiftG) |
         (static_cast<u32>(c.b) << internal::kPackedShiftB) |
         (static_cast<u32>(c.a) << internal::kPackedShiftA);
}

[[nodiscard]] inline constexpr Rgba packedToRgba(u32 p) noexcept {
  return Rgba{static_cast<u8>((p >> internal::kPackedShiftR) & 0xFF),
              static_cast<u8>((p >> internal::kPackedShiftG) & 0xFF),
              static_cast<u8>((p >> internal::kPackedShiftB) & 0xFF),
              static_cast<u8>((p >> internal::kPackedShiftA) & 0xFF)};
}

[[nodiscard]] inline constexpr RgbaNorm rgbaToRgbaNorm(const Rgba& c) noexcept {
  return RgbaNorm{static_cast<f32>(c.r) * internal::kInv255,
                  static_cast<f32>(c.g) * internal::kInv255,
                  static_cast<f32>(c.b) * internal::kInv255,
                  static_cast<f32>(c.a) * internal::kInv255};
}

[[nodiscard]] inline constexpr Rgba rgbaNormToRgba(const RgbaNorm& c) noexcept {
  return Rgba{internal::normalizedToU8(c.r), internal::normalizedToU8(c.g),
              internal::normalizedToU8(c.b), internal::normalizedToU8(c.a)};
}

[[nodiscard]] inline constexpr u32 rgbaNormToPacked(
    const RgbaNorm& c) noexcept {
  return rgbaToPacked(rgbaNormToRgba(c));
}

[[nodiscard]] inline constexpr RgbaNorm packedToRgbaNorm(u32 p) noexcept {
  return rgbaToRgbaNorm(packedToRgba(p));
}

// Rgba constants.
constexpr Rgba kRgbaAliceBlue{240, 248, 255, 255};
constexpr Rgba kRgbaAntiqueWhite{250, 235, 215, 255};
constexpr Rgba kRgbaAqua{0, 255, 255, 255};
constexpr Rgba kRgbaAquaMarine{127, 255, 212, 255};
constexpr Rgba kRgbaAzure{240, 255, 255, 255};
constexpr Rgba kRgbaBeige{245, 245, 220, 255};
constexpr Rgba kRgbaBisque{255, 228, 196, 255};
constexpr Rgba kRgbaBlack{0, 0, 0, 255};
constexpr Rgba kRgbaBlanchedAlmond{255, 235, 205, 255};
constexpr Rgba kRgbaBlue{0, 0, 255, 255};
constexpr Rgba kRgbaBlueViolet{138, 43, 226, 255};
constexpr Rgba kRgbaBrown{165, 42, 42, 255};
constexpr Rgba kRgbaBurlyWood{222, 184, 135, 255};
constexpr Rgba kRgbaCadetBlue{95, 158, 160, 255};
constexpr Rgba kRgbaChartreuse{127, 255, 0, 255};
constexpr Rgba kRgbaChocolate{210, 105, 30, 255};
constexpr Rgba kRgbaCoral{255, 127, 80, 255};
constexpr Rgba kRgbaCornflowerBlue{100, 149, 237, 255};
constexpr Rgba kRgbaCornSilk{255, 248, 220, 255};
constexpr Rgba kRgbaCrimson{220, 20, 60, 255};
constexpr Rgba kRgbaCyan{0, 255, 255, 255};
constexpr Rgba kRgbaDarkBlue{0, 0, 139, 255};
constexpr Rgba kRgbaDarkCyan{0, 139, 139, 255};
constexpr Rgba kRgbaDarkGoldenrod{184, 134, 11, 255};
constexpr Rgba kRgbaDarkGray{169, 169, 169, 255};
constexpr Rgba kRgbaDarkGreen{0, 100, 0, 255};
constexpr Rgba kRgbaDarkGrey{169, 169, 169, 255};
constexpr Rgba kRgbaDarkKhaki{189, 183, 107, 255};
constexpr Rgba kRgbaDarkMagenta{139, 0, 139, 255};
constexpr Rgba kRgbaDarkOliveGreen{85, 107, 47, 255};
constexpr Rgba kRgbaDarkOrange{255, 140, 0, 255};
constexpr Rgba kRgbaDarkOrchid{153, 50, 204, 255};
constexpr Rgba kRgbaDarkRed{139, 0, 0, 255};
constexpr Rgba kRgbaDarkSalmon{233, 150, 122, 255};
constexpr Rgba kRgbaDarkSeaGreen{143, 188, 143, 255};
constexpr Rgba kRgbaDarkSlateBlue{72, 61, 139, 255};
constexpr Rgba kRgbaDarkSlateGray{47, 79, 79, 255};
constexpr Rgba kRgbaDarkSlateGrey{47, 79, 79, 255};
constexpr Rgba kRgbaDarkTurquoise{0, 206, 209, 255};
constexpr Rgba kRgbaDarkViolet{148, 0, 211, 255};
constexpr Rgba kRgbaDeepPink{255, 20, 147, 255};
constexpr Rgba kRgbaDeepSkyBlue{0, 191, 255, 255};
constexpr Rgba kRgbaDimGray{105, 105, 105, 255};
constexpr Rgba kRgbaDimGrey{105, 105, 105, 255};
constexpr Rgba kRgbaDodgerBlue{30, 144, 255, 255};
constexpr Rgba kRgbaFireBrick{178, 34, 34, 255};
constexpr Rgba kRgbaFloralWhite{255, 250, 240, 255};
constexpr Rgba kRgbaForestGreen{34, 139, 34, 255};
constexpr Rgba kRgbaFuchsia{255, 0, 255, 255};
constexpr Rgba kRgbaGainsboro{220, 220, 220, 255};
constexpr Rgba kRgbaGhostWhite{248, 248, 255, 255};
constexpr Rgba kRgbaGold{255, 215, 0, 255};
constexpr Rgba kRgbaGoldenrod{218, 165, 32, 255};
constexpr Rgba kRgbaGray{128, 128, 128, 255};
constexpr Rgba kRgbaGreen{0, 128, 0, 255};
constexpr Rgba kRgbaGreenYellow{173, 255, 47, 255};
constexpr Rgba kRgbaGrey{128, 128, 128, 255};
constexpr Rgba kRgbaHoneydew{240, 255, 240, 255};
constexpr Rgba kRgbaHotPink{255, 105, 180, 255};
constexpr Rgba kRgbaIndianRed{205, 92, 92, 255};
constexpr Rgba kRgbaIndigo{75, 0, 130, 255};
constexpr Rgba kRgbaIvory{255, 255, 240, 255};
constexpr Rgba kRgbaKhaki{240, 230, 140, 255};
constexpr Rgba kRgbaLavender{230, 230, 250, 255};
constexpr Rgba kRgbaLavenderBlush{255, 240, 245, 255};
constexpr Rgba kRgbaLawnGreen{124, 252, 0, 255};
constexpr Rgba kRgbaLemonChiffon{255, 250, 205, 255};
constexpr Rgba kRgbaLightBlue{173, 216, 230, 255};
constexpr Rgba kRgbaLightCoral{240, 128, 128, 255};
constexpr Rgba kRgbaLightCyan{224, 255, 255, 255};
constexpr Rgba kRgbaLightGoldenrodYellow{250, 250, 210, 255};
constexpr Rgba kRgbaLightGray{211, 211, 211, 255};
constexpr Rgba kRgbaLightGreen{144, 238, 144, 255};
constexpr Rgba kRgbaLightGrey{211, 211, 211, 255};
constexpr Rgba kRgbaLightPink{255, 182, 193, 255};
constexpr Rgba kRgbaLightSalmon{255, 160, 122, 255};
constexpr Rgba kRgbaLightSeaGreen{32, 178, 170, 255};
constexpr Rgba kRgbaLightSkyBlue{135, 206, 250, 255};
constexpr Rgba kRgbaLightSlateGray{119, 136, 153, 255};
constexpr Rgba kRgbaLightSlateGrey{119, 136, 153, 255};
constexpr Rgba kRgbaLightSteelBlue{176, 196, 222, 255};
constexpr Rgba kRgbaLightYellow{255, 255, 224, 255};
constexpr Rgba kRgbaLime{0, 255, 0, 255};
constexpr Rgba kRgbaLimeGreen{50, 205, 50, 255};
constexpr Rgba kRgbaLinen{250, 240, 230, 255};
constexpr Rgba kRgbaMagenta{255, 0, 255, 255};
constexpr Rgba kRgbaMaroon{128, 0, 0, 255};
constexpr Rgba kRgbaMediumAquamarine{102, 205, 170, 255};
constexpr Rgba kRgbaMediumBlue{0, 0, 205, 255};
constexpr Rgba kRgbaMediumOrchid{186, 85, 211, 255};
constexpr Rgba kRgbaMediumPurple{147, 112, 219, 255};
constexpr Rgba kRgbaMediumSeaGreen{60, 179, 113, 255};
constexpr Rgba kRgbaMediumSlateBlue{123, 104, 238, 255};
constexpr Rgba kRgbaMediumSpringGreen{0, 250, 154, 255};
constexpr Rgba kRgbaMediumTurquoise{72, 209, 204, 255};
constexpr Rgba kRgbaMediumVioletRed{199, 21, 133, 255};
constexpr Rgba kRgbaMidnightBlue{25, 25, 112, 255};
constexpr Rgba kRgbaMintCream{245, 255, 250, 255};
constexpr Rgba kRgbaMistyRose{255, 228, 225, 255};
constexpr Rgba kRgbaMoccasin{255, 228, 181, 255};
constexpr Rgba kRgbaNavajoWhite{255, 222, 173, 255};
constexpr Rgba kRgbaNavy{0, 0, 128, 255};
constexpr Rgba kRgbaOldLace{253, 245, 230, 255};
constexpr Rgba kRgbaOlive{128, 128, 0, 255};
constexpr Rgba kRgbaOliveDrab{107, 142, 35, 255};
constexpr Rgba kRgbaOrange{255, 165, 0, 255};
constexpr Rgba kRgbaOrangeRed{255, 69, 0, 255};
constexpr Rgba kRgbaOrchid{218, 112, 214, 255};
constexpr Rgba kRgbaPaleGoldenrod{238, 232, 170, 255};
constexpr Rgba kRgbaPaleGreen{152, 251, 152, 255};
constexpr Rgba kRgbaPaleTurquoise{175, 238, 238, 255};
constexpr Rgba kRgbaPaleVioletRed{219, 112, 147, 255};
constexpr Rgba kRgbaPapayaWhip{255, 239, 213, 255};
constexpr Rgba kRgbaPeachPuff{255, 218, 185, 255};
constexpr Rgba kRgbaPeru{205, 133, 63, 255};
constexpr Rgba kRgbaPink{255, 192, 203, 255};
constexpr Rgba kRgbaPlum{221, 160, 221, 255};
constexpr Rgba kRgbaPowderBlue{176, 224, 230, 255};
constexpr Rgba kRgbaPurple{128, 0, 128, 255};
constexpr Rgba kRgbaRebeccaPurple{102, 51, 153, 255};
constexpr Rgba kRgbaRed{255, 0, 0, 255};
constexpr Rgba kRgbaRosyBrown{188, 143, 143, 255};
constexpr Rgba kRgbaRoyalBlue{65, 105, 225, 255};
constexpr Rgba kRgbaSaddleBrown{139, 69, 19, 255};
constexpr Rgba kRgbaSalmon{250, 128, 114, 255};
constexpr Rgba kRgbaSandyBrown{244, 164, 96, 255};
constexpr Rgba kRgbaSeaGreen{46, 139, 87, 255};
constexpr Rgba kRgbaSeashell{255, 245, 238, 255};
constexpr Rgba kRgbaSienna{160, 82, 45, 255};
constexpr Rgba kRgbaSilver{192, 192, 192, 255};
constexpr Rgba kRgbaSkyBlue{135, 206, 235, 255};
constexpr Rgba kRgbaSlateBlue{106, 90, 205, 255};
constexpr Rgba kRgbaSlateGray{112, 128, 144, 255};
constexpr Rgba kRgbaSlateGrey{112, 128, 144, 255};
constexpr Rgba kRgbaSnow{255, 250, 250, 255};
constexpr Rgba kRgbaSpringGreen{0, 255, 127, 255};
constexpr Rgba kRgbaSteelBlue{70, 130, 180, 255};
constexpr Rgba kRgbaTan{210, 180, 140, 255};
constexpr Rgba kRgbaTeal{0, 128, 128, 255};
constexpr Rgba kRgbaThistle{216, 191, 216, 255};
constexpr Rgba kRgbaTomato{255, 99, 71, 255};
constexpr Rgba kRgbaTransparent{0, 0, 0, 0};
constexpr Rgba kRgbaTurquoise{64, 224, 208, 255};
constexpr Rgba kRgbaViolet{238, 130, 238, 255};
constexpr Rgba kRgbaWheat{245, 222, 179, 255};
constexpr Rgba kRgbaWhite{255, 255, 255, 255};
constexpr Rgba kRgbaWhiteSmoke{245, 245, 245, 255};
constexpr Rgba kRgbaYellow{255, 255, 0, 255};
constexpr Rgba kRgbaYellowGreen{154, 205, 50, 255};

// RgbaNorm constants.
constexpr RgbaNorm kRgbaNormAliceBlue{rgbaToRgbaNorm(kRgbaAliceBlue)};
constexpr RgbaNorm kRgbaNormAntiqueWhite{rgbaToRgbaNorm(kRgbaAntiqueWhite)};
constexpr RgbaNorm kRgbaNormAqua{rgbaToRgbaNorm(kRgbaAqua)};
constexpr RgbaNorm kRgbaNormAquaMarine{rgbaToRgbaNorm(kRgbaAquaMarine)};
constexpr RgbaNorm kRgbaNormAzure{rgbaToRgbaNorm(kRgbaAzure)};
constexpr RgbaNorm kRgbaNormBeige{rgbaToRgbaNorm(kRgbaBeige)};
constexpr RgbaNorm kRgbaNormBisque{rgbaToRgbaNorm(kRgbaBisque)};
constexpr RgbaNorm kRgbaNormBlack{rgbaToRgbaNorm(kRgbaBlack)};
constexpr RgbaNorm kRgbaNormBlanchedAlmond{rgbaToRgbaNorm(kRgbaBlanchedAlmond)};
constexpr RgbaNorm kRgbaNormBlue{rgbaToRgbaNorm(kRgbaBlue)};
constexpr RgbaNorm kRgbaNormBlueViolet{rgbaToRgbaNorm(kRgbaBlueViolet)};
constexpr RgbaNorm kRgbaNormBrown{rgbaToRgbaNorm(kRgbaBrown)};
constexpr RgbaNorm kRgbaNormBurlyWood{rgbaToRgbaNorm(kRgbaBurlyWood)};
constexpr RgbaNorm kRgbaNormCadetBlue{rgbaToRgbaNorm(kRgbaCadetBlue)};
constexpr RgbaNorm kRgbaNormChartreuse{rgbaToRgbaNorm(kRgbaChartreuse)};
constexpr RgbaNorm kRgbaNormChocolate{rgbaToRgbaNorm(kRgbaChocolate)};
constexpr RgbaNorm kRgbaNormCoral{rgbaToRgbaNorm(kRgbaCoral)};
constexpr RgbaNorm kRgbaNormCornflowerBlue{rgbaToRgbaNorm(kRgbaCornflowerBlue)};
constexpr RgbaNorm kRgbaNormCornSilk{rgbaToRgbaNorm(kRgbaCornSilk)};
constexpr RgbaNorm kRgbaNormCrimson{rgbaToRgbaNorm(kRgbaCrimson)};
constexpr RgbaNorm kRgbaNormCyan{rgbaToRgbaNorm(kRgbaCyan)};
constexpr RgbaNorm kRgbaNormDarkBlue{rgbaToRgbaNorm(kRgbaDarkBlue)};
constexpr RgbaNorm kRgbaNormDarkCyan{rgbaToRgbaNorm(kRgbaDarkCyan)};
constexpr RgbaNorm kRgbaNormDarkGoldenrod{rgbaToRgbaNorm(kRgbaDarkGoldenrod)};
constexpr RgbaNorm kRgbaNormDarkGray{rgbaToRgbaNorm(kRgbaDarkGray)};
constexpr RgbaNorm kRgbaNormDarkGreen{rgbaToRgbaNorm(kRgbaDarkGreen)};
constexpr RgbaNorm kRgbaNormDarkGrey{rgbaToRgbaNorm(kRgbaDarkGrey)};
constexpr RgbaNorm kRgbaNormDarkKhaki{rgbaToRgbaNorm(kRgbaDarkKhaki)};
constexpr RgbaNorm kRgbaNormDarkMagenta{rgbaToRgbaNorm(kRgbaDarkMagenta)};
constexpr RgbaNorm kRgbaNormDarkOliveGreen{rgbaToRgbaNorm(kRgbaDarkOliveGreen)};
constexpr RgbaNorm kRgbaNormDarkOrange{rgbaToRgbaNorm(kRgbaDarkOrange)};
constexpr RgbaNorm kRgbaNormDarkOrchid{rgbaToRgbaNorm(kRgbaDarkOrchid)};
constexpr RgbaNorm kRgbaNormDarkRed{rgbaToRgbaNorm(kRgbaDarkRed)};
constexpr RgbaNorm kRgbaNormDarkSalmon{rgbaToRgbaNorm(kRgbaDarkSalmon)};
constexpr RgbaNorm kRgbaNormDarkSeaGreen{rgbaToRgbaNorm(kRgbaDarkSeaGreen)};
constexpr RgbaNorm kRgbaNormDarkSlateBlue{rgbaToRgbaNorm(kRgbaDarkSlateBlue)};
constexpr RgbaNorm kRgbaNormDarkSlateGray{rgbaToRgbaNorm(kRgbaDarkSlateGray)};
constexpr RgbaNorm kRgbaNormDarkSlateGrey{rgbaToRgbaNorm(kRgbaDarkSlateGrey)};
constexpr RgbaNorm kRgbaNormDarkTurquoise{rgbaToRgbaNorm(kRgbaDarkTurquoise)};
constexpr RgbaNorm kRgbaNormDarkViolet{rgbaToRgbaNorm(kRgbaDarkViolet)};
constexpr RgbaNorm kRgbaNormDeepPink{rgbaToRgbaNorm(kRgbaDeepPink)};
constexpr RgbaNorm kRgbaNormDeepSkyBlue{rgbaToRgbaNorm(kRgbaDeepSkyBlue)};
constexpr RgbaNorm kRgbaNormDimGray{rgbaToRgbaNorm(kRgbaDimGray)};
constexpr RgbaNorm kRgbaNormDimGrey{rgbaToRgbaNorm(kRgbaDimGrey)};
constexpr RgbaNorm kRgbaNormDodgerBlue{rgbaToRgbaNorm(kRgbaDodgerBlue)};
constexpr RgbaNorm kRgbaNormFireBrick{rgbaToRgbaNorm(kRgbaFireBrick)};
constexpr RgbaNorm kRgbaNormFloralWhite{rgbaToRgbaNorm(kRgbaFloralWhite)};
constexpr RgbaNorm kRgbaNormForestGreen{rgbaToRgbaNorm(kRgbaForestGreen)};
constexpr RgbaNorm kRgbaNormFuchsia{rgbaToRgbaNorm(kRgbaFuchsia)};
constexpr RgbaNorm kRgbaNormGainsboro{rgbaToRgbaNorm(kRgbaGainsboro)};
constexpr RgbaNorm kRgbaNormGhostWhite{rgbaToRgbaNorm(kRgbaGhostWhite)};
constexpr RgbaNorm kRgbaNormGold{rgbaToRgbaNorm(kRgbaGold)};
constexpr RgbaNorm kRgbaNormGoldenrod{rgbaToRgbaNorm(kRgbaGoldenrod)};
constexpr RgbaNorm kRgbaNormGray{rgbaToRgbaNorm(kRgbaGray)};
constexpr RgbaNorm kRgbaNormGreen{rgbaToRgbaNorm(kRgbaGreen)};
constexpr RgbaNorm kRgbaNormGreenYellow{rgbaToRgbaNorm(kRgbaGreenYellow)};
constexpr RgbaNorm kRgbaNormGrey{rgbaToRgbaNorm(kRgbaGrey)};
constexpr RgbaNorm kRgbaNormHoneydew{rgbaToRgbaNorm(kRgbaHoneydew)};
constexpr RgbaNorm kRgbaNormHotPink{rgbaToRgbaNorm(kRgbaHotPink)};
constexpr RgbaNorm kRgbaNormIndianRed{rgbaToRgbaNorm(kRgbaIndianRed)};
constexpr RgbaNorm kRgbaNormIndigo{rgbaToRgbaNorm(kRgbaIndigo)};
constexpr RgbaNorm kRgbaNormIvory{rgbaToRgbaNorm(kRgbaIvory)};
constexpr RgbaNorm kRgbaNormKhaki{rgbaToRgbaNorm(kRgbaKhaki)};
constexpr RgbaNorm kRgbaNormLavender{rgbaToRgbaNorm(kRgbaLavender)};
constexpr RgbaNorm kRgbaNormLavenderBlush{rgbaToRgbaNorm(kRgbaLavenderBlush)};
constexpr RgbaNorm kRgbaNormLawnGreen{rgbaToRgbaNorm(kRgbaLawnGreen)};
constexpr RgbaNorm kRgbaNormLemonChiffon{rgbaToRgbaNorm(kRgbaLemonChiffon)};
constexpr RgbaNorm kRgbaNormLightBlue{rgbaToRgbaNorm(kRgbaLightBlue)};
constexpr RgbaNorm kRgbaNormLightCoral{rgbaToRgbaNorm(kRgbaLightCoral)};
constexpr RgbaNorm kRgbaNormLightCyan{rgbaToRgbaNorm(kRgbaLightCyan)};
constexpr RgbaNorm kRgbaNormLightGoldenrodYellow{
    rgbaToRgbaNorm(kRgbaLightGoldenrodYellow)};
constexpr RgbaNorm kRgbaNormLightGray{rgbaToRgbaNorm(kRgbaLightGray)};
constexpr RgbaNorm kRgbaNormLightGreen{rgbaToRgbaNorm(kRgbaLightGreen)};
constexpr RgbaNorm kRgbaNormLightGrey{rgbaToRgbaNorm(kRgbaLightGrey)};
constexpr RgbaNorm kRgbaNormLightPink{rgbaToRgbaNorm(kRgbaLightPink)};
constexpr RgbaNorm kRgbaNormLightSalmon{rgbaToRgbaNorm(kRgbaLightSalmon)};
constexpr RgbaNorm kRgbaNormLightSeaGreen{rgbaToRgbaNorm(kRgbaLightSeaGreen)};
constexpr RgbaNorm kRgbaNormLightSkyBlue{rgbaToRgbaNorm(kRgbaLightSkyBlue)};
constexpr RgbaNorm kRgbaNormLightSlateGray{rgbaToRgbaNorm(kRgbaLightSlateGray)};
constexpr RgbaNorm kRgbaNormLightSlateGrey{rgbaToRgbaNorm(kRgbaLightSlateGrey)};
constexpr RgbaNorm kRgbaNormLightSteelBlue{rgbaToRgbaNorm(kRgbaLightSteelBlue)};
constexpr RgbaNorm kRgbaNormLightYellow{rgbaToRgbaNorm(kRgbaLightYellow)};
constexpr RgbaNorm kRgbaNormLime{rgbaToRgbaNorm(kRgbaLime)};
constexpr RgbaNorm kRgbaNormLimeGreen{rgbaToRgbaNorm(kRgbaLimeGreen)};
constexpr RgbaNorm kRgbaNormLinen{rgbaToRgbaNorm(kRgbaLinen)};
constexpr RgbaNorm kRgbaNormMagenta{rgbaToRgbaNorm(kRgbaMagenta)};
constexpr RgbaNorm kRgbaNormMaroon{rgbaToRgbaNorm(kRgbaMaroon)};
constexpr RgbaNorm kRgbaNormMediumAquamarine{
    rgbaToRgbaNorm(kRgbaMediumAquamarine)};
constexpr RgbaNorm kRgbaNormMediumBlue{rgbaToRgbaNorm(kRgbaMediumBlue)};
constexpr RgbaNorm kRgbaNormMediumOrchid{rgbaToRgbaNorm(kRgbaMediumOrchid)};
constexpr RgbaNorm kRgbaNormMediumPurple{rgbaToRgbaNorm(kRgbaMediumPurple)};
constexpr RgbaNorm kRgbaNormMediumSeaGreen{rgbaToRgbaNorm(kRgbaMediumSeaGreen)};
constexpr RgbaNorm kRgbaNormMediumSlateBlue{
    rgbaToRgbaNorm(kRgbaMediumSlateBlue)};
constexpr RgbaNorm kRgbaNormMediumSpringGreen{
    rgbaToRgbaNorm(kRgbaMediumSpringGreen)};
constexpr RgbaNorm kRgbaNormMediumTurquoise{
    rgbaToRgbaNorm(kRgbaMediumTurquoise)};
constexpr RgbaNorm kRgbaNormMediumVioletRed{
    rgbaToRgbaNorm(kRgbaMediumVioletRed)};
constexpr RgbaNorm kRgbaNormMidnightBlue{rgbaToRgbaNorm(kRgbaMidnightBlue)};
constexpr RgbaNorm kRgbaNormMintCream{rgbaToRgbaNorm(kRgbaMintCream)};
constexpr RgbaNorm kRgbaNormMistyRose{rgbaToRgbaNorm(kRgbaMistyRose)};
constexpr RgbaNorm kRgbaNormMoccasin{rgbaToRgbaNorm(kRgbaMoccasin)};
constexpr RgbaNorm kRgbaNormNavajoWhite{rgbaToRgbaNorm(kRgbaNavajoWhite)};
constexpr RgbaNorm kRgbaNormNavy{rgbaToRgbaNorm(kRgbaNavy)};
constexpr RgbaNorm kRgbaNormOldLace{rgbaToRgbaNorm(kRgbaOldLace)};
constexpr RgbaNorm kRgbaNormOlive{rgbaToRgbaNorm(kRgbaOlive)};
constexpr RgbaNorm kRgbaNormOliveDrab{rgbaToRgbaNorm(kRgbaOliveDrab)};
constexpr RgbaNorm kRgbaNormOrange{rgbaToRgbaNorm(kRgbaOrange)};
constexpr RgbaNorm kRgbaNormOrangeRed{rgbaToRgbaNorm(kRgbaOrangeRed)};
constexpr RgbaNorm kRgbaNormOrchid{rgbaToRgbaNorm(kRgbaOrchid)};
constexpr RgbaNorm kRgbaNormPaleGoldenrod{rgbaToRgbaNorm(kRgbaPaleGoldenrod)};
constexpr RgbaNorm kRgbaNormPaleGreen{rgbaToRgbaNorm(kRgbaPaleGreen)};
constexpr RgbaNorm kRgbaNormPaleTurquoise{rgbaToRgbaNorm(kRgbaPaleTurquoise)};
constexpr RgbaNorm kRgbaNormPaleVioletRed{rgbaToRgbaNorm(kRgbaPaleVioletRed)};
constexpr RgbaNorm kRgbaNormPapayaWhip{rgbaToRgbaNorm(kRgbaPapayaWhip)};
constexpr RgbaNorm kRgbaNormPeachPuff{rgbaToRgbaNorm(kRgbaPeachPuff)};
constexpr RgbaNorm kRgbaNormPeru{rgbaToRgbaNorm(kRgbaPeru)};
constexpr RgbaNorm kRgbaNormPink{rgbaToRgbaNorm(kRgbaPink)};
constexpr RgbaNorm kRgbaNormPlum{rgbaToRgbaNorm(kRgbaPlum)};
constexpr RgbaNorm kRgbaNormPowderBlue{rgbaToRgbaNorm(kRgbaPowderBlue)};
constexpr RgbaNorm kRgbaNormPurple{rgbaToRgbaNorm(kRgbaPurple)};
constexpr RgbaNorm kRgbaNormRebeccaPurple{rgbaToRgbaNorm(kRgbaRebeccaPurple)};
constexpr RgbaNorm kRgbaNormRed{rgbaToRgbaNorm(kRgbaRed)};
constexpr RgbaNorm kRgbaNormRosyBrown{rgbaToRgbaNorm(kRgbaRosyBrown)};
constexpr RgbaNorm kRgbaNormRoyalBlue{rgbaToRgbaNorm(kRgbaRoyalBlue)};
constexpr RgbaNorm kRgbaNormSaddleBrown{rgbaToRgbaNorm(kRgbaSaddleBrown)};
constexpr RgbaNorm kRgbaNormSalmon{rgbaToRgbaNorm(kRgbaSalmon)};
constexpr RgbaNorm kRgbaNormSandyBrown{rgbaToRgbaNorm(kRgbaSandyBrown)};
constexpr RgbaNorm kRgbaNormSeaGreen{rgbaToRgbaNorm(kRgbaSeaGreen)};
constexpr RgbaNorm kRgbaNormSeashell{rgbaToRgbaNorm(kRgbaSeashell)};
constexpr RgbaNorm kRgbaNormSienna{rgbaToRgbaNorm(kRgbaSienna)};
constexpr RgbaNorm kRgbaNormSilver{rgbaToRgbaNorm(kRgbaSilver)};
constexpr RgbaNorm kRgbaNormSkyBlue{rgbaToRgbaNorm(kRgbaSkyBlue)};
constexpr RgbaNorm kRgbaNormSlateBlue{rgbaToRgbaNorm(kRgbaSlateBlue)};
constexpr RgbaNorm kRgbaNormSlateGray{rgbaToRgbaNorm(kRgbaSlateGray)};
constexpr RgbaNorm kRgbaNormSlateGrey{rgbaToRgbaNorm(kRgbaSlateGrey)};
constexpr RgbaNorm kRgbaNormSnow{rgbaToRgbaNorm(kRgbaSnow)};
constexpr RgbaNorm kRgbaNormSpringGreen{rgbaToRgbaNorm(kRgbaSpringGreen)};
constexpr RgbaNorm kRgbaNormSteelBlue{rgbaToRgbaNorm(kRgbaSteelBlue)};
constexpr RgbaNorm kRgbaNormTan{rgbaToRgbaNorm(kRgbaTan)};
constexpr RgbaNorm kRgbaNormTeal{rgbaToRgbaNorm(kRgbaTeal)};
constexpr RgbaNorm kRgbaNormThistle{rgbaToRgbaNorm(kRgbaThistle)};
constexpr RgbaNorm kRgbaNormTomato{rgbaToRgbaNorm(kRgbaTomato)};
constexpr RgbaNorm kRgbaNormTransparent{rgbaToRgbaNorm(kRgbaTransparent)};
constexpr RgbaNorm kRgbaNormTurquoise{rgbaToRgbaNorm(kRgbaTurquoise)};
constexpr RgbaNorm kRgbaNormViolet{rgbaToRgbaNorm(kRgbaViolet)};
constexpr RgbaNorm kRgbaNormWheat{rgbaToRgbaNorm(kRgbaWheat)};
constexpr RgbaNorm kRgbaNormWhite{rgbaToRgbaNorm(kRgbaWhite)};
constexpr RgbaNorm kRgbaNormWhiteSmoke{rgbaToRgbaNorm(kRgbaWhiteSmoke)};
constexpr RgbaNorm kRgbaNormYellow{rgbaToRgbaNorm(kRgbaYellow)};
constexpr RgbaNorm kRgbaNormYellowGreen{rgbaToRgbaNorm(kRgbaYellowGreen)};

// Packed u32 constants.
constexpr u32 kRgbaPackedAliceBlue{rgbaToPacked(kRgbaAliceBlue)};
constexpr u32 kRgbaPackedAntiqueWhite{rgbaToPacked(kRgbaAntiqueWhite)};
constexpr u32 kRgbaPackedAqua{rgbaToPacked(kRgbaAqua)};
constexpr u32 kRgbaPackedAquaMarine{rgbaToPacked(kRgbaAquaMarine)};
constexpr u32 kRgbaPackedAzure{rgbaToPacked(kRgbaAzure)};
constexpr u32 kRgbaPackedBeige{rgbaToPacked(kRgbaBeige)};
constexpr u32 kRgbaPackedBisque{rgbaToPacked(kRgbaBisque)};
constexpr u32 kRgbaPackedBlack{rgbaToPacked(kRgbaBlack)};
constexpr u32 kRgbaPackedBlanchedAlmond{rgbaToPacked(kRgbaBlanchedAlmond)};
constexpr u32 kRgbaPackedBlue{rgbaToPacked(kRgbaBlue)};
constexpr u32 kRgbaPackedBlueViolet{rgbaToPacked(kRgbaBlueViolet)};
constexpr u32 kRgbaPackedBrown{rgbaToPacked(kRgbaBrown)};
constexpr u32 kRgbaPackedBurlyWood{rgbaToPacked(kRgbaBurlyWood)};
constexpr u32 kRgbaPackedCadetBlue{rgbaToPacked(kRgbaCadetBlue)};
constexpr u32 kRgbaPackedChartreuse{rgbaToPacked(kRgbaChartreuse)};
constexpr u32 kRgbaPackedChocolate{rgbaToPacked(kRgbaChocolate)};
constexpr u32 kRgbaPackedCoral{rgbaToPacked(kRgbaCoral)};
constexpr u32 kRgbaPackedCornflowerBlue{rgbaToPacked(kRgbaCornflowerBlue)};
constexpr u32 kRgbaPackedCornSilk{rgbaToPacked(kRgbaCornSilk)};
constexpr u32 kRgbaPackedCrimson{rgbaToPacked(kRgbaCrimson)};
constexpr u32 kRgbaPackedCyan{rgbaToPacked(kRgbaCyan)};
constexpr u32 kRgbaPackedDarkBlue{rgbaToPacked(kRgbaDarkBlue)};
constexpr u32 kRgbaPackedDarkCyan{rgbaToPacked(kRgbaDarkCyan)};
constexpr u32 kRgbaPackedDarkGoldenrod{rgbaToPacked(kRgbaDarkGoldenrod)};
constexpr u32 kRgbaPackedDarkGray{rgbaToPacked(kRgbaDarkGray)};
constexpr u32 kRgbaPackedDarkGreen{rgbaToPacked(kRgbaDarkGreen)};
constexpr u32 kRgbaPackedDarkGrey{rgbaToPacked(kRgbaDarkGrey)};
constexpr u32 kRgbaPackedDarkKhaki{rgbaToPacked(kRgbaDarkKhaki)};
constexpr u32 kRgbaPackedDarkMagenta{rgbaToPacked(kRgbaDarkMagenta)};
constexpr u32 kRgbaPackedDarkOliveGreen{rgbaToPacked(kRgbaDarkOliveGreen)};
constexpr u32 kRgbaPackedDarkOrange{rgbaToPacked(kRgbaDarkOrange)};
constexpr u32 kRgbaPackedDarkOrchid{rgbaToPacked(kRgbaDarkOrchid)};
constexpr u32 kRgbaPackedDarkRed{rgbaToPacked(kRgbaDarkRed)};
constexpr u32 kRgbaPackedDarkSalmon{rgbaToPacked(kRgbaDarkSalmon)};
constexpr u32 kRgbaPackedDarkSeaGreen{rgbaToPacked(kRgbaDarkSeaGreen)};
constexpr u32 kRgbaPackedDarkSlateBlue{rgbaToPacked(kRgbaDarkSlateBlue)};
constexpr u32 kRgbaPackedDarkSlateGray{rgbaToPacked(kRgbaDarkSlateGray)};
constexpr u32 kRgbaPackedDarkSlateGrey{rgbaToPacked(kRgbaDarkSlateGrey)};
constexpr u32 kRgbaPackedDarkTurquoise{rgbaToPacked(kRgbaDarkTurquoise)};
constexpr u32 kRgbaPackedDarkViolet{rgbaToPacked(kRgbaDarkViolet)};
constexpr u32 kRgbaPackedDeepPink{rgbaToPacked(kRgbaDeepPink)};
constexpr u32 kRgbaPackedDeepSkyBlue{rgbaToPacked(kRgbaDeepSkyBlue)};
constexpr u32 kRgbaPackedDimGray{rgbaToPacked(kRgbaDimGray)};
constexpr u32 kRgbaPackedDimGrey{rgbaToPacked(kRgbaDimGrey)};
constexpr u32 kRgbaPackedDodgerBlue{rgbaToPacked(kRgbaDodgerBlue)};
constexpr u32 kRgbaPackedFireBrick{rgbaToPacked(kRgbaFireBrick)};
constexpr u32 kRgbaPackedFloralWhite{rgbaToPacked(kRgbaFloralWhite)};
constexpr u32 kRgbaPackedForestGreen{rgbaToPacked(kRgbaForestGreen)};
constexpr u32 kRgbaPackedFuchsia{rgbaToPacked(kRgbaFuchsia)};
constexpr u32 kRgbaPackedGainsboro{rgbaToPacked(kRgbaGainsboro)};
constexpr u32 kRgbaPackedGhostWhite{rgbaToPacked(kRgbaGhostWhite)};
constexpr u32 kRgbaPackedGold{rgbaToPacked(kRgbaGold)};
constexpr u32 kRgbaPackedGoldenrod{rgbaToPacked(kRgbaGoldenrod)};
constexpr u32 kRgbaPackedGray{rgbaToPacked(kRgbaGray)};
constexpr u32 kRgbaPackedGreen{rgbaToPacked(kRgbaGreen)};
constexpr u32 kRgbaPackedGreenYellow{rgbaToPacked(kRgbaGreenYellow)};
constexpr u32 kRgbaPackedGrey{rgbaToPacked(kRgbaGrey)};
constexpr u32 kRgbaPackedHoneydew{rgbaToPacked(kRgbaHoneydew)};
constexpr u32 kRgbaPackedHotPink{rgbaToPacked(kRgbaHotPink)};
constexpr u32 kRgbaPackedIndianRed{rgbaToPacked(kRgbaIndianRed)};
constexpr u32 kRgbaPackedIndigo{rgbaToPacked(kRgbaIndigo)};
constexpr u32 kRgbaPackedIvory{rgbaToPacked(kRgbaIvory)};
constexpr u32 kRgbaPackedKhaki{rgbaToPacked(kRgbaKhaki)};
constexpr u32 kRgbaPackedLavender{rgbaToPacked(kRgbaLavender)};
constexpr u32 kRgbaPackedLavenderBlush{rgbaToPacked(kRgbaLavenderBlush)};
constexpr u32 kRgbaPackedLawnGreen{rgbaToPacked(kRgbaLawnGreen)};
constexpr u32 kRgbaPackedLemonChiffon{rgbaToPacked(kRgbaLemonChiffon)};
constexpr u32 kRgbaPackedLightBlue{rgbaToPacked(kRgbaLightBlue)};
constexpr u32 kRgbaPackedLightCoral{rgbaToPacked(kRgbaLightCoral)};
constexpr u32 kRgbaPackedLightCyan{rgbaToPacked(kRgbaLightCyan)};
constexpr u32 kRgbaPackedLightGoldenrodYellow{
    rgbaToPacked(kRgbaLightGoldenrodYellow)};
constexpr u32 kRgbaPackedLightGray{rgbaToPacked(kRgbaLightGray)};
constexpr u32 kRgbaPackedLightGreen{rgbaToPacked(kRgbaLightGreen)};
constexpr u32 kRgbaPackedLightGrey{rgbaToPacked(kRgbaLightGrey)};
constexpr u32 kRgbaPackedLightPink{rgbaToPacked(kRgbaLightPink)};
constexpr u32 kRgbaPackedLightSalmon{rgbaToPacked(kRgbaLightSalmon)};
constexpr u32 kRgbaPackedLightSeaGreen{rgbaToPacked(kRgbaLightSeaGreen)};
constexpr u32 kRgbaPackedLightSkyBlue{rgbaToPacked(kRgbaLightSkyBlue)};
constexpr u32 kRgbaPackedLightSlateGray{rgbaToPacked(kRgbaLightSlateGray)};
constexpr u32 kRgbaPackedLightSlateGrey{rgbaToPacked(kRgbaLightSlateGrey)};
constexpr u32 kRgbaPackedLightSteelBlue{rgbaToPacked(kRgbaLightSteelBlue)};
constexpr u32 kRgbaPackedLightYellow{rgbaToPacked(kRgbaLightYellow)};
constexpr u32 kRgbaPackedLime{rgbaToPacked(kRgbaLime)};
constexpr u32 kRgbaPackedLimeGreen{rgbaToPacked(kRgbaLimeGreen)};
constexpr u32 kRgbaPackedLinen{rgbaToPacked(kRgbaLinen)};
constexpr u32 kRgbaPackedMagenta{rgbaToPacked(kRgbaMagenta)};
constexpr u32 kRgbaPackedMaroon{rgbaToPacked(kRgbaMaroon)};
constexpr u32 kRgbaPackedMediumAquamarine{rgbaToPacked(kRgbaMediumAquamarine)};
constexpr u32 kRgbaPackedMediumBlue{rgbaToPacked(kRgbaMediumBlue)};
constexpr u32 kRgbaPackedMediumOrchid{rgbaToPacked(kRgbaMediumOrchid)};
constexpr u32 kRgbaPackedMediumPurple{rgbaToPacked(kRgbaMediumPurple)};
constexpr u32 kRgbaPackedMediumSeaGreen{rgbaToPacked(kRgbaMediumSeaGreen)};
constexpr u32 kRgbaPackedMediumSlateBlue{rgbaToPacked(kRgbaMediumSlateBlue)};
constexpr u32 kRgbaPackedMediumSpringGreen{
    rgbaToPacked(kRgbaMediumSpringGreen)};
constexpr u32 kRgbaPackedMediumTurquoise{rgbaToPacked(kRgbaMediumTurquoise)};
constexpr u32 kRgbaPackedMediumVioletRed{rgbaToPacked(kRgbaMediumVioletRed)};
constexpr u32 kRgbaPackedMidnightBlue{rgbaToPacked(kRgbaMidnightBlue)};
constexpr u32 kRgbaPackedMintCream{rgbaToPacked(kRgbaMintCream)};
constexpr u32 kRgbaPackedMistyRose{rgbaToPacked(kRgbaMistyRose)};
constexpr u32 kRgbaPackedMoccasin{rgbaToPacked(kRgbaMoccasin)};
constexpr u32 kRgbaPackedNavajoWhite{rgbaToPacked(kRgbaNavajoWhite)};
constexpr u32 kRgbaPackedNavy{rgbaToPacked(kRgbaNavy)};
constexpr u32 kRgbaPackedOldLace{rgbaToPacked(kRgbaOldLace)};
constexpr u32 kRgbaPackedOlive{rgbaToPacked(kRgbaOlive)};
constexpr u32 kRgbaPackedOliveDrab{rgbaToPacked(kRgbaOliveDrab)};
constexpr u32 kRgbaPackedOrange{rgbaToPacked(kRgbaOrange)};
constexpr u32 kRgbaPackedOrangeRed{rgbaToPacked(kRgbaOrangeRed)};
constexpr u32 kRgbaPackedOrchid{rgbaToPacked(kRgbaOrchid)};
constexpr u32 kRgbaPackedPaleGoldenrod{rgbaToPacked(kRgbaPaleGoldenrod)};
constexpr u32 kRgbaPackedPaleGreen{rgbaToPacked(kRgbaPaleGreen)};
constexpr u32 kRgbaPackedPaleTurquoise{rgbaToPacked(kRgbaPaleTurquoise)};
constexpr u32 kRgbaPackedPaleVioletRed{rgbaToPacked(kRgbaPaleVioletRed)};
constexpr u32 kRgbaPackedPapayaWhip{rgbaToPacked(kRgbaPapayaWhip)};
constexpr u32 kRgbaPackedPeachPuff{rgbaToPacked(kRgbaPeachPuff)};
constexpr u32 kRgbaPackedPeru{rgbaToPacked(kRgbaPeru)};
constexpr u32 kRgbaPackedPink{rgbaToPacked(kRgbaPink)};
constexpr u32 kRgbaPackedPlum{rgbaToPacked(kRgbaPlum)};
constexpr u32 kRgbaPackedPowderBlue{rgbaToPacked(kRgbaPowderBlue)};
constexpr u32 kRgbaPackedPurple{rgbaToPacked(kRgbaPurple)};
constexpr u32 kRgbaPackedRebeccaPurple{rgbaToPacked(kRgbaRebeccaPurple)};
constexpr u32 kRgbaPackedRed{rgbaToPacked(kRgbaRed)};
constexpr u32 kRgbaPackedRosyBrown{rgbaToPacked(kRgbaRosyBrown)};
constexpr u32 kRgbaPackedRoyalBlue{rgbaToPacked(kRgbaRoyalBlue)};
constexpr u32 kRgbaPackedSaddleBrown{rgbaToPacked(kRgbaSaddleBrown)};
constexpr u32 kRgbaPackedSalmon{rgbaToPacked(kRgbaSalmon)};
constexpr u32 kRgbaPackedSandyBrown{rgbaToPacked(kRgbaSandyBrown)};
constexpr u32 kRgbaPackedSeaGreen{rgbaToPacked(kRgbaSeaGreen)};
constexpr u32 kRgbaPackedSeashell{rgbaToPacked(kRgbaSeashell)};
constexpr u32 kRgbaPackedSienna{rgbaToPacked(kRgbaSienna)};
constexpr u32 kRgbaPackedSilver{rgbaToPacked(kRgbaSilver)};
constexpr u32 kRgbaPackedSkyBlue{rgbaToPacked(kRgbaSkyBlue)};
constexpr u32 kRgbaPackedSlateBlue{rgbaToPacked(kRgbaSlateBlue)};
constexpr u32 kRgbaPackedSlateGray{rgbaToPacked(kRgbaSlateGray)};
constexpr u32 kRgbaPackedSlateGrey{rgbaToPacked(kRgbaSlateGrey)};
constexpr u32 kRgbaPackedSnow{rgbaToPacked(kRgbaSnow)};
constexpr u32 kRgbaPackedSpringGreen{rgbaToPacked(kRgbaSpringGreen)};
constexpr u32 kRgbaPackedSteelBlue{rgbaToPacked(kRgbaSteelBlue)};
constexpr u32 kRgbaPackedTan{rgbaToPacked(kRgbaTan)};
constexpr u32 kRgbaPackedTeal{rgbaToPacked(kRgbaTeal)};
constexpr u32 kRgbaPackedThistle{rgbaToPacked(kRgbaThistle)};
constexpr u32 kRgbaPackedTomato{rgbaToPacked(kRgbaTomato)};
constexpr u32 kRgbaPackedTransparent{rgbaToPacked(kRgbaTransparent)};
constexpr u32 kRgbaPackedTurquoise{rgbaToPacked(kRgbaTurquoise)};
constexpr u32 kRgbaPackedViolet{rgbaToPacked(kRgbaViolet)};
constexpr u32 kRgbaPackedWheat{rgbaToPacked(kRgbaWheat)};
constexpr u32 kRgbaPackedWhite{rgbaToPacked(kRgbaWhite)};
constexpr u32 kRgbaPackedWhiteSmoke{rgbaToPacked(kRgbaWhiteSmoke)};
constexpr u32 kRgbaPackedYellow{rgbaToPacked(kRgbaYellow)};
constexpr u32 kRgbaPackedYellowGreen{rgbaToPacked(kRgbaYellowGreen)};

inline Rgba lerp(const Rgba& a, const Rgba& b, f32 u) {
  return {static_cast<u8>(a.r + (b.r - a.r) * u),
          static_cast<u8>(a.g + (b.g - a.g) * u),
          static_cast<u8>(a.b + (b.b - a.b) * u),
          static_cast<u8>(a.a + (b.a - a.a) * u)};
}

inline constexpr Rgba mixMultiply(const Rgba& a, const Rgba& b) noexcept {
  auto an = rgbaToRgbaNorm(a);
  auto bn = rgbaToRgbaNorm(b);
  return rgbaNormToRgba({an.r * bn.r, an.g * bn.g, an.b * bn.b, an.a * bn.a});
}

inline Rgba mixMultiply(const Rgba& a, const Rgba& b, f32 u) {
  return lerp(a, mixMultiply(a, b), u);
}

inline constexpr Rgba gradientMultiply(std::span<const Rgba> colors,
                                       f32 t) noexcept {
  if (colors.empty()) return kRgbaWhite;
  if (colors.size() == 1) return colors[0];
  t = std::clamp(t, .0f, 1.0f);

  auto pos = t * static_cast<f32>(colors.size() - 1);
  auto i = static_cast<usize>(std::floor(pos));
  auto j = std::min(i + 1, colors.size() - 1);
  auto u = pos - static_cast<f32>(i);

  return mixMultiply(colors[i], colors[j], u);
}
}  // namespace rl

#endif  // ENGINE_CORE_COLOR_H_
