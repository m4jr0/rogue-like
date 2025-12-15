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
# Game Character CMake file
#
################################################################################

# Source files #################################################################
target_sources(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src/game/character/character.h"
    "${PROJECT_SOURCE_DIR}/src/game/character/character_action.cc"
    "${PROJECT_SOURCE_DIR}/src/game/character/character_anim.h"
    "${PROJECT_SOURCE_DIR}/src/game/character/character_archetype.h"
    "${PROJECT_SOURCE_DIR}/src/game/character/character_archetype_library.cc"
    "${PROJECT_SOURCE_DIR}/src/game/character/character_archetype_resource.h"
    "${PROJECT_SOURCE_DIR}/src/game/character/character_archetype_serialize.cc"
    "${PROJECT_SOURCE_DIR}/src/game/character/character_factory.cc"
    "${PROJECT_SOURCE_DIR}/src/game/character/character_fsm.cc"
    "${PROJECT_SOURCE_DIR}/src/game/character/character_step_handler.cc"
    "${PROJECT_SOURCE_DIR}/src/game/character/character_system.cc"

    "${PROJECT_SOURCE_DIR}/src/game/character/class/character_class_default_bindings.h"
    "${PROJECT_SOURCE_DIR}/src/game/character/class/character_class_generator.cc"
)

# Compiling ####################################################################
target_include_directories(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)