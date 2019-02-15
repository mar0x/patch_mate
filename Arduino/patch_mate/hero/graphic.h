
#pragma once

namespace hero {

struct Graphic {
    void setup()
    {
        static uint8_t graphics[] = {
            // empty
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            // Run position 1
            0b01100,
            0b01100,
            0b00000,
            0b01110,
            0b11100,
            0b01100,
            0b11010,
            0b10011,
            // Run position 2
            0b01100,
            0b01100,
            0b00000,
            0b01100,
            0b01100,
            0b01100,
            0b01100,
            0b01110,
            // Jump
            0b01100,
            0b01100,
            0b00000,
            0b11110,
            0b01101,
            0b11111,
            0b10000,
            0b00000,
            // Jump lower
            0b11110,
            0b01101,
            0b11111,
            0b10000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            // Ground
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            // Ground right
            0b00011,
            0b00011,
            0b00011,
            0b00011,
            0b00011,
            0b00011,
            0b00011,
            0b00011,
            // Ground left
            0b11000,
            0b11000,
            0b11000,
            0b11000,
            0b11000,
            0b11000,
            0b11000,
            0b11000,
        };

        for (uint8_t i = 0; i < 8; ++i) {
            lcd.createChar(i, graphics + i * 8);
        }

        delay(1); // Wait for the CGRAM to be written
    }
};

}
