
#pragma once

namespace patch_mate {

struct serial_num_t {
    char buf[4];

    operator char*() { return buf; }
    operator const char*() const { return buf; }

    void get() {
        get(buf);
    }

    static void get(char *b) {
        EEPROM.get(EEPROM.length() - sizeof(buf), b);
    }

    void put() const {
        EEPROM.put(EEPROM.length() - sizeof(buf), buf);
    }
};

}
