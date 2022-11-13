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
#include "dirent.h"
#include "config.h"
#include "core.h"
#include "graphics.h"
#include "input.h"

int core_init(core_t **core)
{
    *core = (core_t*)calloc(1, sizeof(struct core));
    if (NULL == *core)
    {
        return CORE_ERROR;
    }

    (*core)->L = luaL_newstate();
    luaopen_base((*core)->L);
    luaopen_string((*core)->L);
    luaopen_table((*core)->L);
    luaopen_math((*core)->L);

    if (NULL != (*core)->L)
    {
        register_graphics_api((*core));
        register_input_api((*core));
    }

    (*core)->is_running = SDL_TRUE;
    (*core)->is_menu    = SDL_TRUE;

    return CORE_OK;
}

int core_update(core_t *core)
{
    int status = CORE_OK;

    if (NULL == core)
    {
        return status;
    }

    if (SDL_TRUE == core->is_menu)
    {
        core->menu_item_max_index = graphics_draw_menu(core->cur_menu_index);
    }

    if (SDL_PollEvent(&core->event))
    {
        Uint16 button_state;

        input_get_buttons(&button_state);

        switch (core->event.type)
        {
            case SDL_QUIT:
                status = CORE_QUIT;
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
                        if (SDL_TRUE == core->is_menu)
                        {
                            core->cur_menu_index -= 1;
                        }
                        button_state |= 1ul << BUTTON_UP;
                        break;
                    case KEYCODE_DOWN:
                        if (SDL_TRUE == core->is_menu)
                        {
                            core->cur_menu_index += 1;
                        }
                        button_state |= 1ul << BUTTON_DOWN;
                        break;
                    case KEYCODE_O:
                        if (SDL_TRUE == core->is_menu)
                        {
                            core_run_cartridge(core);
                        }
                        button_state |= 1ul << BUTTON_O;
                        break;
                    case KEYCODE_X:
                        if (SDL_TRUE == core->is_menu)
                        {
                            core_run_cartridge(core);
                        }
                        button_state |= 1ul << BUTTON_X;
                        break;
                    case KEYCODE_QUIT:
                        if (SDL_FALSE == core->is_menu)
                        {
                            core->is_menu = SDL_TRUE;
                        }
                        else
                        {
                            status = CORE_QUIT;
                        }
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
                        break;
                }
                break;
        }

        input_set_buttons(button_state);
    }

    if (core->cur_menu_index > core->menu_item_max_index)
    {
        core->cur_menu_index = 0;
    }
    else if (core->cur_menu_index < 0)
    {
        core->cur_menu_index = core->menu_item_max_index;
    }

    if (SDL_FALSE == core->is_menu)
    {
        lua_getglobal(core->L, "_update");
        if (lua_isfunction(core->L, -1))
        {
            if (LUA_OK == lua_pcall(core->L, 0, 1, 0))
            {
                lua_pop(core->L, lua_gettop(core->L));
            }
            else
            {
                status = CORE_ERROR;
                SDL_Log("Error calling _update(): %s", lua_tostring(core->L,-1));
            }
        }
        graphics_update(core);
    }

    return status;
}

void core_run_cartridge(core_t *core)
{
    DIR *dir;

    if (NULL == core)
    {
        return;
    }

    dir = opendir(APP_PATH "carts\\");

    if (NULL != dir)
    {
        struct dirent *ent;
        int            item_index = -1;

        while (NULL != (ent = readdir (dir)))
        {
            if ((SDL_strstr(ent->d_name, ".lua") != NULL) || (SDL_strstr(ent->d_name, ".LUA") != NULL))
            {
                item_index += 1;

                if (item_index == core->cur_menu_index)
                {
                    char file_name[256] = { 0 };

                    SDL_snprintf(file_name, sizeof(file_name), "%scarts%s%s", APP_PATH, PATH_SEP, ent->d_name);

                    if (LUA_OK == luaL_dofile(core->L, file_name))
                    {
                        lua_pop(core->L, lua_gettop(core->L));
                        core->is_menu = SDL_FALSE;
                    }
                    else
                    {
                        SDL_Log("Could not load script '%s'", file_name);
                    }
                    break;
                }

                // Max carts. Needs to be fixed later.
                if (20 == item_index)
                {
                    break;
                }
            }
        }
        closedir (dir);
    }

    lua_getglobal(core->L, "_init");
    if (lua_isfunction(core->L, -1))
    {
        if (LUA_OK == lua_pcall(core->L, 0, 1, 0))
        {
            lua_pop(core->L, lua_gettop(core->L));
        }
        else
        {
            SDL_Log("Error calling _init(): %s", lua_tostring(core->L,-1));
        }
    }

    return;
}

void core_deinit(core_t *core)
{
    if (NULL == core)
    {
        return;
    }

    /* "On several platforms, you may not need to call this function,
     * because all resources are naturally released when the host
     * program ends."
     *
     * Fixes an APP CLOSED error on Symbian.
     */
#if ! defined (__NGAGE__)
    if (NULL != core->L)
    {
        lua_close(core->L);
    }
#endif

    free(core);
}
