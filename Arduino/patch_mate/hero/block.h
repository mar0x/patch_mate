
#pragma once

#include <stdlib.h>

namespace hero {

struct Block {
    enum {
        MAX_LENGTH = LCD_COLUMNS * 2,
    };

    enum {
        EMPTY = ' ',
        SOLID = 5,
        SOLID_RIGHT = 6,
        SOLID_LEFT = 7,
    };

    void generate() {
        row_ = (random(3) == 0) ? 0 : 1;
        length_ = 2 + random(10);
        start_ = MAX_LENGTH;
    }

    void step() {
        if (start_ == 0) {
            --length_;
        } else {
            --start_;

            uint8_t col = start_ / 2;
            uint8_t c = (start_ % 2) ? SOLID_RIGHT : SOLID;

            lcd_buf[row_][col] = c;
            lcd_write(col, row_, c);
        }

        uint8_t end = start_ + length_;
        if ( end < MAX_LENGTH - 1) {
            uint8_t col = end / 2;
            uint8_t c = (end % 2) ? SOLID_LEFT : EMPTY;

            lcd_buf[row_][col] = c;
            lcd_write(col, row_, c);
        }
    }

    uint8_t row_:2;
    uint8_t length_:6;
    uint8_t start_;
};

}
