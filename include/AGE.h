// license
// ---------------------------------------------------------------------------------------------- //
// Copyright (c) 2022, Casey Walker
// All rights reserved.
// 
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
//
//
// AGE.h
// ---------------------------------------------------------------------------------------------- //
// A game engine using the Vulkan API.

#ifndef ANONYMOUS_GAME_ENGINE_H
#define ANONYMOUS_GAME_ENGINE_H

#include <stdbool.h>

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cuti.h>

#include "core/AGE_instance.h"
#include "core/AGE_window.h"

#define AGE_NAME "Anonymous Game Engine"
#define AGE_VK_VERSION VK_MAKE_VERSION(0, 0, 0)
#define AGE_VK_API_VERSION VK_API_VERSION_1_0

void AGE_init();
void AGE_term();
bool is_AGE_init();


// function descriptions
// ---------------------------------------------------------------------------------------------- //

// description
//  |> initialize Anonymous Game Engine library. Must be called before any other library functions
//  |   are called, unless otherwise stated for a specific function.
void AGE_init();

// description
//  |> terminate Anonymous Game Engine library. Must be called before program exit.
void AGE_term();

// description
//  |> returns whether AGE_init() has been successfully called.
// return
//  |> [true] if AGE_init() was previously called and resulted in successful initialization of the
//  |   library, or [false] otherwise
// notes
//  |> this function is always safe to call, even if AGE_init() has not been called already.
bool is_AGE_init();

#endif // ANONYMOUS_GAME_ENGINE_H
