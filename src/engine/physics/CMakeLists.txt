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
# Engine Physics CMake file
#
################################################################################

# Source files #################################################################
target_sources(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src/engine/physics/anim_collider.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/anim_collider_library.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/anim_collider_sync.h"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/anim_collider_sync_system.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/collider.h"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/collider_resource.h"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/collider_serialize.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/grid.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/hitbox.h"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/hitbox_system.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/physics.h"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/physics_body.h"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/physics_body_serialize.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/physics_system.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/physics/physics_utils.cc"
)

# Compiling ####################################################################
target_include_directories(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)