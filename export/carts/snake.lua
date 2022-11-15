-- Snake.

local food    = 0
local is_dead = false
local map     = {}
local s       = {}
local score   = 0
local t       = 0

function check_input()
    if is_dead == false then
        if btn(0) and s.dir ~= 1 then s.dir = 0
        elseif btn(1) and s.dir ~= 0 then s.dir = 1
        elseif btn(2) and s.dir ~= 3 then s.dir = 2
        elseif btn(3) and s.dir ~= 2 then s.dir = 3 end
    else
        if btn(4) or btn(5) then reset_game() end
    end
end

function create_food()
    food = rnd(1024)
    map[food] = -1
end

function draw_map()
    for index = 1, 1024 do
        y = (index - 1) // 32
        x = (index - 1) % 32
        y = y * 4
        x = x * 4
        w = x + 4
        h = y + 4

        if map[index] > 0 or map[index] < 0 then
            rectfill(x, y, w, h, 3)
        end
    end
end

function draw_score(col)
    cursor(2, 2)
    print("Score:", col)
    cursor(28, 2)
    print(score, col)
end

function reset_game()
    for index = 1, 1024 do
        map[index] = 0
    end

    s.dir    = 1
    s.len    = 10
    s.x      = 16
    s.y      = 16
    index    = (32 * s.y) + s.x
    map[100] = -1
    is_dead  = false
end

function update_map()
    for index = 1, 1024 do
        if map[index] > 0 then
            map[index] = map[index] + 1
        end

        if map[index] > s.len then
            map[index] = 0
        end
    end

    index = (32 * s.y) + s.x

    if map[index] == -1 then
        s.len = s.len + 1
        score = score + 10
        create_food()
    end

    if map[index] > 0 then
        is_dead = true
    else
        map[index] = 1
    end
end

function update_pos()
    if s.dir == 0 then s.x = s.x - 1
    elseif s.dir == 1 then s.x = s.x + 1
    elseif s.dir == 2 then s.y = s.y - 1
    elseif s.dir == 3 then s.y = s.y + 1 end

    if s.x < 1 then s.x = 32
    elseif s.x > 32 then s.x = 1 end

    if s.y < 0 then s.y = 31
    elseif s.y > 31 then s.y = 0 end
end

function _init()
    reset_game()
end

function _update()
    check_input()

    if is_dead == false then
        if t > 2 then
            update_pos()
            update_map()
            t = 0
        end
        t = t + 1
    end
end

function _draw()
    if is_dead == false then
        cls(11)
        draw_map()
        draw_score(0)
    else
        cls(0)
        print("Game Over", 48, 58, 8)
        draw_score(11)
    end
end
