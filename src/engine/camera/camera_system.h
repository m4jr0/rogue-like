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

#ifndef CAMERA_CAMERA_SYSTEM_H_
#define CAMERA_CAMERA_SYSTEM_H_

#include "engine/camera/camera.h"
#include "engine/common.h"
#include "engine/core/handle.h"
#include "engine/core/value_utils.h"
#include "engine/transform/transform.h"

namespace rl {
class CameraSystem {
 public:
  static CameraSystem& instance();

  void init();
  void shutdown();

  [[nodiscard]] CameraHandle generate(const CameraDesc& desc);
  void destroy(CameraHandle h);

  void resizeAll(const Viewport& v);

  void moveTo(CameraHandle h, const Position& p, bool centered = true) {
    return moveTo(cam(h), p, centered);
  }

  void moveTo(Camera* c, const Position& p, bool centered = true);

  void moveTo(const Position& p, bool centered = true) {
    moveTo(main(), p, centered);
  }

  void zoom(CameraHandle h, CameraZoom z, const Position* w = nullptr) {
    zoom(cam(h), z, w);
  }

  void zoom(Camera* c, CameraZoom z, const Position* w = nullptr);
  void zoom(CameraZoom z, const Position* w = nullptr) { zoom(main(), z, w); }

  void zoomDelta(CameraHandle h, CameraZoom delta,
                 const Position* w = nullptr) {
    zoomDelta(cam(h), delta, w);
  }

  void zoomDelta(Camera* c, CameraZoom delta, const Position* w = nullptr);

  void zoomDelta(CameraZoom delta, const Position* w = nullptr) {
    zoomDelta(main(), delta, w);
  }

  Position worldCenter(CameraHandle h) const { return worldCenter(cam(h)); }
  Position worldCenter(const Camera* c) const;
  Position worldCenter() const { return worldCenter(main()); }

  Position worldToScreen(CameraHandle h, const Position& w) const {
    return worldToScreen(cam(h), w);
  }

  Position worldToScreen(const Camera* c, const Position& w) const;

  Position worldToScreen(const Position& w) const {
    return worldToScreen(cam(mainCam_), w);
  }

  Position screenToWorld(CameraHandle h, const Position& s) const {
    return screenToWorld(cam(h), s);
  }

  Position screenToWorld(const Camera* c, const Position& s) const;

  Position screenToWorld(const Position& s) const {
    return screenToWorld(cam(mainCam_), s);
  }

  void viewport(CameraHandle h, const CameraViewport& v) {
    return viewport(cam(h), v);
  }

  void viewport(Camera* c, const CameraViewport& v);

  void viewport(const CameraViewport& v) { return viewport(cam(mainCam_), v); }

  Camera* cam(CameraHandle h);
  const Camera* cam(CameraHandle h) const;
  const Camera* main() const;
  Camera* main();

 private:
  static inline constexpr CameraZoom kMinZoom_{.1f};
  static inline constexpr CameraZoom kMaxZoom_{15.0f};

  HandlePool<CameraTag> hCamPool_{};
  CameraHandle mainCam_{kInvalidHandle};
  std::vector<Camera> cams_{};

  static void buildViewProj(Camera* c);

  static CameraViewport toCamViewport(const Viewport& v);

  CameraSystem() = default;

  static CameraZoom zoomSafe(CameraZoom z) {
    return std::min(avoidNegOrZero(z, kMinZoom_), kMaxZoom_);
  }

  static CameraZoom viewportZoom(const Camera* c) {
    return c ? zoomSafe(c->viewport.zoom) : kMinZoom_;
  }

  static CameraZoom userZoom(const Camera* c) {
    return c ? zoomSafe(c->zoom) : kMinZoom_;
  }

  static CameraZoom appliedZoom(const Camera* c) {
    return viewportZoom(c) * userZoom(c);
  }
};
}  // namespace rl

#define RL_CAMSYS (::rl::CameraSystem::instance())
#define RL_CCAMSYS \
  (static_cast<const ::rl::CameraSystem&>(::rl::CameraSystem::instance()))

#endif  // CAMERA_CAMERA_SYSTEM_H_
