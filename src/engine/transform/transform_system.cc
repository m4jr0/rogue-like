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
#include "transform_system.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/core/value_utils.h"
#include "engine/core/vector.h"
#include "engine/transform/transform_utils.h"

namespace rl {
TransformSystem& TransformSystem::instance() {
  static TransformSystem inst;
  return inst;
}

void TransformSystem::init() {
  RL_LOG_DEBUG("TransformSystem::init");
  static constexpr usize kDefaultTransCap{256};
  hTransPool_.clear();
  locals_.clear();
  globals_.clear();
  hierarchy_.clear();
  roots_.clear();
  subtreeStack_.clear();
  transformStack_.clear();

  hTransPool_.reserve(kDefaultTransCap);
  locals_.reserve(kDefaultTransCap);
  globals_.reserve(kDefaultTransCap);
  hierarchy_.reserve(kDefaultTransCap);
  roots_.reserve(kDefaultTransCap);
  subtreeStack_.reserve(kDefaultTransCap);
  transformStack_.reserve(kDefaultTransCap);
}

void TransformSystem::shutdown() {
  RL_LOG_DEBUG("TransformSystem::shutdown");
  hTransPool_.clear();
  locals_.clear();
  globals_.clear();
  hierarchy_.clear();
  roots_.clear();
  subtreeStack_.clear();
  transformStack_.clear();
}

void TransformSystem::tick(FramePacket&) {
  transformStack_.clear();

  for (const auto& root : roots_) {
    if (!hTransPool_.alive(root)) continue;

    transformStack_.push_back({
        .handle = root,
        .parentDirty = false,
    });
  }

  while (!transformStack_.empty()) {
    auto item = transformStack_.back();
    transformStack_.pop_back();

    auto h = item.handle;
    if (!h || !hTransPool_.alive(h)) continue;
    auto parentDirty = item.parentDirty;

    auto* l = local(h);
    auto* g = global(h);
    auto* hi = hierarchy(h);
    auto dirty = l->dirty || parentDirty;

    if (dirty) {
      auto isRoot = !hi->parent;
      auto parentOk = hi->parent && hTransPool_.alive(hi->parent);

      if (isRoot || !parentOk) {
        RL_ASSERT(
            isRoot || parentOk,
            "TransformSystem::tick: Malformed transform hierarchy detected!");
        makeGlobalAsRoot(*l, *g);
      } else {
        const auto* gp = global(hi->parent);
        makeGlobalFromParent(*l, *gp, *g);
      }

      l->dirty = false;
    }

    for (auto child = hi->firstChild; child;
         child = hierarchy(child)->nextSibling) {
      transformStack_.push_back({child, dirty});
    }
  }
}

TransformHandle TransformSystem::generate(TransformDesc desc) {
  auto h = hTransPool_.generate();
  ensureCapacity(locals_, h.index);
  ensureCapacity(globals_, h.index);
  ensureCapacity(hierarchy_, h.index);

  auto& l = locals_[h.index] = {
      .dirty = false,  // Global is computed just below.
      .handle = h,
      .pos = desc.pos,
      .rot = desc.rot,
      .scale = desc.scale,
  };

  auto& g = globals_[h.index];
  g.handle = h;
  makeGlobalAsRoot(l, g);

  hierarchy_[h.index] = {
      .parent = {},
      .firstChild = {},
      .nextSibling = {},
      .prevSibling = {},
  };

  roots_.push_back(h);

  return h;
}

void TransformSystem::destroy(TransformHandle h) {
  if (!h || !hTransPool_.alive(h)) return;

  auto idx = h.index;
  auto* hi = hierarchy(h);
  auto child = hi->firstChild;

  while (child) {
    auto next = hierarchy(child)->nextSibling;
    detachFromParent(child);
    addToRoots(child);
    child = next;
  }

  detachFromParent(h);

  locals_[idx] = {};
  globals_[idx] = {};
  hierarchy_[idx] = {};
  hTransPool_.destroy(h);
}

void TransformSystem::destroySubtree(TransformHandle root) {
  if (!root || !hTransPool_.alive(root)) return;
  detachFromParent(root);
  removeFromRoots(root);

  subtreeStack_.clear();
  subtreeStack_.push_back(root);

  while (!subtreeStack_.empty()) {
    auto h = subtreeStack_.back();
    subtreeStack_.pop_back();
    RL_ASSERT(h && hTransPool_.alive(h),
              "TransformSystem::destroySubtree: Malformed transform hierarchy "
              "detected!");

    auto* hi = hierarchy(h);

    for (auto child = hi->firstChild; child;
         child = hierarchy(child)->nextSibling) {
      subtreeStack_.push_back(child);
    }

    auto idx = h.index;
    locals_[idx] = {};
    globals_[idx] = {};
    hierarchy_[idx] = {};
    hTransPool_.destroy(h);
  }
}

void TransformSystem::onParent(TransformHandle child, TransformHandle parent,
                               bool keepGlobal) {
  if (!child) return;
  RL_ASSERT(child != parent,
            "TransformSystem::onParent: Transform cannot be parent of itself!");

#ifdef RL_DEBUG
  for (auto p = parent; p; p = hierarchy(p)->parent) {
    RL_ASSERT(
        p != child,
        "TransformSystem::onParent: Transform parenting would create a cycle!");
  }
#endif  // RL_DEBUG

  auto* hc = hierarchy(child);
  if (hc->parent == parent) return;

  Trs globalChildTrs{};

  if (keepGlobal) {
    const auto* gChild = global(child);
    RL_ASSERT(gChild,
              "TransformSystem::onParent: child must have a valid global!");
    globalChildTrs = gChild->trs;
  }

  if (hc->parent) {
    detachFromParent(child);
  } else {
    removeFromRoots(child);
  }

  if (!parent) {
    addToRoots(child);
  } else {
    RL_ASSERT(hTransPool_.alive(parent),
              "TransformSystem::onParent: parent handle is not alive");
    auto* hp = hierarchy(parent);

    hc->parent = parent;
    hc->prevSibling = {};
    hc->nextSibling = hp->firstChild;

    if (hp->firstChild) {
      hierarchy(hp->firstChild)->prevSibling = child;
    }

    hp->firstChild = child;
  }

  if (keepGlobal) {
    setLocalFromGlobal(child, parent, globalChildTrs);
  }

  markSubtreeDirty(child);
}

void TransformSystem::offParent(TransformHandle child, bool keepGlobal) {
  onParent(child, {}, keepGlobal);
}

const Transform* TransformSystem::local(TransformHandle h) const {
  RL_ASSERT(h && hTransPool_.alive(h),
            "TransformSystem::local: Invalid transform handle provided!");
  if (!h || !hTransPool_.alive(h)) return nullptr;
  return &locals_[h.index];
}

Transform* TransformSystem::local(TransformHandle h) {
  RL_ASSERT(h && hTransPool_.alive(h),
            "TransformSystem::local: Invalid transform handle provided!");
  if (!h || !hTransPool_.alive(h)) return nullptr;
  return &locals_[h.index];
}

const GlobalTransform* TransformSystem::global(TransformHandle h) const {
  RL_ASSERT(h && hTransPool_.alive(h),
            "TransformSystem::global: Invalid transform handle provided!");
  if (!h || !hTransPool_.alive(h)) return nullptr;
  return &globals_[h.index];
}

void TransformSystem::translation(TransformHandle h, const Position& t) {
  if (auto* l = local(h)) {
    l->pos = t;
    l->dirty = true;
  }
}

void TransformSystem::rotation(TransformHandle h, Rotation r) {
  if (auto* l = local(h)) {
    l->rot = r;
    l->dirty = true;
  }
}

void TransformSystem::scaling(TransformHandle h, const Scale& s) {
  if (auto* l = local(h)) {
    l->scale = s;
    l->dirty = true;
  }
}

void TransformSystem::scaling(TransformHandle h, f32 s) { scaling(h, {s, s}); }

void TransformSystem::translate(TransformHandle h, const Position& t) {
  if (auto* l = local(h)) {
    l->pos += t;
    l->dirty = true;
  }
}

void TransformSystem::rotate(TransformHandle h, Rotation r) {
  if (auto* l = local(h)) {
    l->rot += r;
    l->dirty = true;
  }
}

void TransformSystem::scale(TransformHandle h, const Scale& s) {
  if (auto* l = local(h)) {
    l->scale += s;
    l->dirty = true;
  }
}

void TransformSystem::scale(TransformHandle h, f32 s) { scale(h, {s, s}); }

GlobalTransform* TransformSystem::global(TransformHandle h) {
  RL_ASSERT(h && hTransPool_.alive(h),
            "TransformSystem::global: Invalid transform handle provided!");
  if (!h || !hTransPool_.alive(h)) return nullptr;
  return &globals_[h.index];
}

void TransformSystem::addToRoots(TransformHandle h) {
#ifdef RL_DEBUG
  for (auto r : roots_) {
    RL_ASSERT(r != h, "TransformSystem::addToRoots: Transform ", h,
              "is already root!");
  }
#endif  // RL_DEBUG
  roots_.push_back(h);
}

void TransformSystem::removeFromRoots(TransformHandle h) {
  if (roots_.empty()) return;

  for (usize i = 0; i < roots_.size(); ++i) {
    if (roots_[i] == h) {
      roots_[i] = roots_.back();
      roots_.pop_back();
      return;
    }
  }
}

void TransformSystem::detachFromParent(TransformHandle child) {
  if (!child) return;
  RL_ASSERT(hTransPool_.alive(child),
            "TransformSystem::detachFromParent: Invalid child handle!");

  auto* hc = hierarchy(child);
  auto parent = hc->parent;

  if (!parent) {
    return;
  }

  RL_ASSERT(hTransPool_.alive(parent),
            "TransformSystem::detachFromParent: Invalid parent handle!");
  auto* hp = hierarchy(parent);

  if (hp->firstChild == child) {
    hp->firstChild = hc->nextSibling;
  }

  if (hc->prevSibling) {
    hierarchy(hc->prevSibling)->nextSibling = hc->nextSibling;
  }

  if (hc->nextSibling) {
    hierarchy(hc->nextSibling)->prevSibling = hc->prevSibling;
  }

  hc->parent = {};
  hc->prevSibling = {};
  hc->nextSibling = {};
}

void TransformSystem::markSubtreeDirty(TransformHandle root) {
  if (!root || !hTransPool_.alive(root)) return;
  subtreeStack_.clear();
  subtreeStack_.push_back(root);

  while (!subtreeStack_.empty()) {
    auto h = subtreeStack_.back();
    subtreeStack_.pop_back();
    if (!h || !hTransPool_.alive(h)) continue;

    local(h)->dirty = true;
    auto* hi = hierarchy(h);

    for (auto child = hi->firstChild; child;
         child = hierarchy(child)->nextSibling) {
      subtreeStack_.push_back(child);
    }
  }
}

void TransformSystem::setLocalFromGlobal(TransformHandle child,
                                         TransformHandle parent,
                                         const Trs& gChildTrs) {
  auto gParentTrs = Trs::identity();

  if (parent && hTransPool_.alive(parent)) {
    const auto* gParent = global(parent);
    RL_ASSERT(
        gParent,
        "TransformSystem::setLocalFromGlobal: parent does not have a global "
        "transform!");
    gParentTrs = gParent->trs;
  }

  RL_ASSERT(hTransPool_.alive(child),
            "TransformSystem::setLocalFromGlobal: Invalid child handle!");
  makeLocalFromGlobal(gParentTrs, gChildTrs, *local(child));
}

TransformHierarchy* TransformSystem::hierarchy(TransformHandle h) {
  RL_ASSERT(h && hTransPool_.alive(h),
            "TransformSystem::hierarchy: Invalid transform handle provided!");
  if (!h || !hTransPool_.alive(h)) return nullptr;
  return &hierarchy_[h.index];
}

const TransformHierarchy* TransformSystem::hierarchy(TransformHandle h) const {
  RL_ASSERT(h && hTransPool_.alive(h),
            "TransformSystem::hierarchy: Invalid transform handle provided!");
  if (!h || !hTransPool_.alive(h)) return nullptr;
  return &hierarchy_[h.index];
}
}  // namespace rl