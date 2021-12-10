
#pragma once

#include "eeprom_t.h"

struct eeprom_traits {
    static constexpr uint16_t size() { return 1024; }

    static constexpr const char *name() { return "eeprom.bin"; }
};

using eeprom = eeprom_t<eeprom_traits>;

