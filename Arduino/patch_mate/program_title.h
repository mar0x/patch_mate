
#pragma once

#include <EEPROM.h>

#include "debug.h"

namespace patch_mate {

class program_title {
    static uint16_t ee_start_;
    static uint16_t ee_end_;

    constexpr static uint16_t title_size = 15;
    constexpr static uint16_t ee_title_size = title_size + 1;

public:
    static void setup(uint16_t ee_start);

    void reset();
    bool empty() const;

    bool operator==(const program_title& n) const;
    bool operator!=(const program_title& n) const;

    uint16_t ee_find(uint8_t prog);

    bool read(uint8_t prog);
    bool write(uint8_t prog);

    uint16_t idx = 0;
    char title[title_size] =
        { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
    char zero = 0;
};

uint16_t program_title::ee_start_ = 0;
uint16_t program_title::ee_end_ = 0;

inline void
program_title::setup(uint16_t ee_start) {
    ee_start_ = ee_start;
    ee_end_ = EEPROM.length() - ee_title_size;

    debug(7, "program_title::setup: start ", ee_start_);

    for (uint16_t i = ee_start_; i < ee_end_; i += ee_title_size) {

        uint8_t p = EEPROM.read(i);
        if (p == 0) {
            ee_end_ = i;
            break;
        }
        debug(7, "program_title::setup: p=", p);
    }

    debug(7, "program_title::setup: end ", ee_end_);
}

inline void
program_title::reset() {
    idx = 0;
    memset(title, ' ', title_size);
}

inline bool
program_title::empty() const {
    return *this == program_title();
}

inline bool
program_title::operator==(const program_title& n) const {
    return memcmp(title, n.title, title_size) == 0;
}

inline bool
program_title::operator!=(const program_title& n) const {
    return memcmp(title, n.title, title_size) != 0;
}

inline uint16_t
program_title::ee_find(uint8_t prog) {
    if (ee_start_ == 0) {
        return 0;
    }

    for (uint16_t i = ee_start_; i < ee_end_; i += ee_title_size) {

        uint8_t p = EEPROM.read(i);
        if (p - 1 == prog) {
            debug(8, "ee_find: ", i);

            return i;
        }
    }

    return 0;
}

inline bool
program_title::read(uint8_t prog) {
    uint16_t i = ee_find(prog);

    if (i > 0) {
        idx = i;
        EEPROM.get(i + 1, title);
        return true;
    }

    reset();
    return false;
}

inline bool
program_title::write(uint8_t prog) {
    if (ee_start_ == 0) {
        return false;
    }

    if (empty()) {
        if (idx > 0) {
            EEPROM.update(idx, 0xFFU);
            idx = 0;
        }

        return true;
    }

    if (idx == 0) {
        for (uint16_t i = ee_start_; i < ee_end_; i += ee_title_size) {

            uint8_t p = EEPROM.read(i);
            if (p == 0) { break; }

            if (p == 0xFFU) {
                idx = i;
                break;
            }
        }
    }

    debug(7, "program_title::write: prog ", prog, " idx ", idx);

    if (idx == 0) {
        if (ee_end_ <= EEPROM.length() - 2 * ee_title_size) {
            idx = ee_end_;
            ee_end_ += ee_title_size;
            EEPROM.update(ee_end_, 0);
        }
    }

    if (idx) {
        EEPROM.update(idx, prog + 1);
        for (uint16_t i = 0; i < title_size; i++) {
            EEPROM.update(idx + i + 1, title[i]);
        }

        return true;
    }

    return false;
}

}
