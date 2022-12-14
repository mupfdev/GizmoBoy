/** @file config.h
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#ifndef CONFIG_H
#define CONFIG_H

#include "SDL.h"

#if defined (__NGAGE__)
#include "config_ngage.h"
#else
#define WINDOW_WIDTH   512
#define WINDOW_HEIGHT  512
#define WINDOW_FLAGS   0
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
#define VIEWPORT_FRAME "frame-256x256.png"
#define VIEWPORT_X     128
#define VIEWPORT_Y     88
#define VIEWPORT_W     256
#define VIEWPORT_H     256

#ifdef __EMSCRIPTEN__
#define APP_PATH       "./assets/"
#else
#define APP_PATH       "./"
#endif
#define PATH_SEP       "/"

#define KEYCODE_UP     SDLK_UP
#define KEYCODE_DOWN   SDLK_DOWN
#define KEYCODE_LEFT   SDLK_LEFT
#define KEYCODE_RIGHT  SDLK_RIGHT
#define KEYCODE_O      SDLK_z
#define KEYCODE_X      SDLK_x
#define KEYCODE_QUIT   SDLK_ESCAPE
#endif

#endif /* CONFIG_H */
