
#pragma once

#include "pos.h"

namespace snake {

struct Graphic {
    enum {
        WIDTH = LCD_COLUMNS,
        HEIGHT = LCD_LINES * 2,
    };

    enum {
        NONE = 0,
        SNAKE = 1,
        APPLE = 2,
    };

    void clear() {
        lcd_buf_clear();
    }

    void set_item(const Pos& p, uint8_t item) {
        uint8_t x = p.x;
        uint8_t y = p.y / 2;

        uint8_t v = lcd_buf[y][x] & 0x07;
        uint8_t hval = v / 3;
        uint8_t lval = v % 3;

        if (p.y % 2) { lval = item; }
                else { hval = item; }

        lcd_buf[y][x] = hval * 3 + lval;
    }

    void flush() {
        lcd_update();
    }

    void update_item(const Pos& p, uint8_t item) {
        set_item(p, item);

        lcd_update(p.x, p.y / 2);
    }

    void setup()
    {
        /*
        0: 0 0
        1: 0 A
        2: 0 B
        3: A 0
        4: A A
        5: A B
        6: B 0
        7: B A
         */

        const uint8_t blank[] = {
            0b00000,
            0b00000,
            0b00000,
            0b00000,
        };

        const uint8_t block[] = {
            0b01110,
            0b01110,
            0b01110,
            0b00000,
        };

        const uint8_t apple[] = {
            0b00100,
            0b01010,
            0b00100,
            0b00000,
        };

        const uint8_t *bitmap[3] = { blank, block, apple };

        for (uint8_t i = 0; i < 8; ++i) {
            uint8_t glyph[8];
            uint8_t upperIcon = i / 3;
            uint8_t lowerIcon = i % 3;

            memcpy(glyph, bitmap[upperIcon], 4);
            memcpy(glyph + 4, bitmap[lowerIcon], 4);

            lcd.createChar(i, glyph);
        }

        delay(1); // Wait for the CGRAM to be written
    }
};

}
