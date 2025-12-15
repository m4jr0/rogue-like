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
# Engine Engine CMake file
#
################################################################################

# Subdirectories ###############################################################

target_sources(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src/engine/common.h"
)

add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/anim")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/camera")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/core")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/event")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/input")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/math")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/physics")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/player")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/render")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/resource")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/scene")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/sound")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/sprite")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/texture")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/transform")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine/time")