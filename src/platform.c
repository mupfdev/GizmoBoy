/** @file platform.c
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
#include "platform.h"

static Uint64 generate_hash(const char* str);

// API functions.
static int uid(lua_State* L);

void register_platform_api(core_t* core)
{
    lua_pushcfunction(core->L, uid);
    lua_setglobal(core->L, "uid");
}

static Uint64 generate_hash(const char* str)
{
    Uint64 hash = 5381;
    int    c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

// API functions.
static int uid(lua_State* L)
{
    int    ret      = 0;
    Uint64 platform = generate_hash(SDL_GetPlatform());

    switch (platform)
    {
        case 0x000000000b87d127: // AIX
            ret = 0;
            break;
        case 0x0000d0a797327b46: // Android
            ret = 1;
            break;
        case 0x000000017c82aaa7: // BSDI
            ret = 2;
            break;
        case 0x03779ce861661c99: // Dreamcast
            ret = 3;
            break;
        case 0x726b5e50dd72c43f: // Emscripten
            ret = 4;
            break;
        case 0x0000d0a9217482c0: // FreeBSD
            ret = 5;
            break;
        case 0x000000310d4ce217: // Haiku
            ret = 6;
            break;
        case 0x000000310d428d77: // HP-UX
            ret = 7;
            break;
        case 0x000000017c870a21: // Irix
            ret = 8;
            break;
        case 0x000000310d99bdb5: // Linux
            ret = 9;
            break;
        case 0x726abdbec87d60ae: // Atari MiNT
            ret = 10;
            break;
        case 0xc2a9f7a91fff0d7a: // MacOS Classic
            ret = 11;
            break;
        case 0x001ae60d9fafd4b0: // Mac OS X
            ret = 12;
            break;
        case 0x000000017c897ac3: // NaCl
            ret = 13;
            break;
        case 0x00000652c535d065: // NetBSD
            ret = 14;
            break;
        case 0x0000d0abd19b43b0: // OpenBSD
            ret = 15;
            break;
        case 0x000000017c89c8c8: // OS/2
            ret = 16;
            break;
        case 0x000000310dc3436d: // OSF/1
            ret = 17;
            break;
        case 0xc2797aaddf3a5eb0: // QNX Neutrino
            ret = 18;
            break;
        case 0x0000d0ac5c32eaf8: // RISC OS
            ret = 19;
            break;
        case 0x0000d0ad03a767e2: // Solaris
            ret = 20;
            break;
        case 0x0000d0ae29b73250: // Windows
            ret = 21;
            break;
        case 0x000000310e60c699: // WinRT
            ret = 22;
            break;
        case 0x00000652da796929: // WinGDK
            ret = 23;
            break;
        case 0x001ae67b359abe48: // Xbox One
            ret = 24;
            break;
        case 0x5ba72ce1706eff98: // Xbox Series X|S
            ret = 25;
            break;
        case 0x000000017c9eabf1: // tvOS
            ret = 26;
            break;
        case 0x000000000b887c10: // iOS
            ret = 27;
            break;
        case 0x0fcdcd7a65b8432f: // PlayStation 2
            ret = 28;
            break;
        case 0xe75f047b84125476: // PlayStation Portable
            ret = 29;
            break;
        case 0x8959cb014fa4b471: // PlayStation Vita
            ret = 30;
            break;
        case 0xc115db296af35bc6: // Nokia N-Gage
            ret = 31;
            break;
        case 0xc0f5c255b56b250e: // Nintendo 3DS
            ret = 32;
            break;
        default:
            ret = -1; // Unknown Platform
            break;
    }

    lua_pushinteger(L, ret);

    return 1;
}
