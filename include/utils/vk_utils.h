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
// utilities for using the Vulkan API.

#ifndef ANONYMOUS_GAME_ENGINE_UTILS_VK_UTILS_H
#define ANONYMOUS_GAME_ENGINE_UTILS_VK_UTILS_H

#include <stdbool.h>

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cuti.h>


// general
// ---------------------------------------------------------------------------------------------- //
void vk_get_sup_exts(arrayable_t* sup_exts_p);
bool vk_exts_supported(arrayable_t exts);
void vk_get_req_exts(arrayable_t* req_exts_p);

// physical device utils
// ---------------------------------------------------------------------------------------------- //
void vk_pdev_get_sup_exts(VkPhysicalDevice pdev, arrayable_t* sup_exts_p);
bool vk_pdev_exts_supported(VkPhysicalDevice pdev, arrayable_t exts);
void vk_pdev_get_qfam_props(VkPhysicalDevice pdev, arrayable_t* qfam_props_p);
bool vk_pdev_has_qfam(VkPhysicalDevice pdev, VkQueueFamilyProperties props);


// function descriptions
// ---------------------------------------------------------------------------------------------- //

// description
//  |> retrieves the extensions supported by the current Vulkan API
// parameters
//  |> [sup_exts_p]: a pointer to an array that will be filled with the supported extensions
//  |   |> [.bytes]: the location of the array of extensions (of type [VkExtensionProperties])
//  |   |> [.elem_size]: will be equal to [sizeof(VkExtensionProperties)]
//  |   |> [.len]: the number of extensions in the array
// notes
//  |> if anything failed or an error occurred, [xerr()] will be called and [sup_exts_p] will be
//  |   an empty array with [sup_exts_p->bytes] being set to [NULL] to signal failure
void vk_get_sup_exts(arrayable_t* sup_exts_p);

// description
//  |> checks if the extensions specified are supported by the current Vulkan API
// parameters
//  |> [exts]: an array that contains the extensions to be checked
//  |   |> [.bytes]: the location of the array of extensions, which may be of type
//  |   |   [VkExtensionProperties] or [str_t]
//  |   |> [.elem_size]: must be equal to the size of the elements in [.bytes] (either
//  |   |   [sizeof(VkExtensionProperties)] or [sizeof(str_t)])
//  |   |> [.len]: the number of extensions in the array
// return
//  |> [true] if all the extensions in [exts] are supported or [false] otherwise
// notes
//  |> if anything failed or an error occurred, [xerr()] will be called and [false] will be returned
//  |> currently the only way to know if an error occurred is to use [xerr] functions to check for
//  |   errors, but future implementations may use the fields in [exts] to indicate an error also
bool vk_exts_supported(arrayable_t exts);

// description
//  |> retrieves the extensions required by Anonymous Game Engine
// parameters
//  |> [req_exts_p]: a pointer to an array that will be filled with the required extensions
//  |   |> [.bytes]: the location of the array of extensions (of type [VkExtensionProperties])
//  |   |> [.elem_size]: will be equal to [sizeof(VkExtensionProperties)]
//  |   |> [.len]: the number of extensions in the array
// notes
//  |> if anything failed or an error occurred, [xerr()] will be called and [req_exts_p] will be
//  |   an empty array with [req_exts_p->bytes] being set to [NULL] to signal failure
void vk_get_req_exts(arrayable_t* req_exts_p);

// description
//  |> retrieves the extensions supported by the specified Vulkan physical device
// parameters
//  |> [pdev]: the Vulkan physical device to get the supported extensions from
//  |> [sup_exts_p]: a pointer to an array that will be filled with the supported extensions
//  |   |> [.bytes]: the location of the array of extensions (of type [VkExtensionProperties])
//  |   |> [.elem_size]: will be equal to [sizeof(VkExtensionProperties)]
//  |   |> [.len]: the number of extensions in the array
// notes
//  |> if anything failed or an error occurred, [xerr()] will be called and [sup_exts_p] will be
//  |   an empty array with [sup_exts_p->bytes] being set to [NULL] to signal failure
void vk_pdev_get_sup_exts(VkPhysicalDevice pdev, arrayable_t* sup_exts_p);

// description
//  |> checks if the extensions specified are supported by the specified Vulkan physical device
// parameters
//  |> [pdev]: the Vulkan physical device to check the extensions against
//  |> [exts]: an array that contains the extensions to be checked
//  |   |> [.bytes]: the location of the array of extensions, which may be of type
//  |   |   [VkExtensionProperties] or [str_t]
//  |   |> [.elem_size]: must be equal to the size of the elements in [.bytes] (either
//  |   |   [sizeof(VkExtensionProperties)] or [sizeof(str_t)])
//  |   |> [.len]: the number of extensions in the array
// return
//  |> [true] if all the extensions in [exts] are supported or [false] otherwise
// notes
//  |> if anything failed or an error occurred, [xerr()] will be called and [false] will be returned
//  |> currently the only way to know if an error occurred is to use [xerr] functions to check for
//  |   errors, but future implementations may use the fields in [exts] to indicate an error also
bool vk_pdev_exts_supported(VkPhysicalDevice pdev, arrayable_t exts);

// description
//  |> retrieves the queue family properties supported by the specified Vulkan physical device
// parameters
//  |> [pdev]: the Vulkan physical device to get the supported properties from
//  |> [qfam_props_p]: a pointer to an array that will be filled with the supported properties
//  |   |> [.bytes]: the location of the array of properties (of type [VkQueueFamilyProperties])
//  |   |> [.elem_size]: will be equal to [sizeof(VkQueueFamilyProperties)]
//  |   |> [.len]: the number of properties in the array
// notes
//  |> if anything failed or an error occurred, [xerr()] will be called and [qfam_props_p] will be
//  |   an empty array with [qfam_props_p->bytes] being set to [NULL] to signal failure
void vk_pdev_get_qfam_props(VkPhysicalDevice pdev, arrayable_t* qfam_props_p);

// description
//  |> checks if queue family properties are supported by the specified Vulkan physical device
// parameters
//  |> [pdev]: the Vulkan physical device to check the properties against
//  |> [props]: the queue family properties to check for
//  |   |> if any field is left [0], it will be ignored when searching through supported properties
// return
//  |> [true] if [props] is supported or [false] otherwise
bool vk_pdev_has_qfam(VkPhysicalDevice pdev, VkQueueFamilyProperties props);

#endif // ANONYMOUS_GAME_ENGINE_UTILS_VK_UTILS_H
