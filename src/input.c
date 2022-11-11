/** @file input.c
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
#include "input.h"

static Uint16 input_state = 0;

// API functions.
static int btn(lua_State* L);

void input_get_buttons(Uint16 *state)
{
    *state = input_state;
}

void input_set_buttons(Uint16 state)
{
    input_state = state;
}

void register_input_api(core_t* core)
{
    lua_pushcfunction(core->L, btn);
    lua_setglobal(core->L, "btn");
}

// API functions.
static int btn(lua_State* L)
{
    int argc = lua_gettop(L);
    int ret  = input_state;
    int i    = (int)luaL_checkinteger(L, 1);

    if (i < 0)
    {
        i = 0;
    }
    else if (i > 5)
    {
        i = 5;
    }

    if (argc > 0)
    {
        if (1 == ((input_state >> i) & 1u))
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }
        lua_pushboolean(L, ret);
    }
    else
    {
        ret = input_state;
        lua_pushinteger(L, ret);
    }

    return 1;
}
