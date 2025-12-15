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
# Game CMake file
#
################################################################################

# Subdirectories ###############################################################

target_sources(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src/game/action_scope.h"
    "${PROJECT_SOURCE_DIR}/src/game/demo.cc"
    "${PROJECT_SOURCE_DIR}/src/game/game.cc"
)

add_subdirectory("${PROJECT_SOURCE_DIR}/src/game/ability")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game/anim")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game/camera")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game/character")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game/combat")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game/physics")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game/render")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game/resource")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game/world")