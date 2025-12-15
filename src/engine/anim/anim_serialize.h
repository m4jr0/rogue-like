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

#ifndef ENGINE_ANIM_ANIM_IO_H_
#define ENGINE_ANIM_ANIM_IO_H_

#include "engine/anim/anim_resource.h"
#include "engine/anim/anim_type.h"
#include "engine/common.h"
#include "engine/core/serialize.h"

namespace rl {
void writeAnimSample(std::ostream& os, const AnimSampleResource& s);
void readAnimSample(std::istream& is, AnimSampleResource& s);
void writeAnimSampleVector(std::ostream& os,
                           const std::vector<AnimSampleResource>& v);
void readAnimSampleVector(std::istream& is, std::vector<AnimSampleResource>& v);

static_assert(TriviallySerializable<AnimParamSet>,
              "AnimParamSet must be trivially copyable for this serializer!");

void writeParamSet(std::ostream& os, const AnimParamSet& ps);
void readParamSet(std::istream& is, AnimParamSet& ps);
void writeAnimKeyFrame(std::ostream& os, const AnimKeyFrame& kf);
void readAnimKeyFrame(std::istream& is, AnimKeyFrame& kf);
void writeAnimKeyFrameVector(std::ostream& os,
                             const std::vector<AnimKeyFrame>& v);
void readAnimKeyFrameVector(std::istream& is, std::vector<AnimKeyFrame>& v);
void writeAnimResource(std::ostream& os, const AnimResource& a);
void readAnimResource(std::istream& is, AnimResource& a);
void writeAnimSolvedEntry(std::ostream& os, const AnimSolvedEntry& e);
void readAnimSolvedEntry(std::istream& is, AnimSolvedEntry& e);
void writeAnimSolvedEntryVector(std::ostream& os,
                                const std::vector<AnimSolvedEntry>& v);
void readAnimSolvedEntryVector(std::istream& is,
                               std::vector<AnimSolvedEntry>& v);
void writeAnimSetResource(std::ostream& os, const AnimSetResource& set);
void readAnimSetResource(std::istream& is, AnimSetResource& set);
void writeAnimSetFile(std::ostream& os, const AnimSetResource& set);
bool readAnimSetFile(std::istream& is, AnimSetResource& set, u32& outVersion);
}  // namespace rl

#endif  // ENGINE_ANIM_ANIM_IO_H_
