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

typedef struct core
{
    lua_State *L;
    SDL_Event  event;
    SDL_bool   is_running;

} core_t;

int  core_init(core_t **core);
void core_update(core_t *core);
void core_run_cartridge(const char *file_name, core_t *core);
void core_deinit(core_t *core);

#endif /* CORE_H */
