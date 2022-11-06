# API Reference

GizmoBoy games are written in Lua 5.4, please refer to the official Lua
5.4 reference manual which can be found here: [Lua 5.4 Reference
Manual](https://www.lua.org/manual/5.4/).

!> Please note that most of the standard libraries are not available.
This limitation is intentional.

<details>
<summary>Available standard libraries</summary>

- <a href="https://www.lua.org/manual/5.3/manual.html#6.6">Table Manipulation</a>

</details>

## The game loop

The game loop is the mechanism that drives a game's logic and rendering
processes. Because each cycle of the loop updates the display, it is
important for the loop to run at regular intervals to keep the animation
smooth and the game feeling responsive to user input.

GizmoBoy has a built-in game loop.  You use this in your code by
defining the function: `_update()`

### `_update()`

Updates the main game loop.

```lua
_update( )
```

?> You define this function in your game's source code.  It takes no
arguments.

Example:

```lua
function _update()
    cls(11)
    print("Hello, World!", 40, 58, 1)
    flip()
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
    </tr>
    <tr>
        <td>0</td>
        <td>LEFT</td>
        <td>0x0001</td>
    </tr>
    <tr>
        <td>1</td>
        <td>RIGHT</td>
        <td>0x0002</td>
    </tr>
    <tr>
        <td>2</td>
        <td>UP</td>
        <td>0x0004</td>
    </tr>
    <tr>
        <td>3</td>
        <td>DOWN</td>
        <td>0x0008</td>
    </tr>
    <tr>
        <td>4</td>
        <td>O</td>
        <td>0x0010</td>
    </tr>
    <tr>
        <td>5</td>
        <td>X</td>
        <td>0x0020</td>
    </tr>
</table>

</details>

> **Return value** If a button is specified, then true or false,
otherwise a bitfield.
