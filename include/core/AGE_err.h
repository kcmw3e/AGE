// license
// ---------------------------------------------------------------------------------------------- //
// Copyright (c) 2022, Casey Walker
// All rights reserved.
// 
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
//
//
// AGE_err.h
// ---------------------------------------------------------------------------------------------- //
// Extension of xerr from cuti

#ifndef ANONYMOUS_GAME_ENGINE_ERR_H
#define ANONYMOUS_GAME_ENGINE_ERR_H

#include <cuti.h>

enum AGE_err {
    AGE_ERR_START = XERR_MAX_ENUM,
    AGE_ERR_VULKAN_ENUMERATION_FAILURE,
    AGE_ERR_INITIALIZATION_FAULURE,
    AGE_ERR_BAD_INITIALIZER,
    AGE_ERR_MAX_ENUM,
};

#endif // ANONYMOUS_GAME_ENGINE_H
