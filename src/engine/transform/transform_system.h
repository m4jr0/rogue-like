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

#ifndef ENGINE_TRANSFORM_TRANSFORM_SYSTEM_H_
#define ENGINE_TRANSFORM_TRANSFORM_SYSTEM_H_

#include "engine/common.h"
#include "engine/core/frame.h"
#include "engine/core/handle.h"
#include "engine/math/trs.h"
#include "engine/transform/transform.h"

namespace rl {
class TransformSystem {
 public:
  static TransformSystem& instance();

  void init();
  void shutdown();

  void tick(FramePacket&);

  [[nodiscard]] TransformHandle generate(TransformDesc desc);
  void destroy(TransformHandle h);
  void destroySubtree(TransformHandle root);

  void onParent(TransformHandle child, TransformHandle parent,
                bool keepGlobal = true);
  void offParent(TransformHandle child, bool keepGlobal = true);

  Transform* local(TransformHandle h);
  const Transform* local(TransformHandle h) const;

  GlobalTransform* global(TransformHandle h);
  const GlobalTransform* global(TransformHandle h) const;

  void translation(TransformHandle h, const Position& t);
  void rotation(TransformHandle h, Rotation r);
  void scaling(TransformHandle h, const Scale& s);
  void scaling(TransformHandle h, f32 s);

  void translate(TransformHandle h, const Position& t);
  void rotate(TransformHandle h, Rotation r);
  void scale(TransformHandle h, const Scale& s);
  void scale(TransformHandle h, f32 s);

 private:
  f64 lag_{.0};
  HandlePool<TransformTag> hTransPool_{};
  std::vector<Transform> locals_{};
  std::vector<GlobalTransform> globals_{};
  std::vector<TransformHierarchy> hierarchy_{};
  std::vector<TransformHandle> roots_{};
  std::vector<TransformHandle> subtreeStack_{};

  struct StackItem {
    TransformHandle handle{kInvalidHandle};
    bool parentDirty{false};
  };

  std::vector<StackItem> transformStack_;

  TransformSystem() = default;

  void addToRoots(TransformHandle h);
  void removeFromRoots(TransformHandle h);
  void detachFromParent(TransformHandle child);
  void markSubtreeDirty(TransformHandle root);
  void setLocalFromGlobal(TransformHandle child, TransformHandle parent,
                          const Trs& globalChildTrs);

  TransformHierarchy* hierarchy(TransformHandle h);
  const TransformHierarchy* hierarchy(TransformHandle h) const;
};
}  // namespace rl

#define RL_TRANSSYS (::rl::TransformSystem::instance())
#define RL_CTRANSSYS \
  (static_cast<const ::rl::TransformSystem&>(::rl::TransformSystem::instance()))

#endif  // ENGINE_TRANSFORM_TRANSFORM_SYSTEM_H_
