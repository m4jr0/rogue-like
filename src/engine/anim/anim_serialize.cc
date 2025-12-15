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
#include "anim_serialize.h"
////////////////////////////////////////////////////////////////////////////////

#include "engine/anim/anim_resource.h"
#include "engine/anim/anim_type.h"
#include "engine/resource/resource_file_serialize.h"
#include "engine/sprite/sprite_serialize.h"

namespace rl {
void writeAnimSample(std::ostream& os, const AnimSampleResource& s) {
  writeSprite(os, s.sprite);
  writePod(os, s.duration);
}

void readAnimSample(std::istream& is, AnimSampleResource& s) {
  readSprite(is, s.sprite);
  readPod(is, s.duration);
}

void writeAnimSampleVector(std::ostream& os,
                           const std::vector<AnimSampleResource>& v) {
  writeVectorWith(os, v, [](std::ostream& os2, const AnimSampleResource& s) {
    writeAnimSample(os2, s);
  });
}

void readAnimSampleVector(std::istream& is,
                          std::vector<AnimSampleResource>& v) {
  readVectorWith(is, v, [](std::istream& is2, AnimSampleResource& s) {
    readAnimSample(is2, s);
  });
}

void writeParamSet(std::ostream& os, const AnimParamSet& ps) {
  writePod(os, ps);
}

void readParamSet(std::istream& is, AnimParamSet& ps) { readPod(is, ps); }

void writeAnimKeyFrame(std::ostream& os, const AnimKeyFrame& kf) {
  writePod(os, kf.keyTag);
  writePod(os, kf.frame);

  const AnimParamSet& base = static_cast<const AnimParamSet&>(kf);
  writeParamSet(os, base);
}

void readAnimKeyFrame(std::istream& is, AnimKeyFrame& kf) {
  readPod(is, kf.keyTag);
  readPod(is, kf.frame);

  AnimParamSet& base = static_cast<AnimParamSet&>(kf);
  readParamSet(is, base);
}

void writeAnimKeyFrameVector(std::ostream& os,
                             const std::vector<AnimKeyFrame>& v) {
  writeVectorWith(os, v, [](std::ostream& os2, const AnimKeyFrame& kf) {
    writeAnimKeyFrame(os2, kf);
  });
}

void readAnimKeyFrameVector(std::istream& is, std::vector<AnimKeyFrame>& v) {
  readVectorWith(is, v, [](std::istream& is2, AnimKeyFrame& kf) {
    readAnimKeyFrame(is2, kf);
  });
}

void writeAnimResource(std::ostream& os, const AnimResource& a) {
  writePod(os, a.flags);
  writePod(os, a.duration);
  writePod(os, a.key);

  writeAnimSampleVector(os, a.samples);
  writeVector(os, a.accDurations);
  writeAnimKeyFrameVector(os, a.keys);
}

void readAnimResource(std::istream& is, AnimResource& a) {
  readPod(is, a.flags);
  readPod(is, a.duration);
  readPod(is, a.key);

  readAnimSampleVector(is, a.samples);
  readVector(is, a.accDurations);
  readAnimKeyFrameVector(is, a.keys);
}

void writeAnimSolvedEntry(std::ostream& os, const AnimSolvedEntry& e) {
  writePod(os, e.fromKey);
  writePod(os, e.toKey);
  writePod(os, e.flipPolicy);
}

void readAnimSolvedEntry(std::istream& is, AnimSolvedEntry& e) {
  readPod(is, e.fromKey);
  readPod(is, e.toKey);
  readPod(is, e.flipPolicy);
}

void writeAnimSolvedEntryVector(std::ostream& os,
                                const std::vector<AnimSolvedEntry>& v) {
  writeVectorWith(os, v, [](std::ostream& os2, const AnimSolvedEntry& e2) {
    writeAnimSolvedEntry(os2, e2);
  });
}

void readAnimSolvedEntryVector(std::istream& is,
                               std::vector<AnimSolvedEntry>& v) {
  readVectorWith(is, v, [](std::istream& is2, AnimSolvedEntry& e2) {
    readAnimSolvedEntry(is2, e2);
  });
}

void writeAnimSetResource(std::ostream& os, const AnimSetResource& set) {
  writePod(os, set.id);
  writePod(os, set.tex);
  writePod(os, set.colliderProfile.id);
  writePod(os, set.defaultAnimIdx);
  writeAnimSolvedEntryVector(os, set.solved);

  writeVectorWith(os, set.anims, [](std::ostream& os2, const AnimResource& a) {
    writeAnimResource(os2, a);
  });
}

void readAnimSetResource(std::istream& is, AnimSetResource& set) {
  readPod(is, set.id);
  readPod(is, set.tex);
  readPod(is, set.colliderProfile.id);
  readPod(is, set.defaultAnimIdx);

  readVectorWith(is, set.anims, [](std::istream& is2, AnimResource& a) {
    readAnimResource(is2, a);
  });

  readAnimSolvedEntryVector(is, set.solved);

  set.keyToIdx.clear();
  set.keyToIdx.reserve(set.anims.size());

  for (usize i = 0; i < set.anims.size(); ++i) {
    const AnimResource& a = set.anims[i];
    set.keyToIdx[a.key] = i;
  }
}

void writeAnimSetFile(std::ostream& os, const AnimSetResource& set) {
  writeResourceFileHeader(os, kResourceTypeAnimSet, AnimSetResource::kVersion);
  writeAnimSetResource(os, set);
}

bool readAnimSetFile(std::istream& is, AnimSetResource& set, u32& outVersion) {
  if (!readAndCheckResourceFileHeader(is, kResourceTypeAnimSet, outVersion)) {
    return false;
  }

  readAnimSetResource(is, set);
  return AnimSetResource::kVersion == outVersion;
}
}  // namespace rl