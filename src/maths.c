/** @file maths.c
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#include "SDL.h"
#include "lua.h"
#include "lauxlib.h"
#include "time.h"
#include "core.h"
#include "maths.h"

static unsigned xs = 1;

static unsigned xorshift(void);

// API functions.
static int rnd(lua_State* L);

void math_init(void)
{
    xs = (unsigned)time(NULL);
}

void register_math_api(core_t* core)
{
    lua_pushcfunction(core->L, rnd);
    lua_setglobal(core->L, "rnd");
}

static unsigned xorshift(void)
{
    xs ^= xs << 7;
    xs ^= xs >> 9;
    xs ^= xs << 8;

    return xs;
}

// API functions.
static int rnd(lua_State* L)
{
    unsigned ret   = xorshift() - 1;
    unsigned limit = (unsigned)luaL_checkinteger(L, 1);

    ret = (xorshift() - 1) % limit;
    lua_pushinteger(L, ret);

    return 1;
}
