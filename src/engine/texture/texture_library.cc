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
#include "texture_library.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/render/render_system.h"
#include "engine/resource/resource_file.h"
#include "engine/resource/resource_type_registry.h"
#include "engine/texture/texture_serialize.h"

namespace rl {
TextureLibrary& TextureLibrary::instance() {
  static TextureLibrary inst;
  return inst;
}

void TextureLibrary::init() {
  RL_LOG_DEBUG("TextureLibrary::init");
  RL_RESREG.on(kResourceTypeTexture, "tex");
  rDevice_ = RL_RENDERSYS.device();
  RL_ASSERT(rDevice_, "TextureLibrary::init: No render device found!");
  constexpr auto kDefaultTexCap = 128;
  slots_.reserve(kDefaultTexCap);
}

void TextureLibrary::shutdown() {
  RL_LOG_DEBUG("TextureLibrary::shutdown");
  RL_RESREG.off(kResourceTypeTexture);

  if (rDevice_) {
    for (const auto& pair : slots_) {
      const auto* entry = pair.second.value.get();

      if (entry->gpuHandle) rDevice_->destroyTexture(entry->gpuHandle);
    }
  }

  rDevice_ = nullptr;
  slots_.clear();
}

const TextureResource* TextureLibrary::load(TextureId id) {
  return &slots_
              .acquire(id,
                       [&, id] {
                         auto* entry = new TexEntry{};
                         auto& res = entry->resource;

                         auto ok = loadResourceFile(
                             kResourceTypeTexture, id,
                             TextureResource::kVersion,
                             TextureResource::kVersion, [&](std::istream& is) {
                               readTextureResource(is, res);
                             });

                         RL_FASSERT(ok,
                                    "TextureLibrary::load: Failed to load "
                                    "texture resource with id: ",
                                    id, "!");

                         RL_FASSERT(
                             rDevice_->generateTexture(
                                 res.size,
                                 res.data.empty() ? nullptr : res.data.data(),
                                 entry->gpuHandle),
                             "TextureLibrary::load: Could not generate texture "
                             "for id: ",
                             id, "!");

                         return entry;
                       })
              ->resource;
}

void TextureLibrary::unload(TextureId id) {
  const auto* e = slots_.get(id);
  u64 handle = 0;

  if (e) {
    handle = e->gpuHandle;
  }

  slots_.release(id);

  if (handle && !slots_.has(id)) {
    rDevice_->destroyTexture(handle);
  }
}

const TextureResource* TextureLibrary::get(TextureId id) const {
  const auto* entry = slots_.get(id);
  return entry ? &entry->resource : nullptr;
}

bool TextureLibrary::gpuHandle(TextureId id, u64& out) const {
  const auto* entry = slots_.get(id);
  auto exist = !!entry;
  out = exist ? entry->gpuHandle : 0;
  return exist;
}

bool TextureLibrary::size(TextureId id, TextureExtent& out) const {
  const auto* entry = slots_.get(id);
  auto exist = !!entry;
  out = exist ? entry->resource.size : TextureExtent{};
  return exist;
}
}  // namespace rl