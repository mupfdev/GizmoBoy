/** @file mymath.c
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
#include "core.h"
#include "mymath.h"

// API functions.
static int max_(lua_State* L);
static int min_(lua_State* L);

void register_math_api(core_t* core)
{
    lua_pushcfunction(core->L, max_);
    lua_setglobal(core->L, "max");

    lua_pushcfunction(core->L, min_);
    lua_setglobal(core->L, "min");
}

// API functions.
static int max_(lua_State* L)
{
    lua_Integer ret;
    int         first  = luaL_checkinteger(L, 1);
    int         second = luaL_checkinteger(L, 2);

    if (first >= second)
    {
        ret = first;
    }
    else
    {
        ret = second;
    }

    lua_pushinteger(L, ret);

    return 1;
}

static int min_(lua_State* L)
{
    lua_Integer ret;
    int         first  = luaL_checkinteger(L, 1);
    int         second = luaL_checkinteger(L, 2);

    if (first < second)
    {
        ret = first;
    }
    else
    {
        ret = second;
    }

    lua_pushinteger(L, ret);

    return 1;
}
