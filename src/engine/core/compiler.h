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

#ifndef ENGINE_CORE_COMPILER_H_
#define ENGINE_CORE_COMPILER_H_

#ifdef __clang__
#define RL_CLANG
#elif defined(__GNUC__)
#define RL_GCC
#ifdef __GNUG__
#define RL_GCC_CPP
#else
#define RL_GCC_C
#endif  // __GNUG__
#elif defined(_MSC_VER)
#define RL_MSVC
#endif  // __clang__

#ifdef __cplusplus
#define RL_CPP
#else
#define RL_C
#endif  // __cplusplus

#ifdef RL_GCC
#ifdef __x86_64__
#define RL_ARCH_X86_64
#define RL_ARCH_X86
#elif defined(__i386__)
#define RL_ARCH_X86_32
#define RL_ARCH_X86
#elif defined(__arm__)
#define RL_ARCH_ARM
#else
static_assert(false, "Unsupported architecture.");
#endif  // __x86_64__
#endif  // RL_GCC

#ifdef RL_MSVC
#ifdef _M_X64
#define RL_ARCH_X86_64
#define RL_ARCH_X86
#elif defined(_M_IX86)
#define RL_ARCH_X86_32
#define RL_ARCH_X86
#elif defined(_M_ARM)
#define RL_ARCH_ARM
#else
static_assert(false, "Unsupported architecture.");
#endif  // _M_X64
#endif  // RL_MSVC

#ifdef RL_MSVC
#define RL_FORCE_NOT_INLINE __declspec(noinline)
#else
#if __has_attribute(optnone)
#define RL_FORCE_NOT_INLINE __attribute__((optnone))
#elif __has_attribute(noinline)
#define RL_FORCE_NOT_INLINE __attribute__((noinline))
#else
#define RL_FORCE_NOT_INLINE
#endif
#endif  // RL_MSVC

#endif  // ENGINE_CORE_COMPILER_H_