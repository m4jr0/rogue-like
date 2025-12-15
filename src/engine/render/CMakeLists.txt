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
# Engine Render CMake file
#
################################################################################

# Libraries ####################################################################
# Glad.
add_library(glad "${PROJECT_SOURCE_DIR}/external/glad/src/glad.c")
target_include_directories(glad PUBLIC "${PROJECT_SOURCE_DIR}/external/glad/include")

# GLFW.
if (NOT TARGET glfw)
  add_subdirectory(
    "${PROJECT_SOURCE_DIR}/external/glfw"
    "${CMAKE_CURRENT_BINARY_DIR}/external/glfw"
  )
endif()

# OpenGL.
find_package(OpenGL REQUIRED)

# Source files #################################################################
target_sources(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src/engine/render/opengl_render_device.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/render/render_common.h"
    "${PROJECT_SOURCE_DIR}/src/engine/render/render_device.h"
    "${PROJECT_SOURCE_DIR}/src/engine/render/render_gizmo.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/render/render_queue.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/render/render_submit.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/render/render_system.cc"
    "${PROJECT_SOURCE_DIR}/src/engine/render/render_utils.cc"
)

# Compiling ####################################################################
target_include_directories(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)

target_include_directories(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/external"
)

# Linking ######################################################################
target_link_libraries(${EXECUTABLE_NAME}
  PRIVATE
    glad
    glfw
    OpenGL::GL
)