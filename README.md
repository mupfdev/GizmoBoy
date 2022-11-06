# GizmoBoy

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/4b253572c9bc49b1b94728f579792e97)](https://www.codacy.com/gh/mupfdev/GizmoBoy/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=mupfdev/GizmoBoy&amp;utm_campaign=Badge_Grade)
[![CMake](https://github.com/mupfdev/GizmoBoy/actions/workflows/cmake.yml/badge.svg)](https://github.com/mupfdev/GizmoBoy/actions/workflows/cmake.yml)
[![API Reference](https://img.shields.io/badge/docs-API%20Reference-informational)](https://mupfdev.github.io/GizmoBoy)

## Description

A portable fantasy console inspired by
[PICO-8](https://www.lexaloffle.com/pico-8.php) from [Lexaloffle
Games](https://www.lexaloffle.com) written in ANSI C using SDL2.

## Portability

GizmoBoy is written entirely in ANSI C and should run on all platforms
supported by SDL2 such as Android, IBM OS/2, Linux, Nintendo 3DS, Nokia
N-Gage, Sony PS2, Sony PSP, Windows.

It is my goal to support as many platforms as possible.

## Compiling

### Windows

The easiest way to get the GizmoBoy up and running is Visual Studio 2022
with [C++ CMake tools for
Windows](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio)
installed.  Simply open the cloned repository via `File -> Open ->
Folder`.  Everything else is set up automatically and all required
dependencies are fetched at compile time.

### Linux

GizmoBoy can also be compiled on Linux with the included CMake
configuration.

```bash
mkdir build
cd build
cmake ..
make
````

## Trivia

GizmoBoy is a neologism made up of GameBoy and Gizmo, a nonsensical
placeholder name for something that one does not know the proper term
for.

Despite the name, this project has nothing to do with Tiger Telematics'
Gizmondo.

## License and Credits

- This project's source code is, unless stated otherwise, licensed under
  the "The MIT License".  See the file [LICENSE.md](LICENSE.md) for
  details.

- stb by Sean Barrett is licensed under "The MIT License".  See the file
  [LICENSE](https://github.com/nothings/stb/blob/master/LICENSE) for
  details.

- I adapted much of my API description from the [PICO-8
  Wiki](https://pico-8.fandom.com/wiki/Pico-8_Wikia) and owe thanks to
  the countless authors for the great reference.  This content is
  licensed under [CC-BY-SA](https://www.fandom.com/licensing).
