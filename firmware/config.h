
#pragma once

#include <stdint.h>

#if defined(ARDUINO) && defined(ARDUINO_AVR_PATCHMATE_X)

#define HAVE_SPI_OLED

#endif

namespace patch_mate {

#if !defined(ARDUINO) || defined(ARDUINO_AVR_PATCHMATE_X)
    using loop_value_t = uint16_t;

    enum {
        MAX_LOOP = 10,
        MAX_PROGRAMS = 128,
    };

#define HAVE_SPI_EEPROM

#else
    using loop_value_t = uint8_t;

    enum {
        MAX_LOOP = 8,
        MAX_PROGRAMS = 64,
    };

#endif

    enum {
        MAX_CTRL = 128,
        MAX_CHANNEL = 16,
    };
}
