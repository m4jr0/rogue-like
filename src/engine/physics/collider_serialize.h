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

#ifndef ENGINE_PHYSICS_COLLIDER_SERIALIZE_H_
#define ENGINE_PHYSICS_COLLIDER_SERIALIZE_H_

#include "collider_resource.h"
#include "engine/common.h"
#include "engine/core/serialize.h"
#include "engine/physics/collider.h"
#include "engine/physics/collider_resource.h"

namespace rl {
void writeCollider(std::ostream& os, const Collider& c);
void readCollider(std::istream& is, Collider& c);
void writeColliderSample(std::ostream& os, const AnimColliderSample& s);
void readColliderSample(std::istream& is, AnimColliderSample& s);
void writeColliderTrack(std::ostream& os, const AnimColliderTrack& track);
void readColliderTrack(std::istream& is, AnimColliderTrack& track);
void writeColliderSet(std::ostream& os, const AnimColliderSet& set);
void readColliderSet(std::istream& is, AnimColliderSet& set);
void writeColliderProfileResource(std::ostream& os,
                                  const AnimColliderProfileResource& c);
void readColliderProfileResource(std::istream& is,
                                 AnimColliderProfileResource& c);
}  // namespace rl

#endif  // ENGINE_PHYSICS_COLLIDER_SERIALIZE_H_
