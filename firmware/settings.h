#pragma once

#include "config.h"
#include "debug.h"

namespace patch_mate {

class settings_t {
    static int ee_start_;

public:
    enum {
        MIDI_OUT_PROG = 0x01,
        MIDI_OUT_ALL = 0x02,
    };

    static int setup(int s);

    uint8_t midi_channel = 0;
    uint8_t midi_loop_ctrl_in[10] = { 128, 128, 128, 128, 128, 128, 128, 128, 128, 128 };
    uint8_t midi_loop_ctrl_out[10] = { 128, 128, 128, 128, 128, 128, 128, 128, 128, 128 };
    uint8_t midi_bypass_ctrl_in = 128;
    uint8_t midi_bypass_ctrl_out = 128;
    uint8_t midi_out_prog = 0;
    uint8_t midi_fwd = 0;
    uint8_t usb_debug = 0;
    uint16_t mute_delay_ms = 100;
    uint8_t hide_cursor_delay_s = 5;

    void read();
    void write() const;

    void write(const uint8_t& v) const;
    void write(const uint16_t& v) const;
};

int settings_t::ee_start_ = 0;

inline int
settings_t::setup(int s)
{
    ee_start_ = s;
    return s + sizeof(settings_t);
}

inline void
settings_t::read()
{
    EEPROM.get(ee_start_, *this);
}

inline void
settings_t::write() const
{
    EEPROM.put(ee_start_, *this);
}

inline void
settings_t::write(const uint8_t& v) const
{
    int off = &v - (const uint8_t *) this;
    EEPROM.put(ee_start_ + off, v);
}

inline void
settings_t::write(const uint16_t& v) const
{
    int off = (const uint8_t *) &v - (const uint8_t *) this;
    EEPROM.put(ee_start_ + off, v);
}

}
