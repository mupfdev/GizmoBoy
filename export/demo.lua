--[[

Title:   Demo
Author:  Michael Fitzmayer
License: Public domain

--]]

x = 40
y = 58

function _update()
    cls(11)

    if (btn(0) == true) then
        x = x - 1
    end

    if (btn(1) == true) then
        x = x + 1
    end

    if (btn(2) == true) then
        y = y - 1
    end

    if (btn(3) == true) then
        y = y + 1
    end

    print("Hello, World!", x, y, 3)

    flip()
end
