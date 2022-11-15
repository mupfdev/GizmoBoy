/** @file main.c
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#include <stdlib.h>
#include "SDL.h"
#include "SDL_main.h"
#include "config.h"
#include "core.h"
#include "graphics.h"
#include "maths.h"

int main(int argc, char* argv[])
{
    int     status = EXIT_SUCCESS;
    core_t* core   = NULL;

    math_init();

    if (graphics_init() != 0)
    {
        status = EXIT_FAILURE;
        goto exit;
    }

    if (core_init(&core) != 0)
    {
        status = EXIT_FAILURE;
        goto exit;
    }

    while (SDL_TRUE == core->is_running)
    {
        status_t core_status = CORE_OK;

        core_status = core_update(core);

        switch(core_status)
        {
            default:
            case CORE_OK:
            case CORE_WARNING:
                break;
            case CORE_QUIT:
                core->is_running = SDL_FALSE;
                break;
            case CORE_ERROR:
                core->is_running = SDL_FALSE;
                status           = EXIT_FAILURE;
        }
    }

exit:
    core_deinit(core);
    graphics_deinit();

    return status;
}
