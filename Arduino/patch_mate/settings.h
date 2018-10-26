#pragma once

#include <EEPROM.h>

#include "config.h"
#include "debug.h"

namespace patch_mate {

class settings {
    static int ee_start_;

public:
    static int setup(int s);

    uint8_t midi_channel = 0;
    uint8_t midi_loop_ctrl_in[8] = { 255, 255, 255, 255, 255, 255, 255, 255 };
    uint8_t midi_loop_ctrl_out[8] = { 255, 255, 255, 255, 255, 255, 255, 255 };
    uint8_t midi_prog_chg_out = 0;
    uint8_t usb_debug = 0;
    uint16_t mute_delay_ms = 100;
    uint8_t hide_cursor_delay_s = 5;

    void read();
    void write() const;

    void write(const uint8_t& v) const;
    void write(const uint16_t& v) const;
};

int settings::ee_start_ = 0;

inline int
settings::setup(int s)
{
    ee_start_ = s;
    return s + sizeof(settings);
}

inline void
settings::read()
{
    EEPROM.get(ee_start_, *this);
}

inline void
settings::write() const
{
    EEPROM.put(ee_start_, *this);
}

inline void
settings::write(const uint8_t& v) const
{
    int off = &v - (const uint8_t *) this;
    EEPROM.put(ee_start_ + off, v);
}

inline void
settings::write(const uint16_t& v) const
{
    int off = (const uint8_t *) &v - (const uint8_t *) this;
    EEPROM.put(ee_start_ + off, v);
}

}
