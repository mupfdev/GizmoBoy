/** @file input.h
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"
#include "lua.h"
#include "core.h"

typedef enum button
{
    BUTTON_LEFT = 0,
    BUTTON_RIGHT,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_O,
    BUTTON_X

} button_t;

void input_get_buttons(Uint16 *state);
void input_set_buttons(Uint16 state);
void register_input_api(core_t* core);

#endif /* INPUT_H */
