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
#include "collider_serialize.h"
////////////////////////////////////////////////////////////////////////////////

namespace rl {
void writeCollider(std::ostream& os, const Collider& c) {
  auto shapeVal = static_cast<u8>(c.shape);
  writePod(os, shapeVal);

  switch (c.shape) {
    case ColliderShape::Aabb: {
      const auto& aabb = c.bounds.aabb;
      writePod(os, aabb.center.x);
      writePod(os, aabb.center.y);
      writePod(os, aabb.halfExtents.x);
      writePod(os, aabb.halfExtents.y);
      break;
    }

    case ColliderShape::Circle: {
      const auto& circle = c.bounds.circle;
      writePod(os, circle.center.x);
      writePod(os, circle.center.y);
      writePod(os, circle.radius);
      break;
    }

    default: {
      auto zero = 0.0f;
      writePod(os, zero);
      writePod(os, zero);
      writePod(os, zero);
      writePod(os, zero);
      break;
    }
  }
}

void readCollider(std::istream& is, Collider& c) {
  u8 shapeVal;
  readPod(is, shapeVal);
  c.shape = static_cast<ColliderShape>(shapeVal);

  switch (c.shape) {
    case ColliderShape::Aabb: {
      auto& aabb = c.bounds.aabb;
      readPod(is, aabb.center.x);
      readPod(is, aabb.center.y);
      readPod(is, aabb.halfExtents.x);
      readPod(is, aabb.halfExtents.y);
      break;
    }

    case ColliderShape::Circle: {
      auto& circle = c.bounds.circle;
      readPod(is, circle.center.x);
      readPod(is, circle.center.y);
      readPod(is, circle.radius);
      break;
    }

    default: {
      f32 dummy;
      readPod(is, dummy);
      readPod(is, dummy);
      readPod(is, dummy);
      readPod(is, dummy);
      break;
    }
  }
}

void writeColliderSample(std::ostream& os, const AnimColliderSample& s) {
  writeCollider(os, s.collider);
}

void readColliderSample(std::istream& is, AnimColliderSample& s) {
  readCollider(is, s.collider);
}

void writeColliderTrack(std::ostream& os, const AnimColliderTrack& track) {
  auto count = track.samples.size();
  writePod(os, count);

  for (const auto& sample : track.samples) {
    writeColliderSample(os, sample);
  }
}

void readColliderTrack(std::istream& is, AnimColliderTrack& track) {
  usize count;
  readPod(is, count);

  track.samples.clear();
  track.samples.reserve(count);

  for (usize i = 0; i < count; ++i) {
    AnimColliderSample s;
    readColliderSample(is, s);
    track.samples.push_back(std::move(s));
  }
}

void writeColliderSet(std::ostream& os, const AnimColliderSet& set) {
  auto hurtCount = set.hurts.size();
  writePod(os, hurtCount);

  for (const auto& track : set.hurts) {
    writeColliderTrack(os, track);
  }

  auto hitCount = set.hits.size();
  writePod(os, hitCount);

  for (const auto& track : set.hits) {
    writeColliderTrack(os, track);
  }
}

void readColliderSet(std::istream& is, AnimColliderSet& set) {
  usize hurtCount;
  readPod(is, hurtCount);

  set.hurts.clear();
  set.hurts.reserve(hurtCount);

  for (usize i = 0; i < hurtCount; ++i) {
    AnimColliderTrack t;
    readColliderTrack(is, t);
    set.hurts.push_back(std::move(t));
  }

  usize hitCount;
  readPod(is, hitCount);

  set.hits.clear();
  set.hits.reserve(hitCount);

  for (usize i = 0; i < hitCount; ++i) {
    AnimColliderTrack t;
    readColliderTrack(is, t);
    set.hits.push_back(std::move(t));
  }
}

void writeColliderProfileResource(std::ostream& os,
                                  const AnimColliderProfileResource& c) {
  writePod(os, c.id.id);

  auto animCount = c.perAnim.size();
  writePod(os, animCount);

  for (const auto& set : c.perAnim) {
    writeColliderSet(os, set);
  }
}

void readColliderProfileResource(std::istream& is,
                                 AnimColliderProfileResource& c) {
  readPod(is, c.id);

  usize animCount;
  readPod(is, animCount);

  c.perAnim.clear();
  c.perAnim.reserve(animCount);

  for (usize i = 0; i < animCount; ++i) {
    AnimColliderSet set;
    readColliderSet(is, set);
    c.perAnim.push_back(std::move(set));
  }
}

}  // namespace rl