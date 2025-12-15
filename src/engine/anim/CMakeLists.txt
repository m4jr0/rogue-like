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
# Engine Animation CMake file
#
################################################################################

# Source files #################################################################
target_sources(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src/engine/anim/anim_library.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/anim/anim_resource.h"
    "${PROJECT_SOURCE_DIR}/src/engine/anim/anim_runtime.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/anim/anim_serialize.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/anim/anim_system.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/anim/anim_type.h"
    "${PROJECT_SOURCE_DIR}/src/engine/anim/anim_utils.cc"
)

# Compiling ####################################################################
target_include_directories(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)