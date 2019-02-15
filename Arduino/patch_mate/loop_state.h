
#pragma once

#include "config.h"
#include "debug.h"

namespace patch_mate {

class loop_state {
    static int ee_start_;

public:
    static int setup(int ee_start);

    loop_state(uint8_t v = 0) : value_(v) { }

    void set(uint8_t i, bool s);
    bool toggle(uint8_t i);

    bool operator[](uint8_t i) const;
    loop_state& operator=(uint8_t v);
    operator uint8_t() const { return value_; }
    bool operator==(const loop_state& s) const { return value_ == s.value_; }
    bool operator!=(const loop_state& s) const { return value_ != s.value_; }

    bool read(uint8_t prog);
    bool write(uint8_t prog);

    void reset() { value_ = 0; }
private:
    uint8_t value_ = 0;
};


int loop_state::ee_start_ = 0;

inline int
loop_state::setup(int ee_start) {
    ee_start_ = ee_start;
    return ee_start_ + MAX_PROGRAMS;
}

inline void
loop_state::set(uint8_t i, bool s) {
    if (s) {
        value_ |= (1 << i);
    } else {
        value_ &= ~(1 << i);
    }
}

inline bool
loop_state::toggle(uint8_t i) {
    uint8_t m = (1 << i);

    return (value_ ^= m) & m;
}

inline bool
loop_state::operator[](uint8_t i) const {
    return (value_ & (1 << i)) != 0;
}

inline loop_state&
loop_state::operator=(uint8_t v) {
    value_ = v;
    return *this;
}

inline bool
loop_state::read(uint8_t prog) {
    if (ee_start_ == 0 || prog >= MAX_PROGRAMS) {
        reset();
        return false;
    }

    value_ = EEPROM.read(ee_start_ + prog);

    debug(6, "loop_state::read: prog ", prog, " = ", value_);

    return true;
}

inline bool
loop_state::write(uint8_t prog) {
    if (ee_start_ == 0 || prog >= MAX_PROGRAMS) {
        return false;
    }

    debug(6, "loop_state::write: prog ", prog, " = ", value_);

    EEPROM.update(ee_start_ + prog, value_);
    return value_ == EEPROM.read(ee_start_ + prog);
}

}
