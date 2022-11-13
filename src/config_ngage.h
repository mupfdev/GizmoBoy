/** @file config_ngage.h
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#ifndef CONFIG_NGAGE_H
#define CONFIG_NGAGE_H

#include "SDL.h"

#define WINDOW_WIDTH   176
#define WINDOW_HEIGHT  208
#define WINDOW_FLAGS   SDL_WINDOW_FULLSCREEN
#define RENDERER_FLAGS SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE
#define VIEWPORT_FRAME "frame-176x208.png"
#define VIEWPORT_X     24
#define VIEWPORT_Y     20
#define VIEWPORT_W     128
#define VIEWPORT_H     128

#define APP_PATH       "E:\\System\\Apps\\GizmoBoy\\"
#define PATH_SEP       "\\"

#define KEYCODE_UP     SDLK_UP
#define KEYCODE_DOWN   SDLK_DOWN
#define KEYCODE_LEFT   SDLK_LEFT
#define KEYCODE_RIGHT  SDLK_RIGHT
#define KEYCODE_O      SDLK_7
#define KEYCODE_X      SDLK_5
#define KEYCODE_QUIT   SDLK_SOFTLEFT

#endif /* CONFIG_NGAGE_H */
