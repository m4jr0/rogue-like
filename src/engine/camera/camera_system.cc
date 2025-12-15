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
#include "camera_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/camera/camera_utils.h"
#include "engine/core/vector.h"
#include "engine/render/render_system.h"

namespace rl {
CameraSystem& CameraSystem::instance() {
  static CameraSystem inst;
  return inst;
}

void CameraSystem::init() {
  RL_LOG_DEBUG("CameraSystem::init");
  constexpr auto kCamCapacity = 4;
  hCamPool_.clear();
  hCamPool_.reserve(kCamCapacity);
  cams_.clear();
  cams_.reserve(kCamCapacity);

  mainCam_ = generate({});
}

void CameraSystem::shutdown() {
  RL_LOG_DEBUG("CameraSystem::shutdown");
  hCamPool_.clear();
  cams_.clear();
  mainCam_.clear();
}

CameraHandle CameraSystem::generate(const CameraDesc& desc) {
  auto h = hCamPool_.generate();
  ensureCapacity(cams_, h.index);
  auto& c = cams_[h.index];
  c.handle = h;
  c.zoom = zoomSafe(desc.zoom);
  c.pos = desc.pos;
  viewport(&c, toCamViewport(RL_CRENDERSYS.viewport()));
  return h;
}

void CameraSystem::destroy(CameraHandle h) {
  RL_ASSERT(h != mainCam_,
            "CameraSystem::destroy: Cannot destroy main camera!");
  if (h == mainCam_) return;
  auto* c = cam(h);
  if (!c) return;
  *c = {};
  hCamPool_.destroy(h);
}

void CameraSystem::resizeAll(const Viewport& v) {
  auto camV = toCamViewport(v);

  for (auto& c : cams_) {
    if (!c.handle) continue;
    viewport(&c, camV);
  }
}

void CameraSystem::moveTo(Camera* c, const Position& p, bool centered) {
  if (!c) return;

  if (centered) {
    auto z = appliedZoom(c);
    const auto& s = c->viewport.size;
    c->pos = {p.x - (s.x * .5f) / z, p.y - (s.y * .5f) / z};
  } else {
    c->pos = p;
  }

  buildViewProj(c);
}

void CameraSystem::zoom(Camera* c, CameraZoom z, const Position* w) {
  if (!c) return;
  auto prev = zoomSafe(c->zoom);
  auto cur = zoomSafe(z);

  auto anchor = w ? *w : worldCenter(c);
  auto k = 1.0f - (prev / cur);
  c->pos.x += k * (anchor.x - c->pos.x);
  c->pos.y += k * (anchor.y - c->pos.y);
  c->zoom = cur;
  buildViewProj(c);
}

void CameraSystem::zoomDelta(Camera* c, CameraZoom delta, const Position* w) {
  zoom(c, c->zoom + delta, w);
}

Position CameraSystem::worldCenter(const Camera* c) const {
  return screenToWorld(c, {c->viewport.size.x * .5f, c->viewport.size.y * .5f});
}

Position CameraSystem::worldToScreen(const Camera* c, const Position& w) const {
  if (!c) return {};
  auto z = appliedZoom(c);
  auto hWorld = c->viewport.size.y;
  return {(w.x - c->pos.x) * z, hWorld - (w.y - c->pos.y) * z};
}

Position CameraSystem::screenToWorld(const Camera* c, const Position& s) const {
  if (!c) return {};
  auto z = appliedZoom(c);
  auto hWorld = c->viewport.size.y;
  return {c->pos.x + s.x / z, c->pos.y + (hWorld - s.y) / z};
}

void CameraSystem::viewport(Camera* c, const CameraViewport& v) {
  if (!c) return;
  const Position oldCenter = worldCenter(c);
  c->viewport = v;
  auto z = appliedZoom(c);
  c->pos.x = oldCenter.x - (c->viewport.size.x * .5f) / z;
  c->pos.y = oldCenter.y - (c->viewport.size.y * .5f) / z;
  buildViewProj(c);
}

Camera* CameraSystem::cam(CameraHandle h) {
  RL_ASSERT(h && hCamPool_.alive(h),
            "CameraSystem::cam: Invalid camera handle provided!");
  if (!h || !hCamPool_.alive(h)) return nullptr;
  return &cams_[h.index];
}

const Camera* CameraSystem::cam(CameraHandle h) const {
  RL_ASSERT(h && hCamPool_.alive(h),
            "CameraSystem::cam: Invalid camera handle provided!");
  if (!h || !hCamPool_.alive(h)) return nullptr;
  return &cams_[h.index];
}

const Camera* CameraSystem::main() const { return cam(mainCam_); }

Camera* CameraSystem::main() { return cam(mainCam_); }

void CameraSystem::buildViewProj(Camera* c) {
  auto z = appliedZoom(c);
  auto wWorld = static_cast<f32>(c->viewport.size.x) / z;
  auto hWorld = static_cast<f32>(c->viewport.size.y) / z;
  auto proj = ortho(.0f, wWorld, .0f, hWorld);
  auto view = Trs::translation(-c->pos.x, -c->pos.y);
  c->viewProj = proj * view;
}

CameraViewport CameraSystem::toCamViewport(const Viewport& v) {
  Vec2F32 vSize = {avoidNegOrZero(static_cast<f32>(v.size.x)),
                   avoidNegOrZero(static_cast<f32>(v.size.y))};
  auto refH = v.ref.y != 0 ? v.ref.y : 1.0f;

  return {
      .zoom = vSize.y / refH,
      .offset = {v.pos.x, v.pos.y},
      .size = vSize,
  };
}
}  // namespace rl