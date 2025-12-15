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

#ifndef ENGINE_RENDER_RENDER_SYSTEM_H_
#define ENGINE_RENDER_RENDER_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/render/render_common.h"
#include "engine/render/render_device.h"
#include "engine/render/render_queue.h"

#define RL_DEBUG_RENDER

namespace rl {
class RenderSystem {
 public:
  static RenderSystem& instance();

  void init();
  void shutdown();

  void update(const FramePacket& f);
  virtual void refSize(WindowSize w, WindowSize h);

  RenderDevice* device() { return device_.get(); }
  const RenderDevice* device() const { return device_.get(); }

  RenderQueue* queue() { return &queue_; }
  const RenderQueue* queue() const { return &queue_; }

  const Viewport& viewport() const noexcept { return device_->viewport(); }

 private:
  static constexpr WindowSize kWindowWidth = 1280;
  static constexpr WindowSize kWindowHeight = 720;
  static constexpr std::string_view kWindowTitle = "Rogue Like";

  std::unique_ptr<RenderDevice> device_{nullptr};
  RenderQueue queue_{};

  RenderSystem() = default;

#ifdef RL_DEBUG_RENDER
  f64 fpsTime_{0.0};
  u32 fpsCount_{0};
  void debugUpdate(const FramePacket& f);
  void setFpsToWindowTitle(f64 fps);
#define RL_RENDER_DEBUG_UPDATE(f) debugUpdate(f)
#else
#define RL_RENDER_DEBUG_UPDATE(f) ((void)0)
#endif  // RL_DEBUG_RENDER
};
}  // namespace rl

#define RL_RENDERSYS (::rl::RenderSystem::instance())
#define RL_CRENDERSYS \
  (static_cast<const ::rl::RenderSystem&>(::rl::RenderSystem::instance()))

#endif  // ENGINE_RENDER_RENDER_SYSTEM_H_
