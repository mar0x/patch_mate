
#pragma once

namespace snake {

struct Pos {
    bool operator ==(const Pos& p) const {
        return x == p.x && y == p.y;
    }

    void set(uint8_t x_, uint8_t y_) {
        x = x_;
        y = y_;
    }

    uint8_t x:5;
    uint8_t y:3;
};

}
