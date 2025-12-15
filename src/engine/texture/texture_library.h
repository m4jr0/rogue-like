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

#ifndef ENGINE_TEXTURE_TEXTURE_LIBRARY_H_
#define ENGINE_TEXTURE_TEXTURE_LIBRARY_H_

#include "engine/common.h"
#include "engine/render/render_device.h"
#include "engine/resource/resource_slot.h"
#include "engine/texture/texture.h"
#include "engine/texture/texture_resource.h"

namespace rl {
class TextureLibrary {
 public:
  static TextureLibrary& instance();

  void init();
  void shutdown();

  const TextureResource* load(TextureId id);
  void unload(TextureId id);
  const TextureResource* get(TextureId id) const;

  bool gpuHandle(TextureId id, u64& out) const;
  bool size(TextureId id, TextureExtent& out) const;

 private:
  struct TexEntry {
    u64 gpuHandle{0};
    TextureResource resource{};
  };

  RenderDevice* rDevice_{nullptr};
  ResourceSlots<TextureTag, TexEntry> slots_{};

  TextureLibrary() = default;
};
}  // namespace rl

#define RL_TEXLIB (::rl::TextureLibrary::instance())
#define RL_CTEXLIB \
  (static_cast<const ::rl::TextureLibrary&>(::rl::TextureLibrary::instance()))

#endif  // ENGINE_TEXTURE_TEXTURE_LIBRARY_H_
