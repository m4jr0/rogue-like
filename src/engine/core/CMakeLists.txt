# Copyright 2025 m4jr0. All Rights Reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

################################################################################
#
# Engine Core CMake file
#
################################################################################

# Source files #################################################################
target_sources(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src/engine/core/color.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/compiler.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/core_message.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/debug.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/engine.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/core/frame.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/fsm.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/handle.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/hash.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/log.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/param_set.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/param_traversal.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/phase_bus.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/core/random.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/serialize.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/spatial_ref.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/string.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/type.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/type_trait.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/value_utils.h"
    "${PROJECT_SOURCE_DIR}/src/engine/core/vector.h"
)

# Compiling ####################################################################
target_include_directories(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)