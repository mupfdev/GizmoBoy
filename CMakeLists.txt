# Do not modify this file unless absolutely necessary.
# All project-specific settings can be found in the file:
# project_config.cmake

cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Use CMake or Visual Studio to enable these settings.
option(BUILD_FOR_NOKIA_NGAGE "Build for the Nokia N-Gage" OFF)
option(INCLUDE_SDL2_MIXER    "Include SDL2_mixer"         OFF)

if(BUILD_FOR_NOKIA_NGAGE)
  include(Nokia_N-Gage)
  return()
endif()

include(project_config)
include(get_SDL2)
include(get_dirent)

project(${PROJECT_TITLE} C CXX)

set(EXPORT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/export)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXPORT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXPORT_DIR})

get_SDL2("2.24.1")
if (INCLUDE_SDL2_MIXER)
  get_SDL2_mixer("2.6.2")
endif()
get_dirent("1.23.2")

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(LUA_VERSION   "5.4.4")
set(LUA_DEVEL_PKG v${LUA_VERSION}.zip)

if(WIN32)
  set(LUA_PATH ${CMAKE_CURRENT_BINARY_DIR}/_deps/lua-${LUA_VERSION}_Windows)
endif(WIN32)

if(UNIX)
  set(LUA_PATH ${CMAKE_CURRENT_BINARY_DIR}/_deps/lua-${LUA_VERSION}_Linux)
endif(UNIX)

ExternalProject_Add(Lua_devel
  URL https://github.com/lua/lua/archive/refs/tags/${LUA_DEVEL_PKG}
  URL_HASH SHA1=21b2815c0ec280877a2a8af4b865cba30df80260
  DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps
  DOWNLOAD_NO_PROGRESS true
  TLS_VERIFY true
  SOURCE_DIR ${LUA_PATH}/

  BUILD_IN_SOURCE 1
  INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "Skipping install step."

  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/lua.cmake" ${LUA_PATH}/CMakeLists.txt)

set(LUA_INCLUDE_DIR ${LUA_PATH})
link_directories(${LUA_PATH})

set(GUI_TYPE "")

if (CMAKE_BUILD_TYPE MATCHES Release)
  #-- Configure the OS X Bundle Plist
  if (APPLE)
    set(GUI_TYPE MACOSX_BUNDLE)
  elseif(WIN32)
    set(GUI_TYPE WIN32)
  endif()
endif()

add_executable(${PROJECT_TITLE} ${GUI_TYPE} ${project_sources})

set_target_properties(
  ${PROJECT_TITLE}
  PROPERTIES
  C_STANDARD   90
  CXX_STANDARD 98)

target_include_directories(
  ${PROJECT_TITLE}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE
  ${SDL2_INCLUDE_DIRS}
  PRIVATE
  ${LUA_INCLUDE_DIR}
  ${DIRENT_INCLUDE_DIR})

if (INCLUDE_SDL2_MIXER)
  target_link_libraries(
    ${PROJECT_TITLE}
    PRIVATE
    ${SDL2_MIXER_LIBS})
endif()

target_link_libraries(
  ${PROJECT_TITLE}
  PRIVATE
  ${SDL2_LIBRARIES}
  lua)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND UNIX)
  target_link_libraries(
    ${PROJECT_TITLE}
    PRIVATE
    m)
endif()

add_dependencies(
  ${PROJECT_TITLE}
  Lua_devel)

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  set(COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic)

elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  set(COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic)

elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  set(COMPILE_OPTIONS
    /W4
    /utf-8)
endif()
