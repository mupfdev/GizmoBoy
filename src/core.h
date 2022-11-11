/** @file core.h
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#ifndef CORE_H
#define CORE_H

#include "SDL.h"
#include "lua.h"

typedef enum status
{
    CORE_OK      = 0,
    CORE_QUIT    = 1,
    CORE_WARNING = -1,
    CORE_ERROR   = -2

} status_t;

typedef struct core
{
    lua_State *L;
    SDL_Event  event;
    SDL_bool   is_running;
    SDL_bool   is_menu;
    int        cur_menu_item;
    int        menu_item_count;

} core_t;

int  core_init(core_t **core);
int  core_update(core_t *core);
void core_run_cartridge(core_t *core);
void core_deinit(core_t *core);

#endif /* CORE_H */
