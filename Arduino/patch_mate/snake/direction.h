
#pragma once

#include "pos.h"

namespace snake {

struct Direction {
    enum {
        LEFT,
        UP,
        RIGHT,
        DOWN,
        MAX
    };

    void move(Pos& p) {
        // Calculate the movement
        switch (v) {
            case LEFT:  --p.x; break;
            case UP:    --p.y; break;
            case RIGHT: ++p.x; break;
            case DOWN:  ++p.y; break;
        }
    }

    void turn_left() { v = (v + MAX - 1) % MAX; }
    void turn_right() { v = (v + 1) % MAX; }

    Direction& operator=(uint8_t d) { v = d; return *this; }

private:
    uint8_t v;
};

}
