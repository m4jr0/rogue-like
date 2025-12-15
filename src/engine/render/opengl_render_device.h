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

#ifndef ENGINE_RENDER_OPENGL_RENDER_DEVICE_H_
#define ENGINE_RENDER_OPENGL_RENDER_DEVICE_H_

#include "engine/common.h"
#include "engine/core/color.h"
#include "engine/render/render_device.h"
#include "engine/texture/texture.h"

namespace rl {
class OpenGlRenderDevice : public RenderDevice {
 public:
  void init(WindowSize width, WindowSize height,
            std::string_view windowTitle = kDefaultWindowTitle_,
            WindowSize refWidth = kDefaultRefWidth_,
            WindowSize refHeight = kDefaultRefHeight_) override;
  void shutdown() override;

  void render(RenderQueue& queue) override;
  void windowTitle(std::string_view title) override;
  void refSize(WindowSize w, WindowSize h) override;

  bool generateTexture(const TextureExtent& size, const void* data,
                       u64& out) override;
  void destroyTexture(u64 handle) override;

  const Viewport& viewport() const noexcept override { return viewport_; }
  void* window() override { return window_; }

 private:
  GLuint prog_{0};
  GLuint quadVao_{0};
  GLuint quadVbo_{0};
  GLuint instanceVbo_{0};
  GLuint colorOnlyTx_{0};
  u64 colorOnlyTxPayload_{0};
  Viewport viewport_{};

  GLint uProj_{-1};
  GLint uTex_{-1};

  GLFWwindow* window_{nullptr};

  RgbaNorm clearColor_{.5f, .5f, .5f, 1.0f};

  static GLuint makeShader(GLenum type, const char* src);
  static GLuint makeProgram(const char* vs, const char* fs);

  void bindCallbacks();
  void unbindCallbacks();
  void draw(RenderQueue& queue);
  void refreshViewport();
  void refreshViewport(WindowSize w, WindowSize h);
};
}  // namespace rl

#endif  // ENGINE_RENDER_OPENGL_RENDER_DEVICE_H_
