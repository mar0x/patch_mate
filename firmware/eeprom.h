
#pragma once

#include <inttypes.h>
#include <avr/eeprom.h>
#include <avr/io.h>

struct eeprom {
    static void setup() { }

    static constexpr uint16_t length() { return E2END + 1; }

    static uint8_t read(uint16_t addr) {
        return eeprom_read_byte( (uint8_t *) addr );
    }

    static void write(uint16_t addr, uint8_t v) {
        eeprom_write_byte( (uint8_t *) addr, v );
    }

    static void update(uint16_t addr, uint8_t v) {
        eeprom_update_byte( (uint8_t *) addr, v );
    }

    template<typename T>
    static void get(uint16_t addr, T &t) {
        uint8_t *p = (uint8_t *) &t;

        for (uint8_t i = sizeof(t); i != 0; --i, ++addr, ++p) {
            *p = read(addr);
        }
    }

    template<typename T>
    static void put(uint16_t addr, const T &t) {
        uint8_t *p = (uint8_t *) &t;

        for (uint8_t i = sizeof(t); i != 0; --i, ++addr, ++p) {
            update(addr, *p);
        }
    }
};
