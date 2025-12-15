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

#ifndef ENGINE_CORE_SERIALIZE_H_
#define ENGINE_CORE_SERIALIZE_H_

#include "engine/common.h"

namespace rl {
template <typename T>
concept TriviallySerializable = std::is_trivially_copyable_v<T>;

template <TriviallySerializable T>
void writePod(std::ostream& os, const T& value) {
  os.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

template <TriviallySerializable T>
void readPod(std::istream& is, T& value) {
  is.read(reinterpret_cast<char*>(&value), sizeof(T));
}

inline void writeStr(std::ostream& os, std::string_view str) {
  auto size = str.size();
  writePod(os, size);
  os.write(str.data(), size);
}

inline void readStr(std::istream& is, std::string& str) {
  usize size{};
  readPod(is, size);
  str.resize(size);

  if (size > 0) {
    is.read(str.data(), size);
  }
}

template <TriviallySerializable T>
void writeVector(std::ostream& os, const std::vector<T>& vec) {
  auto size = vec.size();
  writePod(os, size);

  if (size > 0) {
    os.write(reinterpret_cast<const char*>(vec.data()),
             static_cast<std::streamsize>(size * sizeof(T)));
  }
}

template <TriviallySerializable T>
void readVector(std::istream& is, std::vector<T>& vec) {
  usize size{};
  readPod(is, size);
  vec.resize(size);

  if (size > 0) {
    is.read(reinterpret_cast<char*>(vec.data()),
            static_cast<std::streamsize>(size * sizeof(T)));
  }
}

template <typename T, typename WriteFn>
inline void writeVectorWith(std::ostream& os, const std::vector<T>& vec,
                            WriteFn writeOne) {
  usize size = vec.size();
  writePod(os, size);

  for (const auto& elem : vec) {
    writeOne(os, elem);
  }
}

template <typename T, typename ReadFn>
inline void readVectorWith(std::istream& is, std::vector<T>& vec,
                           ReadFn readOne) {
  usize size;
  readPod(is, size);
  vec.resize(static_cast<std::size_t>(size));

  for (auto& elem : vec) {
    readOne(is, elem);
  }
}
}  // namespace rl

#endif  // ENGINE_CORE_SERIALIZE_H_
