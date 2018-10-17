
#pragma once

#include "../lcd.h"

namespace truck_lane {

struct Graphic {
    void setup
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
            // 1: car up
            B00000,
            B01110,
            B11111,
            B01010,
            B00000,
            B00000,
            B00000,
            B00000,
            // 2: car down
            B00000,
            B00000,
            B00000,
            B00000,
            B01110,
            B11111,
            B01010,
            B00000,
            // 3: truck up
            B00000,
            B11110,
            B11111,
            B01010,
            B00000,
            B00000,
            B00000,
            B00000,
            // 4: truck down
            B00000,
            B00000,
            B00000,
            B00000,
            B11110,
            B11111,
            B01010,
            B00000,
            // 5: crash up
            B10101,
            B01110,
            B01110,
            B10101,
            B00000,
            B00000,
            B00000,
            B00000,
            // 6: crash down
            B00000,
            B00000,
            B00000,
            B10101,
            B01110,
            B01110,
            B10101,
            B00000,
        };

        for (uint8_t i = 0; i < 8; ++i) {
            lcd.createChar(i, graphics + i * 8);
        }

        delay(1); // Wait for the CGRAM to be written
    }
};

}
