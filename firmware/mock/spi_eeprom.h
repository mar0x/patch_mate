
#pragma once

#include "eeprom_t.h"

struct spi_eeprom_traits {
    static constexpr uint16_t size() { return 0x2000; }

    static constexpr const char *name() { return "spi_eeprom.bin"; }
};

using spi_eeprom = eeprom_t<spi_eeprom_traits>;

