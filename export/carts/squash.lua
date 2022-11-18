-- A game of squash by Felix Balzer (@pizzamakesgames)

score = 0

-- Paddle
padx = 52
pady = 122
padw = 24
padh = 4

-- Ball
ballx    = 64
bally    = 64
ballsize = 3
ballxdir = 3
ballydir = -1

function movepaddle()
    if btn(0) then
        padx = padx - 2
    elseif btn(1) then
        padx = padx + 2
    end

    if padx <= 0 then
        padx = 0
    elseif padx >= 127 - padw then
        padx = 128 - padw
    end
end

function moveball()
    ballx = ballx + ballxdir
    bally = bally - ballydir
end

function bounceball()
    -- Left
    if ballx < ballsize then
        ballxdir = -ballxdir
    end

    -- Right
    if ballx > 128 - ballsize then
        ballxdir = -ballxdir
    end

    -- Top
    if bally < ballsize then
        ballydir = -ballydir
    end
end

function bouncepaddle()
    if ballx > padx and ballx <= padx + padw and bally > pady then
        ballydir = - ballydir
        score = score + 10
    end
end

function losedeadball()
    if bally > 128 then
        bally = 24
        score = 0
    end
end

function _update()
    movepaddle()
    bounceball()
    bouncepaddle()
    moveball()
    losedeadball()
end

function _draw()
    cls(11)
    print("Score: ", 2, 2, 0)
    print(score, 30, 2, 0)
    -- Draw the paddle
    rectfill(padx, pady, padx + padw, pady + padh, 3)
    -- Draw the ball
    circfill(ballx, bally, ballsize, 3)
end
