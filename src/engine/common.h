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

#ifndef ENGINE_COMMON_H_
#define ENGINE_COMMON_H_

// STL. ////////////////////////////////////////////////////////////////////////

#define NOMINMAX

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cfloat>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <queue>
#include <span>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

// External. ///////////////////////////////////////////////////////////////////

#include "glad/glad.h"

// GLFW should be defined after.
#include "GLFW/glfw3.h"

////////////////////////////////////////////////////////////////////////////////

// Game. ///////////////////////////////////////////////////////////////////////

// Order matters.
// 1.
#include "engine/core/compiler.h"
// 2.
#include "engine/core/type.h"
// 3.
#include "engine/core/debug.h"
// 4.
#include "engine/core/log.h"

////////////////////////////////////////////////////////////////////////////////

#endif  // ENGINE_COMMON_H_