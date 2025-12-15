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

#ifndef ENGINE_RENDER_RENDER_DEVICE_H_
#define ENGINE_RENDER_RENDER_DEVICE_H_

#include "engine/common.h"
#include "engine/render/render_common.h"
#include "engine/render/render_queue.h"
#include "engine/texture/texture.h"

namespace rl {
class RenderDevice {
 public:
  virtual void init(WindowSize width, WindowSize height,
                    std::string_view windowTitle = kDefaultWindowTitle_,
                    WindowSize refWidth = kDefaultRefWidth_,
                    WindowSize refHeight = kDefaultRefHeight_) = 0;
  virtual void shutdown() = 0;

  virtual void render(RenderQueue& queue) = 0;
  virtual void windowTitle(std::string_view title) = 0;
  virtual void refSize(WindowSize w, WindowSize h) = 0;

  virtual bool generateTexture(const TextureExtent& size, const void* data,
                               u64& out) = 0;
  virtual void destroyTexture(u64 handle) = 0;

  virtual const Viewport& viewport() const noexcept = 0;
  virtual void* window() = 0;

  virtual ~RenderDevice() = default;

 protected:
  static inline const char* kDefaultWindowTitle_ = "Game";
  static inline constexpr WindowSize kDefaultRefWidth_ = 640;
  static inline constexpr WindowSize kDefaultRefHeight_ = 480;
  WindowSize refWidth_{kDefaultRefWidth_};
  WindowSize refHeight_{kDefaultRefHeight_};
};
}  // namespace rl

#endif  // ENGINE_RENDER_RENDER_DEVICE_H_
