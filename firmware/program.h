
#pragma once

#include "debug.h"
#include "config.h"

namespace patch_mate {

class program_t {
    static uint16_t ee_start_;
    static uint16_t ee_end_;

    enum {
        TITLE_SIZE = 12,
    };

public:
    class loop_t {
    public:
        loop_t(uint16_t v = 0) : value_(v) { }

        void set(uint8_t i, bool s);
        bool toggle(uint8_t i);

        bool operator[](uint8_t i) const;
        operator uint16_t() const { return value_; }
        bool operator==(const loop_t& s) const { return value_ == s.value_; }
        bool operator!=(const loop_t& s) const { return value_ != s.value_; }

        void reset() { value_ = 0; }

    private:
        uint16_t value_ = 0;
    };

    static uint16_t setup(uint16_t ee_start);

    void reset();
    bool empty() const;

    bool operator==(const program_t& n) const;
    bool operator!=(const program_t& n) const;

    bool read(uint8_t prog);
    bool write(uint8_t prog);

    char title[TITLE_SIZE] =
        { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
    uint16_t zero = 0;
    loop_t loop;
};

uint16_t program_t::ee_start_ = 0;
uint16_t program_t::ee_end_ = 0;

inline void
program_t::loop_t::set(uint8_t i, bool s) {
    if (s) {
        value_ |= (1 << i);
    } else {
        value_ &= ~(1 << i);
    }
}

inline bool
program_t::loop_t::toggle(uint8_t i) {
    uint8_t m = (1 << i);

    return (value_ ^= m) & m;
}

inline bool
program_t::loop_t::operator[](uint8_t i) const {
    return (value_ & (1 << i)) != 0;
}

inline uint16_t
program_t::setup(uint16_t ee_start) {
    ee_start_ = ee_start;
    if (ee_start_ > 0) ee_start_ = 16;

    ee_end_ = ee_start_ + sizeof(program_t) * MAX_PROGRAMS;

    debug(7, "program_t::setup: start ", ee_start_);

    return ee_end_;
}

inline void
program_t::reset() {
    for (uint8_t i = 0; i < TITLE_SIZE; ++i) {
        title[i] = ' ';
    }
    loop.reset();
}

inline bool
program_t::empty() const {
    return *this == program_t();
}

inline bool
program_t::operator==(const program_t& n) const {
    return memcmp(title, n.title, sizeof(title)) == 0 &&
      loop == n.loop;
}

inline bool
program_t::operator!=(const program_t& n) const {
    return memcmp(title, n.title, sizeof(title)) != 0 ||
      loop != n.loop;
}

inline bool
program_t::read(uint8_t prog) {
    debug(7, "program_t::read: prog ", prog);

    uint16_t addr = ee_start_ + sizeof(program_t) * prog;
    spi_eeprom::get(addr, *this);

    return true;
}

inline bool
program_t::write(uint8_t prog) {
    if (ee_start_ == 0) {
        return false;
    }

    debug(7, "program_t::write: prog ", prog);

    uint16_t addr = ee_start_ + sizeof(program_t) * prog;
    spi_eeprom::put(addr, *this);

    return true;
}

}
