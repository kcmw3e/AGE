// license
// ---------------------------------------------------------------------------------------------- //
// Copyright (c) 2022, Casey Walker
// All rights reserved.
// 
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
//
//
// vk_utils.h
// ---------------------------------------------------------------------------------------------- //
// A game engine using the Vulkan API.

#ifndef ANONYMOUS_GAME_ENGINE_UTILS_VK_UTILS_H
#define ANONYMOUS_GAME_ENGINE_UTILS_VK_UTILS_H

#include <stdbool.h>

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cuti.h>

// core initialization/termination
// ---------------------------------------------------------------------------------------------- //
void vk_init();
void vk_term();
bool is_vk_init();


// general
// ---------------------------------------------------------------------------------------------- //
bool vk_exts_supported(str_t* exts_a, uint32_t exts_len, size_t n);


// function descriptions
// ---------------------------------------------------------------------------------------------- //

// description
//  |> initializes the core Vulkan setup for AGE
// notes
//  |> may be called more than once, will immediately return if already initialized
//  |> [is_vk_init()] may be used to check if successful
//  |> if failure, will call [xerr()] and set errors
void vk_init();

// description
//  |> terminates the core Vulkan setup for AGE and frees any memory allocated in [vk_init()]
// notes
//  |> may be called more than once, will immediately return if already terminated or [vk_init()]
//  |   hasn't been called yet
//  |> after this function is called, [is_vk_init()] will return [false] until reinitialized
void vk_term();

// description
//  |> checks whether Vulkan core has been initialized yet
// return
//  |> [true] if [vk_init()] has been successfully called or [false] otherwise
// notes
//  |> may be called any time, even if [vk_init()] hasn't been called yet
bool is_vk_init();

// description
//  |> checks if Vulkan extensions are supported
// parameters
//  |> [exts_a]: an array of strings naming extensions to check for
//  |> [exts_len]: the length of [exts_a]
//  |> [n]: the minimum length of all strings in [exts_a], including any nul-terminating character
// return
//  |> [true] all extensions in [exts_a] are supported and [false] if any are not or an error
//  |   occurred
// notes
//  |> may fail and return [false], in which case xerr() will be called to set errors
bool vk_exts_supported(str_t* exts_a, uint32_t exts_len, size_t n);

#endif // ANONYMOUS_GAME_ENGINE_UTILS_VK_UTILS_H