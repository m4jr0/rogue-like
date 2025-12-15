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
#include "render_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/phase_bus.h"
#include "engine/render/opengl_render_device.h"

namespace rl {
RenderSystem& RenderSystem::instance() {
  static RenderSystem inst;
  return inst;
}

void RenderSystem::init() {
  RL_LOG_DEBUG("RenderSystem::init");
  device_ = std::make_unique<OpenGlRenderDevice>();
  device_->init(kWindowWidth, kWindowHeight, kWindowTitle);
}

void RenderSystem::shutdown() {
  RL_LOG_DEBUG("RenderSystem::shutdown");
  device_->shutdown();
  device_.reset();
}

void RenderSystem::update(const FramePacket& f) {
  RL_PHASEBUS.invoke(TickPhase::Update, f);
  device_->render(queue_);
  RL_RENDER_DEBUG_UPDATE(f);
  queue_.nextFrame();
}

void RenderSystem::refSize(WindowSize w, WindowSize h) {
  device_->refSize(w, h);
}

#ifdef RL_DEBUG_RENDER
void RenderSystem::debugUpdate(const FramePacket& f) {
  fpsTime_ += f.delta;
  ++fpsCount_;

  if (fpsTime_ >= 1.0) {
    auto fps = static_cast<f64>(fpsCount_) / fpsTime_;
    setFpsToWindowTitle(fps);
    fpsTime_ = 0.0;
    fpsCount_ = 0;
  }
}

void RenderSystem::setFpsToWindowTitle(f64 fps) {
  char title[128];
  std::snprintf(title, sizeof(title), "%s  |  %.0f FPS",
                std::string(kWindowTitle).c_str(), fps);
  device_->windowTitle(title);
}
#endif  // RL_DEBUG_RENDER
}  // namespace rl