# Project configuration.

set(PROJECT_TITLE "GizmoBoy")

set(project_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/input.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/maths.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/platform.c)

set(UID3     0x1000c917) # game.exe UID
set(APP_UID  0x1000c918) # launcher.app UID
set(APP_NAME "GizmoBoy")
