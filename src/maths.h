/** @file maths.h
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#ifndef MATHS_H
#define MATHS_H

#include "lua.h"
#include "core.h"

void math_init(void);
void register_math_api(core_t* core);

#endif /* MATHS_H */
