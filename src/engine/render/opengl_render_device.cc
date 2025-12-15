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
#include "opengl_render_device.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/camera/camera.h"
#include "engine/camera/camera_system.h"
#include "engine/core/color.h"
#include "engine/core/core_message.h"
#include "engine/core/log.h"
#include "engine/event/event_system.h"
#include "engine/render/render_queue.h"
#include "engine/render/render_utils.h"
#include "engine/texture/texture_library.h"

namespace rl {
namespace internal {
static OpenGlRenderDevice* gSelf = nullptr;
}  // namespace internal

void OpenGlRenderDevice::init(WindowSize width, WindowSize height,
                              std::string_view windowTitle, WindowSize refWidth,
                              WindowSize refHeight) {
  internal::gSelf = this;
  RL_FASSERT(glfwInit(),
             "OpenGlRenderDevice::init: Could not initialize GLFW!");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef RL_DEBUG_RENDER
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif  // RL_DEBUG_RENDER

  refWidth_ = refWidth;
  refHeight_ = refHeight;
  window_ = glfwCreateWindow(static_cast<s32>(width), static_cast<s32>(height),
                             windowTitle.data(), nullptr, nullptr);

  RL_ASSERT(window_, "OpenGlRenderDevice::init: Could not create GLFW window!");

  if (!window_) {
    shutdown();
    return;
  }

  glfwMakeContextCurrent(window_);
  RL_FASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
             "OpenGlRenderDevice::init: Unable to initialize GLAD!");
  bindCallbacks();
  glfwSwapInterval(1);

  glViewport(0, 0, static_cast<s32>(width), static_cast<s32>(height));

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glClearColor(.0f, .0f, .0f, 1.0f);

  const char* vsSrc = R"(
#version 460 core

layout(location=0) in vec2 aLocal;
layout(location=1) in vec2 aUV;
layout(location=2) in vec4 aColor;
layout(location=3) in vec2 iRS0;
layout(location=4) in vec2 iRS1;
layout(location=5) in vec2 iT;
layout(location=6) in vec4 iUVRect;
layout(location=7) in vec4 iTint;

uniform mat4 uProj;

out vec2 vUV;
out vec4 vColor;

void main() {
  vec2 world;
  world.x = dot(vec2(iRS0.x, iRS1.x), aLocal) + iT.x;
  world.y = dot(vec2(iRS0.y, iRS1.y), aLocal) + iT.y;

  gl_Position = uProj * vec4(world, .0, 1.0);

  vUV = mix(iUVRect.xy, iUVRect.zw, aUV);
  vColor = aColor * iTint;
}
)";

  const char* fsSrc = R"(
#version 460 core

in vec2 vUV;
in vec4 vColor;

uniform sampler2D uTex;

out vec4 oColor;

void main() {
  vec4 base = texture(uTex, vUV);
  oColor = base * vColor;
}
)";

  prog_ = makeProgram(vsSrc, fsSrc);
  uProj_ = glGetUniformLocation(prog_, "uProj");
  uTex_ = glGetUniformLocation(prog_, "uTex");
  glGenVertexArrays(1, &quadVao_);
  glBindVertexArray(quadVao_);

  const Vertex kUnitQuad[6] = {
      {.x = 0, .y = 0, .u = 0, .v = 0, .rgba = kRgbaPackedWhite},
      {.x = 1, .y = 0, .u = 1, .v = 0, .rgba = kRgbaPackedWhite},
      {.x = 1, .y = 1, .u = 1, .v = 1, .rgba = kRgbaPackedWhite},
      {.x = 0, .y = 0, .u = 0, .v = 0, .rgba = kRgbaPackedWhite},
      {.x = 1, .y = 1, .u = 1, .v = 1, .rgba = kRgbaPackedWhite},
      {.x = 0, .y = 1, .u = 0, .v = 1, .rgba = kRgbaPackedWhite},
  };

  glGenBuffers(1, &quadVbo_);
  glBindBuffer(GL_ARRAY_BUFFER, quadVbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(kUnitQuad), kUnitQuad, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(0));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, u)));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, rgba)));

  glGenBuffers(1, &instanceVbo_);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVbo_);

  constexpr auto kInstanceStride = static_cast<GLsizei>(sizeof(DrawInstance));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, kInstanceStride,
                        reinterpret_cast<void*>(offsetof(DrawInstance, rs0x)));
  glVertexAttribDivisor(3, 1);

  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, kInstanceStride,
                        reinterpret_cast<void*>(offsetof(DrawInstance, rs1x)));
  glVertexAttribDivisor(4, 1);

  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, kInstanceStride,
                        reinterpret_cast<void*>(offsetof(DrawInstance, tx)));
  glVertexAttribDivisor(5, 1);

  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, kInstanceStride,
                        reinterpret_cast<void*>(offsetof(DrawInstance, u0)));
  glVertexAttribDivisor(6, 1);

  glEnableVertexAttribArray(7);
  glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, kInstanceStride,
                        reinterpret_cast<void*>(offsetof(DrawInstance, r)));
  glVertexAttribDivisor(7, 1);

  glBindVertexArray(0);

  generateTexture({1, 1}, &kRgbaPackedWhite, colorOnlyTxPayload_);
  colorOnlyTx_ = static_cast<GLuint>(colorOnlyTxPayload_);
  refreshViewport();
}

void OpenGlRenderDevice::shutdown() {
  if (colorOnlyTxPayload_) {
    destroyTexture(colorOnlyTxPayload_);
    colorOnlyTxPayload_ = 0;
    colorOnlyTx_ = 0;
  }

  if (quadVao_) {
    glDeleteVertexArrays(1, &quadVao_);
    quadVao_ = 0;
  }

  if (quadVbo_) {
    glDeleteBuffers(1, &quadVbo_);
    quadVbo_ = 0;
  }

  if (instanceVbo_) {
    glDeleteBuffers(1, &instanceVbo_);
    instanceVbo_ = 0;
  }

  if (prog_) {
    glDeleteProgram(prog_);
    prog_ = 0;
  }

  unbindCallbacks();

  if (window_) {
    glfwDestroyWindow(window_);
    window_ = nullptr;
  }

  glfwTerminate();
  internal::gSelf = nullptr;
}

void OpenGlRenderDevice::render(RenderQueue& queue) {
  glEnable(GL_SCISSOR_TEST);
  glScissor(viewport_.pos.x, viewport_.pos.y, viewport_.size.x,
            viewport_.size.y);
  glClearColor(clearColor_.r, clearColor_.g, clearColor_.b, clearColor_.a);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_SCISSOR_TEST);

  draw(queue);
  glfwSwapBuffers(window_);
}

void OpenGlRenderDevice::windowTitle(std::string_view title) {
  glfwSetWindowTitle(window_, title.data());
}

void OpenGlRenderDevice::refSize(WindowSize w, WindowSize h) {
  refWidth_ = w;
  refHeight_ = h;
}

bool OpenGlRenderDevice::generateTexture(const TextureExtent& size,
                                         const void* data, u64& out) {
  GLuint id = 0;

  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(size.x),
               static_cast<GLsizei>(size.y), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               data);

  out = static_cast<u64>(id);
  return id != 0;
}

void OpenGlRenderDevice::destroyTexture(u64 handle) {
  if (handle) {
    auto id = static_cast<GLuint>(handle);
    glDeleteTextures(1, &id);
  }
}

GLuint OpenGlRenderDevice::makeShader(GLenum type, const char* src) {
  auto s = glCreateShader(type);
  glShaderSource(s, 1, &src, nullptr);
  glCompileShader(s);
  GLint ok = 0;
  glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
  RL_ASSERT(ok, "OpenGlRenderDevice::makeShader: GL shader compile failed");
  return s;
}

GLuint OpenGlRenderDevice::makeProgram(const char* vs, const char* fs) {
  auto v = makeShader(GL_VERTEX_SHADER, vs);
  auto f = makeShader(GL_FRAGMENT_SHADER, fs);
  auto p = glCreateProgram();
  glAttachShader(p, v);
  glAttachShader(p, f);
  glLinkProgram(p);
  GLint ok = 0;
  glGetProgramiv(p, GL_LINK_STATUS, &ok);
  RL_ASSERT(ok, "OpenGlRenderDevice::makeProgram: GL program link failed");
  glDeleteShader(v);
  glDeleteShader(f);
  return p;
}

void OpenGlRenderDevice::bindCallbacks() {
  glfwSetErrorCallback([](auto code, const auto* desc) {
    RL_LOG_ERR("GLFW Error [", code, "]", desc);
  });

  glfwSetFramebufferSizeCallback(window_, [](auto*, auto w, auto h) {
    internal::gSelf->refreshViewport(static_cast<WindowSize>(w),
                                     static_cast<WindowSize>(h));
  });

  glfwSetWindowCloseCallback(
      window_, [](auto*) { RL_EVENTSYS.dispatch(kCoreMessageIdQuit); });

#ifdef RL_DEBUG_RENDER
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback([](auto, auto, auto, auto, auto, const auto* msg,
                            const void*) { RL_LOG_DEBUG("[OpenGL] ", msg); },
                         nullptr);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif  // RL_DEBUG_RENDER
}

void OpenGlRenderDevice::unbindCallbacks() {
  glfwSetWindowCloseCallback(window_, nullptr);
  glfwSetFramebufferSizeCallback(window_, nullptr);
  glfwSetErrorCallback(nullptr);

#ifdef RL_DEBUG_RENDER
  glDebugMessageCallback(nullptr, nullptr);
  glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDisable(GL_DEBUG_OUTPUT);
#endif  // RL_DEBUG_RENDER
}

void OpenGlRenderDevice::draw(RenderQueue& queue) {
  std::vector<RenderBatch> batches;
  queue.batches(batches);
  const auto* main = RL_CAMSYS.main();
  RL_ASSERT(main, "OpenGlRenderDevice::draw: No main camera set!");

  glUseProgram(prog_);
  glUniformMatrix4fv(uProj_, 1, GL_FALSE, main->viewProj.data());

  glActiveTexture(GL_TEXTURE0);
  glUniform1i(uTex_, 0);
  glBindVertexArray(quadVao_);

  GLuint lastGlTex = 0;

  for (const auto& b : batches) {
    GLuint glTex;

    if (b.tex) {
      u64 gpuHandle;
      if (!RL_TEXLIB.gpuHandle(b.tex, gpuHandle)) continue;  // Stale handle.
      glTex = static_cast<GLuint>(gpuHandle);
    } else {
      glTex = colorOnlyTx_;
    }

    if (glTex != lastGlTex) {
      glBindTexture(GL_TEXTURE_2D, glTex);
      lastGlTex = glTex;
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVbo_);
    glBufferData(GL_ARRAY_BUFFER, b.instances.size() * sizeof(DrawInstance),
                 b.instances.data(), GL_STREAM_DRAW);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6,
                          static_cast<GLsizei>(b.instances.size()));
  }

  glBindVertexArray(0);
  glUseProgram(0);
}

void OpenGlRenderDevice::refreshViewport() {
  s32 w;
  s32 h;
  glfwGetFramebufferSize(window_, &w, &h);
  refreshViewport(static_cast<WindowSize>(w), static_cast<WindowSize>(h));
}

void OpenGlRenderDevice::refreshViewport(WindowSize w, WindowSize h) {
  viewport_ = fitInside(w, h, refWidth_, refHeight_);
  glViewport(viewport_.pos.x, viewport_.pos.y, viewport_.size.x,
             viewport_.size.y);
  glScissor(viewport_.pos.x, viewport_.pos.y, viewport_.size.x,
            viewport_.size.y);
  RL_CAMSYS.resizeAll(viewport_);
}
}  // namespace rl