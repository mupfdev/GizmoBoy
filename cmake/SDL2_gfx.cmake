cmake_minimum_required(VERSION 3.10)

include(FetchContent)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
  cmake_policy(SET CMP0135 NEW)
endif()

set(SDL_VERSION "2.24.2")

FetchContent_Populate(
  SDL2
  URL https://github.com/libsdl-org/SDL/releases/download/release-${SDL_VERSION}/SDL2-${SDL_VERSION}.zip
  URL_HASH
  SHA256=7fae98ac4e7b39eb2511fc27c2e84b220ac69b5296ff41f833b967c891f9d2ac
  SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/SDL-${SDL_VERSION})

project(SDL2_gfx C)

set(SDL2_gfx_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_framerate.c
  ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_gfxPrimitives.c
  ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_imageFilter.c
  ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_rotozoom.c)

add_library(SDL2_gfx
  STATIC
  ${SDL2_gfx_sources})

target_include_directories(SDL2_gfx
  PUBLIC
  ${CMAKE_CURRENT_BINARY_DIR}/SDL-${SDL_VERSION}/include)

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  set(COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wpointer-arith
    -Wshadow
    -Wsign-compare
    -Wundef
    -Wwrite-strings)

elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  set(COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wpointer-arith
    -Wshadow
    -Wsign-compare
    -Wundef
    -Wwrite-strings)

elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  set(COMPILE_OPTIONS
    /W4
    /utf-8)
endif()
