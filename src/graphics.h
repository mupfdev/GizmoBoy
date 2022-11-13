/** @file graphics.h
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "lua.h"
#include "core.h"

int  graphics_init(void);
void graphics_update(core_t* core);
void graphics_deinit(void);
int  graphics_draw_menu(int cur_menu_index);
void register_graphics_api(core_t* core);

#endif /* GRAPHICS_H */
