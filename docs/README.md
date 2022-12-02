![GizmoBoy](https://raw.githubusercontent.com/mupfdev/GizmoBoy/main/media/logo.svg)

This is the API documentation of GizmoBoy, a portable fantasy console
inspired by [PICO-8](https://www.lexaloffle.com/pico-8.php) from
[Lexaloffle Games](https://www.lexaloffle.com) written in ANSI C using
SDL2.

You can find the project page on GitHub:
[mupfdev/GizmoBoy](https://github.com/mupfdev/GizmoBoy).

You can find a web version of GizmoBoy <a href="/wasm.html"
target="_blank">here</a>.

# API Reference

GizmoBoy games are written in Lua 5.4, please refer to the official Lua
5.4 reference manual which can be found here: [Lua 5.4 Reference
Manual](https://www.lua.org/manual/5.4/).

!> Please note that the standard libraries are not available.  This
limitation is intentional.

## The game loop

The game loop is the mechanism that drives a game's logic and rendering
processes. Because each cycle of the loop updates the display, it is
important for the loop to run at regular intervals to keep the animation
smooth and the game feeling responsive to user input.

GizmoBoy has a built-in game loop.  You use this in your code by
defining three functions: `_init()`, `_update()`, and `_draw()`.

### `_init()`

```lua
_init( )
```

The `_init()` function is called by GizmoBoy to initialize a cartridge.

In typical usage, a cartridge's source code includes definitions for
functions and variables, an `_init()` function, and then an `_update()`
function and `_draw()` function for the game loop.  GizmoBoy calls
`_init()` once before starting the game loop.

?> You define this function in your game's source code.  It takes no
arguments.

Example:

```lua
function _init()
  player_x = 64
  player_y = 100
end
```

### `_update()`

Updates the main game loop.

```lua
_update( )
```

?> You define this function in your game's source code.  It takes no
arguments.

Example:

```lua
player_x = 64
player_y = 100

function _update()
    if btn(0) then
        player_x = player_x - 1
    end

    if btn(1) then
        player_x = player_x + 1
    end
end
```

### `_draw()`

The _draw() function is the second part of the game loop.

```lua
_draw( )
```

If a cartridge's source code includes both an `_update()` function and a
`_draw()` function, then GizmoBoy will attempt to call these functions
once for each animation frame.

?> You define this function in your game's source code. It takes no
arguments.

Example:

```lua
function _draw
    cls()
    print("Hello, world!")
end
```

## Graphics

### `circ()`

Draws a circle shape, without fill.

```lua
circ( x, y, [r,] [col] )
```

> **x** The x coordinate of the center of the circle.

> **y** The y coordinate of the center of the circle.

> **r** The radius of the circle, in pixels.  If omitted, the radius
will be 4.

> **col** The color of the circle and fill.  If omitted, the color from
the draw state is used.

### `circfill()`

Draws a filled-in circle shape.

```lua
circfill( x, y, [r,] [col] )
```

> **x** The x coordinate of the center of the circle.

> **y** The y coordinate of the center of the circle.

> **r** The radius of the circle, in pixels.  If omitted, the radius
will be 4.

> **col** The color of the circle and fill.  If omitted, the color from
the draw state is used.

### `cls()`

Clears the graphic buffer.

```lua
cls( [col] )
```

> **col** A color to use for the background.  The default is **0**
(black).

!> `cls()` also sets the text cursor to (0, 0).

### `color()`

Sets the draw color.

```lua
color( [col] )
```

> **col** The color number.  Default is **6** (light gray).

<details>
<summary>Color numbers</summary>

- <span style='color: #000000'>**0**  `#000000`  black</span>
- <span style='color: #1d2b53'>**1**  `#1d2b53`  dark-blue</span>
- <span style='color: #7e2553'>**2**  `#7e2553`  dark-purple</span>
- <span style='color: #008751'>**3**  `#008751`  dark-green</span>
- <span style='color: #ab5236'>**4**  `#ab5236`  brown</span>
- <span style='color: #5f574f'>**5**  `#5f574f`  dark-grey</span>
- <span style='color: #c2c3c7'>**6**  `#c2c3c7`  light-grey</span>
- <span style='color: #fff1e8'>**7**  `#fff1e8`  white</span>
- <span style='color: #ff004d'>**8**  `#ff004d`  red</span>
- <span style='color: #ffa300'>**9**  `#ffa300`  orange</span>
- <span style='color: #ffec27'>**10** `#ffec27`  yellow</span>
- <span style='color: #00e436'>**11** `#00e436`  green</span>
- <span style='color: #29adff'>**12** `#29adff`  blue</span>
- <span style='color: #83769c'>**13** `#83769c`  avender</span>
- <span style='color: #ff77a8'>**14** `#ff77a8`  pink</span>
- <span style='color: #ffccaa'>**15** `#ffccaa`  light-peach</span>

</details>

> **Return value** The previous color is returned when calling color(),
allowing it to be saved and restored if needed.

### `cursor()`

Sets the left-margin cursor position for `print()`.

```lua
cursor( [x,] [y,] [col] )
```

> **x** The x coordinate of the upper left corner of the line.  The
default is **0**.

> **y** The y coordinate of the upper left corner of the line. The
default is **0**.

> **col** The palette index to set the pen color to (optional).

### `flip()`

Copies the graphics buffer to the screen.

```lua
flip( )
```

### `line()`

Draws a line between two points.

```lua
line( [x0,] [y0,] [x1,] [y1,] [col] )
```

> **x0** The x coordinate of the start of the line. If omitted, the x
coordinate of the end of the previous line is used, or **0** if no
previous line has been drawn.

> **y0** The y coordinate of the start of the line. If omitted, the y
coordinate of the end of the previous line is used, or **0** if no
previous line has been drawn.

> **x1** The x coordinate of the end of the line.

> **y1** The y coordinate of the end of the line.

> **col** The color of the line. If omitted, the color from the draw
state is used. This also sets the color in the draw state.

The `line()` function is very versatile, with different operations
depending on how many arguments are supplied.  Line segments, strips,
and loops are all easily created with this function.

This is how GizmoBoy interprets each possible set of arguments:

Draws a line from (`x0, y0`) to (`x1, y1`) in the given color.
Remembers (`x1, y1`) as the current endpoint and color as the current
pen color:

```lua
line(x0, y0, x1, y1, col)
```

Draws a line from (`x0, y0`) to (`x1, y1`) in the current pen color.
Remembers (`x1, y1`) as the current endpoint:

```lua
line(x0, y0, x1, y1)
```

Draws a line from the current endpoint to (`x1, y1`) in the given color.
If there is no current endpoint, nothing is drawn. Remembers (`x1, y1`)
as the current endpoint and color as the current pen color:

```lua
line(x1, y1, col)
```

Draws a line from the current endpoint to (`x1, y1`) in the current pen
color.  If there is no current endpoint, nothing is drawn.  Remembers
(`x1, y1`) as the current endpoint:

```lua
line(x1, y1)
```

Invalidates the current endpoint. Remembers color as the current pen color:

```lua
line(col)
```

Invalidates the current endpoint:

```lua
line()
```

### `print()`

Prints a string of characters to the screen.

```lua
print( text, [x,] [y,] [col] )
```

> **text** The Lua string of characters to print.

> **x** The x coordinate of the upper left corner to start printing.

> **y** The y coordinate of the upper left corner to start printing.

> **col** The color to use for the text.

> **Return value** The x coordinate of the next character to be printed
(can be used to calculate printed width).

The print function writes a line of text to the screen, and then moves
the cursor to the start of the next line.  To write multiple lines in
one call, insert one or more newline characters (`\n`) in the text.

If only the text argument is supplied, `print()` uses the cursor position
and draw color from the current draw state to determine where to draw
the text, and what color to use.

Unless specific **x** and **y** position coordinates are supplied,
`print()` behaves like a retro text-based scrolling display.  The text
is rendered to the screen and then the cursor moves to the left margin
on the next line, as if text had a hidden newline character at the end.
If there isn't enough room to have another line on the screen, the
screen scrolls upwards to make enough.

```lua
print("Hello, world!")
print("Hello, world!")
print("Hello, world!")
```

The three additional arguments are all optional, and can be supplied in
three ways:

If there is only one additional argument, it is assumed to be the color
argument.  The text is printed in the same manner as above, but in the
given color.  This also changes the current draw color of the draw
state.

```lua
print("Hello, world!", 8)
```

If there are two additional arguments, they are assumed to be the **x**
and **y** arguments, which are used as the upper left corner of the
rectangle containing the text.  The current draw color of the draw state
is used, and the current draw position and left margin are set
accordingly.

```lua
print("Hello, World!", 40, 58)
```

If there are three additional arguments, they are assumed to be the
**x**, **y** and **col** arguments. The text is printed in the same
manner as when there are just **x** and **y**, but using the given color
instead of the one in the draw state.

```lua
print("Hello, World!", 40, 58, 8)
```

### `pset()`

Sets a pixel in the graphics buffer.

```lua
pset( x, y, [col] )
```

> **x** The x coordinate.

> **y** The y coordinate.

> **col** The color value. If not specified, uses the current color of
the draw state.


### `rect()`

Draws an empty rectangle shape.

```lua
rect( x0, y0, x1, y1, [col] )
```

> **x0** The x coordinate of the upper left corner.

> **y0** The y coordinate of the upper left corner.

> **x1** The x coordinate of the lower right corner.

> **y1** The y coordinate of the lower right corner.

> **col** The color of the rectangle border.  If omitted, the color from
the draw state is used.

### `rectfill()`

Draws a filled-in rectangle shape.

```lua
rectfill( x0, y0, x1, y1, [col] )
```

> **x0** The x coordinate of the upper left corner.

> **y0** The y coordinate of the upper left corner.

> **x1** The x coordinate of the lower right corner.

> **y1** The y coordinate of the lower right corner.

> **col** The color of the rectangle border.  If omitted, the color from
the draw state is used.

## Input

### `btn()`

Tests if a button is being pressed at this moment.

```lua
btn( [i] )
```

> **i** The button number.

<details>
<summary>Button numbers</summary>

<br />
<table>
    <tr>
        <td>Number</td>
        <td>Button</td>
        <td>Mask</td>
        <td>PC</td>
        <td>N-Gage</td>
    </tr>
    <tr>
        <td>0</td>
        <td>LEFT</td>
        <td>0x0001</td>
        <td>&#8592;</td>
        <td>&#8592;</td>
    </tr>
    <tr>
        <td>1</td>
        <td>RIGHT</td>
        <td>0x0002</td>
        <td>&#8594;</td>
        <td>&#8594;</td>
    </tr>
    <tr>
        <td>2</td>
        <td>UP</td>
        <td>0x0004</td>
        <td>&#8593;</td>
        <td>&#8593;</td>
    </tr>
    <tr>
        <td>3</td>
        <td>DOWN</td>
        <td>0x0008</td>
        <td>&#8595;</td>
        <td>&#8595;</td>
    </tr>
    <tr>
        <td>4</td>
        <td>O</td>
        <td>0x0010</td>
        <td>z</td>
        <td>7</td>
    </tr>
    <tr>
        <td>5</td>
        <td>X</td>
        <td>0x0020</td>
        <td>x</td>
        <td>5</td>
    </tr>
</table>

</details>

> **Return value** If a button is specified, then true or false,
otherwise a bitfield.

## Math

### `rnd()`

Generates a random number under the given limit.

```lua
rnd( limit )
```

The `rnd()` function returns a random number in a range.  The result
ranges from 0 up the given limit, but will never be the limit value
itself.  For instance, `rnd(10)` can return from **0** up to **9**, but
will never return **10**.

> **limit** The range limit.

> **Return value** A random number between **0** and **limit - 1**.

## System

### `uid()`

Returns a unique platform identifier.

```lua
uid( )
```

> **Return value** A unique platform identifier.

<details>
<summary>Platform identifiers</summary>

- **-1** Unknown Platform
- **0** AIX
- **1** Android
- **2** BSDI
- **3** Dreamcast
- **4** Emscripten
- **5** FreeBSD
- **6** Haiku
- **7** HP-UX
- **8** Irix
- **9** Linux
- **10** Atari MiNT
- **11** MacOS Classic
- **12** Mac OS X
- **13** NaCl
- **14** NetBSD
- **15** OpenBSD
- **16** OS/2
- **17** OSF/1
- **18** QNX Neutrino
- **19** RISC OS
- **20** Solaris
- **21** Windows
- **22** WinRT
- **23** WinGDK
- **24** Xbox One
- **25** Xbox Series X|S
- **26** tvOS
- **27** iOS
- **28** PlayStation 2
- **29** PlayStation Portable
- **30** PlayStation Vita
- **31** Nokia N-Gage
- **32** Nintendo 3DS

</details>
