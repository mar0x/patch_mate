
#pragma once

#include "../lcd.h"

namespace hero {

struct Graphic {
    void setup()
    {
        static uint8_t graphics[] = {
            // empty
            B00000,
            B00000,
            B00000,
            B00000,
            B00000,
            B00000,
            B00000,
            B00000,
            // Run position 1
            B01100,
            B01100,
            B00000,
            B01110,
            B11100,
            B01100,
            B11010,
            B10011,
            // Run position 2
            B01100,
            B01100,
            B00000,
            B01100,
            B01100,
            B01100,
            B01100,
            B01110,
            // Jump
            B01100,
            B01100,
            B00000,
            B11110,
            B01101,
            B11111,
            B10000,
            B00000,
            // Jump lower
            B11110,
            B01101,
            B11111,
            B10000,
            B00000,
            B00000,
            B00000,
            B00000,
            // Ground
            B11111,
            B11111,
            B11111,
            B11111,
            B11111,
            B11111,
            B11111,
            B11111,
            // Ground right
            B00011,
            B00011,
            B00011,
            B00011,
            B00011,
            B00011,
            B00011,
            B00011,
            // Ground left
            B11000,
            B11000,
            B11000,
            B11000,
            B11000,
            B11000,
            B11000,
            B11000,
        };

        for (uint8_t i = 0; i < 8; ++i) {
            lcd.createChar(i, graphics + i * 8);
        }

        delay(1); // Wait for the CGRAM to be written
    }
};

}
