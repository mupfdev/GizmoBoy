-- Geodezik by @aliceffekt http://xxiivv.com

frame = 0

function _update()
    frame = frame + 1
    -- loop at f127
    if frame > 127 then
        frame = 0
    end
end

function _draw()
    rectfill(0, 0, 127, 127, 0)
    cls()
    i = 0
    while(i < 20) do
        e = (i * 0.5)
        line(0, (frame * e), 127 - (frame * e), 0, 7)
        line((frame * e), 127, 0, (frame * e), 7)
        line(127, 127 - (frame * e), (frame * e), 127, 7)
        line(127 - (frame * e), 0, 127, 127 - (frame * e), 7)
        i = i + 1
    end
end
