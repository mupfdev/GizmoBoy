/** @file core.c
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#include "SDL.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "config.h"
#include "core.h"
#include "graphics.h"
#include "input.h"

int core_init(core_t **core)
{
    *core = (core_t*)calloc(1, sizeof(struct core));
    if (NULL == *core)
    {
        return -1;
    }

    (*core)->L = luaL_newstate();
    luaopen_string((*core)->L);

    if (NULL != (*core)->L)
    {
        register_graphics_api((*core));
        register_input_api((*core));
    }

    (*core)->is_running = SDL_TRUE;

    return 0;
}

void core_update(core_t *core)
{
    if (SDL_PollEvent(&core->event))
    {
        Uint16 button_state;

        input_get_buttons(&button_state);

        switch (core->event.type)
        {
            case SDL_QUIT:
                core->is_running = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                switch (core->event.key.keysym.sym)
                {
                    case KEYCODE_LEFT:
                        button_state |= 1ul << BUTTON_LEFT;
                        break;
                    case KEYCODE_RIGHT:
                        button_state |= 1ul << BUTTON_RIGHT;
                        break;
                    case KEYCODE_UP:
                        button_state |= 1ul << BUTTON_UP;
                        break;
                    case KEYCODE_DOWN:
                        button_state |= 1ul << BUTTON_DOWN;
                        break;
                    case KEYCODE_O:
                        button_state |= 1ul << BUTTON_O;
                        break;
                    case KEYCODE_X:
                        button_state |= 1ul << BUTTON_X;
                        break;
                    case KEYCODE_QUIT:
                        core->is_running = SDL_FALSE;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (core->event.key.keysym.sym)
                {
                    case KEYCODE_LEFT:
                        button_state &= ~(1ul << BUTTON_LEFT);
                        break;
                    case KEYCODE_RIGHT:
                        button_state &= ~(1ul << BUTTON_RIGHT);
                        break;
                    case KEYCODE_UP:
                        button_state &= ~(1ul << BUTTON_UP);
                        break;
                    case KEYCODE_DOWN:
                        button_state &= ~(1ul << BUTTON_DOWN);
                        break;
                    case KEYCODE_O:
                        button_state &= ~(1ul << BUTTON_O);
                        break;
                    case KEYCODE_X:
                        button_state &= ~(1ul << BUTTON_X);
                        break;
                    case KEYCODE_QUIT:
                        core->is_running = SDL_FALSE;
                        break;
                }
                break;
        }

        input_set_buttons(button_state);
    }

    lua_getglobal(core->L, "_update");
    if (lua_isfunction(core->L, -1))
    {
        if (LUA_OK == lua_pcall(core->L, 0, 1, 0))
        {
            lua_pop(core->L, lua_gettop(core->L));
        }
    }
}

void core_run_cartridge(const char *file_name, core_t *core)
{
    if (LUA_OK == luaL_dofile(core->L, file_name))
    {
        lua_pop(core->L, lua_gettop(core->L));
    }
    else
    {
        SDL_Log("Could not load script '%s'", file_name);
    }
}

void core_deinit(core_t *core)
{
    if (NULL == core)
    {
        return;
    }

    if (NULL != core->L)
    {
        lua_close(core->L);
    }

    core->is_running = SDL_FALSE;

    free(core);
}
