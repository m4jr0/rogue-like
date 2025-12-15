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
# Main CMake file
#
################################################################################

cmake_minimum_required(VERSION 3.21)

project("rogue_like"
  VERSION 0.1.0
  DESCRIPTION "Minimalist Rogue Like video game built in C++20"
  LANGUAGES CXX C ASM ASM_MASM
)

# CMake setup ##################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Generic builds.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Multi-config builds.
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR})
endforeach()

# Global variables #############################################################
# Executable.
set(EXECUTABLE_NAME "rogue_like")
add_executable(${EXECUTABLE_NAME})

# Build type.
if (NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "${MESSAGE_PREFIX}Build type: ${CMAKE_BUILD_TYPE}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(RL_DEBUG)
endif()

# OS specifics #################################################################
if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

# Project ######################################################################
message(STATUS
  "${MESSAGE_PREFIX}Building ${CMAKE_PROJECT_NAME} "
  "to ${CMAKE_CURRENT_BINARY_DIR}...")

option(RL_ARE_WARNINGS_ERRORS "Treat warnings as errors" OFF)
option(RL_IS_TSAN "Enable TSan (ThreadSanitizer)" OFF)
option(RL_IS_ASAN "Enable ASan (AddressSanitizer)" OFF)
option(RL_ARE_STACK_OVERFLOW_CHECKS "Enable stack overflow checks" ON)
option(RL_IS_BUILD_PERFORMANCE_SUMMARY "Request a build performance summary (MSVC only)" OFF)

if(RL_ARE_STACK_OVERFLOW_CHECKS)
  add_compile_definitions(RL_CHECK_STACK_OVERFLOWS)
endif()

if(RL_IS_ASAN)
  add_compile_definitions(RL_IS_ASAN)
endif()

if(RL_IS_TSAN)
  add_compile_definitions(RL_IS_TSAN)
endif()

# Compiler specifics.
if(MSVC)
  # Disable any non-conformant code with Microsoft Visual C++.
  # As to why W4 is used: https://stackoverflow.com/questions/4001736/whats-up-with-the-thousands-of-warnings-in-standard-headers-in-msvc-wall#comment8922238_4001759
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /W4")

  if(RL_ARE_WARNINGS_ERRORS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
  endif()

  if(RL_IS_ASAN)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address")
    SET(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} /fsanitize=address")
    add_definitions(-D_DISABLE_STRING_ANNOTATION=1)
    add_definitions(-D_DISABLE_VECTOR_ANNOTATION=1)
  endif()

  if(RL_IS_TSAN)
    message(ERROR "${MESSAGE_PREFIX}Unsupported option on current compiler: RL_IS_TSAN")
  endif()
else()
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -mavx")

  if(RL_ARE_WARNINGS_ERRORS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif()

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -rdynamic")
  endif()

  if(RL_IS_ASAN)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    SET(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
  endif()

  if(RL_IS_TSAN)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
    SET(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=thread")
  endif()
endif()

if(RL_DEBUG_RELEASE)
  if(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
    add_link_options("/INCREMENTAL:NO" "/DEBUG" "/OPT:REF" "/OPT:ICF")
  else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -rdynamic -DNDEBUG")
  endif()
endif()

target_sources(${EXECUTABLE_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src/main.cc"
)

target_precompile_headers(${EXECUTABLE_NAME}
  PRIVATE 
    "${PROJECT_SOURCE_DIR}/src/precompiled.h"
)

# Subdirectories ###############################################################
add_subdirectory("${PROJECT_SOURCE_DIR}/src/engine")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/game")

message(STATUS "${MESSAGE_PREFIX}Done.")