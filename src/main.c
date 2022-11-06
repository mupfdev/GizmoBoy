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
#include "core.h"
#include "graphics.h"

int main(int argc, char* argv[])
{
    int     status = EXIT_SUCCESS;
    core_t* core   = NULL;

    if (core_init(&core) != 0)
    {
        status = EXIT_FAILURE;
        goto exit;
    }

    if (graphics_init() != 0)
    {
        status = EXIT_FAILURE;
        goto exit;
    }

    if (core_run_cartridge("squash.lua", core) != 0)
    {
        status = EXIT_FAILURE;
        goto exit;
    }

    while (SDL_TRUE == core->is_running)
    {
        core_update(core);
    }

exit:
    graphics_deinit();
    core_deinit(core);
    return status;
}
