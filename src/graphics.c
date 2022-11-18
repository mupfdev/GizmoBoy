/** @file graphics.c
 *
 *  A portable fantasy console inspired by PICO-8 from Lexaloffle Games.
 *
 *  Copyright (c) 2022, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "lua.h"
#include "lauxlib.h"
#include "dirent.h"
#include "config.h"
#include "core.h"
#include "graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct draw_state
{
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Texture  *render_target;
    SDL_Texture  *font;
    SDL_Texture  *frame;
    int           col;
    int           cur_x;
    int           cur_y;
    int           line_endpoint_x;
    int           line_endpoint_y;
    int           cur_margin;
    SDL_bool      endpoint_validity;

} draw_state_t;

static draw_state_t state = { 0 };

static void clear_screen(int col);
static void draw_circ_sub(int xc, int yc, int x, int y, int col);
static void draw_circ(int xc, int yc, int r, int col);
static void draw_frame(void);
static void draw_line(int x0, int y0, int x1, int y1, int col);
static void draw_pixel(int x, int y,  int col);
static void draw_rect(int x0, int y0, int x1, int y1, int col, SDL_bool fill);
static void draw_text(const char *str);
static void flip_screen(void);
static void get_character_position(const unsigned char character, int* pos_x, int* pos_y);
static int  load_texture_from_file(const char* file_name, SDL_Texture** texture);
static void set_col(int col, SDL_bool update_state);
static void set_line_endpoint(int x, int y, SDL_bool validity);

// API functions.
static int circ(lua_State* L);
static int cls(lua_State* L);
static int color(lua_State* L);
static int cursor(lua_State* L);
static int flip(lua_State* L);
static int line(lua_State* L);
static int print(lua_State* L);
static int pset(lua_State* L);
static int rect(lua_State* L);
static int rectfill(lua_State* L);

int graphics_init(void)
{
    int status;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
        status = -1;
        goto exit;
    }

    state.window = SDL_CreateWindow(
        "GizmoBoy",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        WINDOW_FLAGS);

    if (NULL == state.window)
    {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        status = -2;
        goto exit;
    }

    state.renderer = SDL_CreateRenderer(
        state.window,
        -1,
        RENDERER_FLAGS);

    if (NULL == state.renderer)
    {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        status = -3;
        goto exit;
    }

    state.render_target = SDL_CreateTexture(
        state.renderer,
        SDL_PIXELFORMAT_RGB444,
        SDL_TEXTUREACCESS_TARGET,
        128,
        128);

    if (NULL == state.render_target)
    {
        SDL_Log("Unable to create render target: %s", SDL_GetError());
        status = -4;
        goto exit;
    }

    status = load_texture_from_file((const char*)APP_PATH "" VIEWPORT_FRAME, &state.frame);
    if (status != 0)
    {
        goto exit;
    }
    draw_frame();

    if (SDL_SetRenderTarget(state.renderer, state.render_target) != 0)
    {
        SDL_Log("Unable to set render target: %s", SDL_GetError());
        status = -5;
        goto exit;
    }

    status = load_texture_from_file((const char*)APP_PATH "font.png", &state.font);
    if (status != 0)
    {
        goto exit;
    }

exit:
    return status;
}

void graphics_update(core_t* core)
{
    lua_getglobal(core->L, "_draw");
    if (lua_isfunction(core->L, -1))
    {
        if (LUA_OK == lua_pcall(core->L, 0, 1, 0))
        {
            lua_pop(core->L, lua_gettop(core->L));
        }
        else
        {
            SDL_Log("Error calling _draw(): %s", lua_tostring(core->L,-1));
        }
    }

    flip_screen();
}

void graphics_deinit(void)
{
    if (state.frame != NULL)
    {
        SDL_DestroyTexture(state.frame);
    }

    if (state.font != NULL)
    {
        SDL_DestroyTexture(state.font);
    }

    if (state.render_target != NULL)
    {
        SDL_DestroyTexture(state.render_target);
    }

    if (state.renderer != NULL)
    {
        SDL_DestroyRenderer(state.renderer);
    }

    if (state.window != NULL)
    {
        SDL_DestroyWindow(state.window);
    }

    SDL_Quit();
}

int graphics_draw_menu(int cur_menu_index, char *error_msg)
{
    DIR  *dir;
    char *title      = "GizmoBoy";
    int   margin_bak = state.cur_margin;
    int   x_bak      = state.cur_x;
    int   y_bak      = state.cur_y;
    int   item_index = -1;
    int   i;

    clear_screen(0);

    state.cur_margin = 0;
    state.cur_x      = 0;
    state.cur_y      = 2;

    for (i = 0; i < SDL_strlen(title); i += 1)
    {
        char draw_chr[2] = { 0 };

        SDL_snprintf(draw_chr, sizeof(draw_chr), "%c", title[i]);
        set_col(8 + i, SDL_FALSE);
        draw_text((const char*)draw_chr);
        set_col(state.col, SDL_FALSE);
    }

    state.cur_margin = 0;
    state.cur_x      = 0;
    state.cur_y      = 14;
    dir              = opendir(APP_PATH "carts" PATH_SEP);

    if (NULL != dir)
    {
        struct dirent *ent;

        while (NULL != (ent = readdir(dir)))
        {
            if ((SDL_strstr(ent->d_name, ".lua") != NULL) || (SDL_strstr(ent->d_name, ".LUA") != NULL))
            {
                item_index += 1;

                if (item_index == cur_menu_index)
                {
                    draw_rect(0, state.cur_y, 4, state.cur_y + 6, 14, SDL_TRUE);
                    state.cur_x = 6;
                }
                else
                {
                    state.cur_x = 0;
                }

                set_col(7, SDL_FALSE);
                draw_text(ent->d_name);
                set_col(state.col, SDL_FALSE);
                state.cur_y += 6;

                // Max carts. Needs to be fixed later.
                if (19 == item_index)
                {
                    break;
                }
            }
        }
        closedir (dir);
    }

    if (-1 == item_index)
    {
        clear_screen(0);

        state.cur_margin = 38;
        state.cur_x      = 38;
        state.cur_y      = 54;

        set_col(8, SDL_FALSE);
        draw_text("No carts found");
        set_col(state.col, SDL_FALSE);
    }

    if (error_msg != NULL)
    {
        set_col(8, SDL_FALSE);
        state.cur_margin = 0;
        state.cur_x      = 0;
        state.cur_y      = 120;
        draw_text(error_msg);
        set_col(state.col, SDL_FALSE);
    }

    flip_screen();

    state.cur_margin = margin_bak;
    state.cur_x      = x_bak;
    state.cur_y      = y_bak;

    return item_index;
}

void register_graphics_api(core_t* core)
{
    lua_pushcfunction(core->L, circ);
    lua_setglobal(core->L, "circ");

    lua_pushcfunction(core->L, cls);
    lua_setglobal(core->L, "cls");

    lua_pushcfunction(core->L, color);
    lua_setglobal(core->L, "color");

    lua_pushcfunction(core->L, cursor);
    lua_setglobal(core->L, "cursor");

    lua_pushcfunction(core->L, flip);
    lua_setglobal(core->L, "flip");

    lua_pushcfunction(core->L, line);
    lua_setglobal(core->L, "line");

    lua_pushcfunction(core->L, print);
    lua_setglobal(core->L, "print");

    lua_pushcfunction(core->L, pset);
    lua_setglobal(core->L, "pset");

    lua_pushcfunction(core->L, rect);
    lua_setglobal(core->L, "rect");

    lua_pushcfunction(core->L, rectfill);
    lua_setglobal(core->L, "rectfill");
}

static void clear_screen(int col)
{
    state.cur_x = 0;
    state.cur_y = 0;

    set_col(col, SDL_FALSE);
    if (state.renderer != NULL)
    {
        SDL_RenderClear(state.renderer);
    }
    set_col(state.col, SDL_FALSE);
}

// Draw pixels at subsequence points.
static void draw_circ_sub(int xc, int yc, int x, int y, int col)
{
    draw_pixel(xc+x, yc+y, col);
    draw_pixel(xc-x, yc+y, col);
    draw_pixel(xc+x, yc-y, col);
    draw_pixel(xc-x, yc-y, col);
    draw_pixel(xc+y, yc+x, col);
    draw_pixel(xc-y, yc+x, col);
    draw_pixel(xc+y, yc-x, col);
    draw_pixel(xc-y, yc-x, col);
}

// Draw cicrcle using Bresenham's algoritmh.
static void draw_circ(int xc, int yc, int r, int col)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    draw_circ_sub(xc, yc, x, y, col);

    while (y >= x)
    {
        x = x + 1;

        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
        draw_circ_sub(xc, yc, x, y, col);
    }
}

static void draw_frame(void)
{
    if (SDL_RenderCopy(state.renderer, state.frame, NULL, NULL) != 0)
    {
        SDL_Log("Unable to copy render target: %s", SDL_GetError());
    }
}

static void draw_line(int x0, int y0, int x1, int y1, int col)
{
    if (SDL_RenderDrawLine(state.renderer, x0, y0, x1, y1) != 0)
    {
        SDL_Log("Unable to draw line: %s", SDL_GetError());
    }
}

static void draw_pixel(int x, int y, int col)
{
    set_col(col, SDL_FALSE);
    if (SDL_RenderDrawPoint(state.renderer, x, y) != 0)
    {
        SDL_Log("Unable to draw pixel: %s", SDL_GetError());
    }
    set_col(state.col, SDL_FALSE);
}

static void draw_rect(int x0, int y0, int x1, int y1, int col, SDL_bool fill)
{
    SDL_Rect rect;
    int      w = 0;
    int      h = 0;

    if (x0 >= x1)
    {
        w = x0 - x1;
    }
    else
    {
        w = x1 - x0;
    }

    if (y0 >= y1)
    {
        h = y0 - y1;
    }
    else
    {
        h = y1 - y0;
    }

    rect.x = x0;
    rect.y = y0;
    rect.w = w;
    rect.h = h;

    set_col(col, SDL_FALSE);

    if (SDL_TRUE == fill)
    {
        if (SDL_RenderFillRect(state.renderer, &rect) != 0)
        {
            SDL_Log("Unable to fill rect: %s", SDL_GetError());
        }
    }

    if (SDL_RenderDrawRect(state.renderer, &rect) != 0)
    {
        SDL_Log("Unable to draw rect: %s", SDL_GetError());
    }

    set_col(state.col, SDL_FALSE);
}

static void draw_text(const char *str)
{
    int      i = 0;
    SDL_Rect src;
    SDL_Rect dst;

    src.x = 0;
    src.y = 0;
    src.w = 4;
    src.h = 6;

    dst.x = state.cur_x;
    dst.y = state.cur_y;
    dst.w = 4;
    dst.h = 6;

    if (NULL == str)
    {
        return;
    }

    while (str[i] != '\0')
    {
        get_character_position(str[i], &src.x, &src.y);

        if ('\n' == str[i])
        {
            state.cur_y += 6;
            dst.y        = state.cur_y;
            state.cur_x  = state.cur_margin - 4;
            dst.x        = state.cur_x;
        }

        i += 1;

        SDL_RenderCopy(state.renderer, state.font, &src, &dst);

        state.cur_x += 4;
        dst.x        = state.cur_x;
    }
}

static void flip_screen(void)
{
    SDL_Rect source = { 0, 0, 128, 128 };
    SDL_Rect dest   = { VIEWPORT_X, VIEWPORT_Y, VIEWPORT_W, VIEWPORT_H };

    if (SDL_SetRenderTarget(state.renderer, NULL) != 0)
    {
        SDL_Log("Unable to set render target: %s", SDL_GetError());
    }

    if (SDL_RenderCopy(state.renderer, state.render_target, &source, &dest) != 0)
    {
        SDL_Log("Unable to copy render target: %s", SDL_GetError());
    }

    SDL_RenderPresent(state.renderer);

    if (SDL_SetRenderTarget(state.renderer, state.render_target) != 0)
    {
        SDL_Log("Unable to set render target: %s", SDL_GetError());
    }
}

static void get_character_position(const unsigned char c, int* x, int* y)
{
    int i = 0;

    if ((c < 0x20) || (c > 0x7e))
    {
        i = 0;
    }
    else
    {
        i = c - 0x20;
    }

    *x = (i % 16) * 4;
    *y = (i / 16) * 6;
}

/* Based on
 * https://wiki.libsdl.org/SDL_CreateRGBSurfaceWithFormatFrom#Code_Examples
 */
static int load_texture_from_file(const char* file_name, SDL_Texture** texture)
{
    int            status     = 0;
    SDL_Surface*   surface;
    int            width;
    int            height;
    int            orig_format;
    int            req_format = STBI_rgb_alpha;
    int            depth;
    int            pitch;
    uint32_t       pixel_format;
    unsigned char* data;

    if (NULL == state.renderer)
    {
        status = -3;
        goto exit;
    }

    if (NULL == file_name)
    {
        status = -5;
        goto exit;
    }

    data = stbi_load(file_name, &width, &height, &orig_format, req_format);
    if (NULL == data)
    {
        SDL_Log("Unable to load image fil '%s': %s", file_name, stbi_failure_reason());
        status = -6;
        goto exit;
    }

    if (STBI_rgb == req_format)
    {
        depth        = 24;
        pitch        = 3 * width; // 3 bytes per pixel * pixels per row
        pixel_format = SDL_PIXELFORMAT_RGB24;
    }
    else
    {
        // STBI_rgb_alpha (RGBA)
        depth        = 32;
        pitch        = 4 * width;
        pixel_format = SDL_PIXELFORMAT_RGBA32;
    }

    surface = SDL_CreateRGBSurfaceWithFormatFrom((void*)data, width, height, depth, pitch, pixel_format);
    if (NULL == surface)
    {
        SDL_Log("Unable to create RGB surface: %s.", SDL_GetError());
        stbi_image_free(data);
        status = -7;
        goto exit;
    }

    if (SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xff, 0x00, 0xff)) != 0)
    {
        SDL_Log("Failed to set color key for %s: %s", file_name, SDL_GetError());
    }

    if (SDL_SetSurfaceRLE(surface, 1) != 0)
    {
        SDL_Log("Could not enable RLE for surface %s: %s", file_name, SDL_GetError());
    }

    *texture = SDL_CreateTextureFromSurface(state.renderer, surface);

    SDL_FreeSurface(surface);
    stbi_image_free(data);

exit:
    return status;
}

static void set_col(int col, SDL_bool update_state)
{
    Uint8 rd;
    Uint8 gn;
    Uint8 bl;

    if (col < 0)
    {
        col = 0;
    }
    else if (col > 15)
    {
        col = 15;
    }

    if (NULL == state.renderer)
    {
        return;
    }

    switch(col)
    {
        default:
        case 0:
            rd = 0x00, gn = 0x00, bl = 0x00;
            break;
        case 1:
            rd = 0x1d, gn = 0x2b, bl = 0x53;
            break;
        case 2:
            rd = 0x7e, gn = 0x25, bl = 0x53;
            break;
        case 3:
            rd = 0x00, gn = 0x87, bl = 0x51;
            break;
        case 4:
            rd = 0xab, gn = 0x52, bl = 0x36;
            break;
        case 5:
            rd = 0x5f, gn = 0x57, bl = 0x4f;
            break;
        case 6:
            rd = 0xc2, gn = 0xc3, bl = 0xc7;
            break;
        case 7:
            rd = 0xff, gn = 0xf1, bl = 0xe8;
            break;
        case 8:
            rd = 0xff, gn = 0x00, bl = 0x4d;
            break;
        case 9:
            rd = 0xff, gn = 0xa3, bl = 0x00;
            break;
        case 10:
            rd = 0xff, gn = 0xec, bl = 0x27;
            break;
        case 11:
            rd = 0x00, gn = 0xe4, bl = 0x36;
            break;
        case 12:
            rd = 0x29, gn = 0xad, bl = 0xff;
            break;
        case 13:
            rd = 0x83, gn = 0x76, bl = 0x9c;
            break;
        case 14:
            rd = 0xff, gn = 0x77, bl = 0xa8;
            break;
        case 15:
            rd = 0xff, gn = 0xcc, bl = 0xaa;
            break;
    }

    if (SDL_SetRenderDrawColor(state.renderer, rd, gn, bl, 0xff) != 0)
    {
        SDL_Log("Unable to set render color: %s", SDL_GetError());
    }

    if (SDL_SetTextureColorMod(state.font, rd, gn, bl) != 0)
    {
        SDL_Log("Unable to set font color: %s", SDL_GetError());
    }

    if (SDL_TRUE == update_state)
    {
        state.col = col;
    }
}

static void set_line_endpoint(int x, int y, SDL_bool validity)
{
    state.line_endpoint_x   = x;
    state.line_endpoint_y   = y;
    state.endpoint_validity = validity;
}

// API functions.
static int circ(lua_State* L)
{
    int argc = lua_gettop(L);
    int x    = (int)luaL_checkinteger(L, 1);
    int y    = (int)luaL_checkinteger(L, 2);
    int r    = 4;
    int col  = state.col;

    if (argc > 2)
    {
        r = (int)luaL_checkinteger(L, 3);
    }

    if (argc > 3)
    {
        col = (int)luaL_checkinteger(L, 4);
    }

    draw_circ(x, y, r, col);

    return 1;
}

static int cls(lua_State* L)
{
    int argc = lua_gettop(L);
    int col;

    if (argc > 0)
    {
        col = (int)luaL_checkinteger(L, 1);
    }
    else
    {
        col = 0;
    }

    clear_screen(col);

    return 1;
}

static int color(lua_State* L)
{
    int argc = lua_gettop(L);
    int ret  = (int)state.col;

    if (argc > 0)
    {
        state.col = (int)luaL_checkinteger(L, 1);
    }
    else
    {
        state.col = 6;
    }

    lua_pushinteger(L, ret);
    return 1;
}

static int cursor(lua_State* L)
{
    int argc = lua_gettop(L);
    int col;

    switch (argc)
    {
        case 1:
            state.cur_x      = (int)luaL_checkinteger(L, 1);
            state.cur_y      = 0;
            state.cur_margin = state.cur_x;
            break;
        case 2:
            state.cur_x      = (int)luaL_checkinteger(L, 1);
            state.cur_y      = (int)luaL_checkinteger(L, 2);
            state.cur_margin = state.cur_x;
            break;
        case 3:
            state.cur_x      = (int)luaL_checkinteger(L, 1);
            state.cur_y      = (int)luaL_checkinteger(L, 2);
            state.cur_margin = state.cur_x;
            col              = (int)luaL_checkinteger(L, 3);
            set_col(col, SDL_TRUE);
            break;
    }

    return 1;
}

static int flip(lua_State* L)
{
    flip_screen();
    return 1;
}

static int line(lua_State* L)
{
    int      argc              = lua_gettop(L);
    int      x0                = -1;
    int      y0                = -1;
    int      x1                = -1;
    int      y1                = -1;
    int      col               = state.col;
    SDL_bool call_draw_line    = SDL_FALSE;
    SDL_bool endpoint_validity = SDL_FALSE;
    SDL_bool set_color         = SDL_FALSE;

    switch (argc)
    {
        case 0:
            break;
        case 1:
            col       = (int)luaL_checkinteger(L, 1);
            set_color = SDL_TRUE;
            break;
        case 2:
            x0 = state.line_endpoint_x;
            y0 = state.line_endpoint_y;
            x1 = (int)luaL_checkinteger(L, 1);
            y1 = (int)luaL_checkinteger(L, 2);

            if (SDL_TRUE == state.endpoint_validity)
            {
                call_draw_line = SDL_TRUE;
            }

            endpoint_validity = SDL_TRUE;
            break;
        case 3:
            x0  = state.line_endpoint_x;
            y0  = state.line_endpoint_y;
            x1  = (int)luaL_checkinteger(L, 1);
            y1  = (int)luaL_checkinteger(L, 2);
            col = (int)luaL_checkinteger(L, 3);

            if (SDL_TRUE == state.endpoint_validity)
            {
                call_draw_line = SDL_TRUE;
            }

            set_color         = SDL_TRUE;
            endpoint_validity = SDL_TRUE;
            break;
        case 4:
            x0                = (int)luaL_checkinteger(L, 1);
            y0                = (int)luaL_checkinteger(L, 2);
            x1                = (int)luaL_checkinteger(L, 3);
            y1                = (int)luaL_checkinteger(L, 4);
            endpoint_validity = SDL_TRUE;
            call_draw_line    = SDL_TRUE;
            break;
        default:
        case 5:
            x0                = (int)luaL_checkinteger(L, 1);
            y0                = (int)luaL_checkinteger(L, 2);
            x1                = (int)luaL_checkinteger(L, 3);
            y1                = (int)luaL_checkinteger(L, 4);
            col               = (int)luaL_checkinteger(L, 5);
            endpoint_validity = SDL_TRUE;
            call_draw_line    = SDL_TRUE;
            set_color         = SDL_TRUE;
            break;
    }

    set_line_endpoint(x1, y1, endpoint_validity);

    set_col(col, set_color);
    if (SDL_TRUE == call_draw_line)
    {
        draw_line(x0, y0, x1, y1, col);
    }
    set_col(col, set_color);

    return 1;
}

static int print(lua_State* L)
{
    const char* str = luaL_checkstring(L, 1);
    int         argc;
    int         col;
    int         ret;

    if (NULL == str)
    {
        return 0;
    }

    lua_remove(L, 1);
    argc = lua_gettop(L);

    switch (argc)
    {
        case 0:
            if (state.cur_x <= state.cur_margin)
            {
                state.cur_margin = state.cur_x;
            }
            state.cur_x = state.cur_margin;
            break;
        case 1:
            if (state.cur_x <= state.cur_margin)
            {
                state.cur_margin = state.cur_x;
            }
            state.cur_x = state.cur_margin;
            col         = (int)luaL_checkinteger(L, 1);
            set_col(col, SDL_TRUE);
            break;
        case 2:
            state.cur_x      = (int)luaL_checkinteger(L, 1);
            state.cur_y      = (int)luaL_checkinteger(L, 2);
            state.cur_margin = state.cur_x;
            break;
        case 3:
            state.cur_x      = (int)luaL_checkinteger(L, 1);
            state.cur_y      = (int)luaL_checkinteger(L, 2);
            state.cur_margin = state.cur_x;
            col              = (int)luaL_checkinteger(L, 3);
            set_col(col, SDL_FALSE);
            break;
    }

    draw_text(str);

    if (argc <= 1)
    {
        state.cur_y += 6;
    }
    set_col(state.col, SDL_FALSE);

    ret = state.cur_x;
    lua_pushinteger(L, ret);

    return 1;
}

static int pset(lua_State* L)
{
    int argc = lua_gettop(L);
    int x    = (int)luaL_checkinteger(L, 1);
    int y    = (int)luaL_checkinteger(L, 2);
    int col  = state.col;

    if (argc > 2)
    {
        col = (int)luaL_checkinteger(L, 3);
    }

    draw_pixel(x, y, col);

    return 1;
}

static int rect(lua_State* L)
{
    int argc = lua_gettop(L);
    int x0   = (int)luaL_checkinteger(L, 1);
    int y0   = (int)luaL_checkinteger(L, 2);
    int x1   = (int)luaL_checkinteger(L, 3);
    int y1   = (int)luaL_checkinteger(L, 4);
    int col  = state.col;

    if (argc > 4)
    {
        col = (int)luaL_checkinteger(L, 5);
    }
    draw_rect(x0, y0, x1, y1, col, SDL_FALSE);

    return 1;
}

static int rectfill(lua_State* L)
{
    int argc = lua_gettop(L);
    int x0   = (int)luaL_checkinteger(L, 1);
    int y0   = (int)luaL_checkinteger(L, 2);
    int x1   = (int)luaL_checkinteger(L, 3);
    int y1   = (int)luaL_checkinteger(L, 4);
    int col  = state.col;

    if (argc > 4)
    {
        col = (int)luaL_checkinteger(L, 5);
    }
    draw_rect(x0, y0, x1, y1, col, SDL_TRUE);

    return 1;
}
