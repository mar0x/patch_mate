
#pragma once

#include "serial_num.h"

namespace patch_mate {

struct hardware_id_t {
    char buf[10];

    operator char*() { return buf; }
    operator const char*() const { return buf; }

    void get() {
        EEPROM.get(EEPROM.length() - sizeof(buf) - sizeof(serial_num_t), buf);
    }

    void put() const {
        EEPROM.put(EEPROM.length() - sizeof(buf) - sizeof(serial_num_t), buf);
    }
};

}
