
#pragma once

#include "pos.h"
#include "graphic.h"

namespace snake {

struct Snake {
    const Pos& head() const { return body[0]; }
    Pos& head() { return body[0]; }

    const Pos& operator[](uint8_t i) const { return body[i]; }
    Pos& operator[](uint8_t i) { return body[i]; }

    void reset() {
        body[0].set(3, 1);
        body[1].set(2, 1);
        body[2].set(1, 1);
        body[3].set(0, 1);
        length = 4;
    }

    Pos body[Graphic::HEIGHT * Graphic::WIDTH]; // first element is the head.
    uint8_t length = 0;
};

}
