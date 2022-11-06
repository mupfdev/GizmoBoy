/** @file mymath.h
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#ifndef MYMATH_H
#define MYMATH_H

#include "lua.h"
#include "core.h"

void register_math_api(core_t* core);

#endif /* MYMATH_H */
