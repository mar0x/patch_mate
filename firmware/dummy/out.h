
#pragma once

#include "../artl/digital_out.h"

#include "../config.h"
#include "../debug.h"
#include "../usb_midi.h"
#include "../midi_cmd.h"

#include "../serial_out.h"

namespace patch_mate {

struct out : public serial_out {
    void setup();

    void loop_relay(uint8_t i, bool v);
    void loop_led(uint8_t i, bool v);
    void loop_led(uint16_t mask);
    void loop(uint8_t i, bool v);
    void store_led(bool v);
    void data_led(bool v);

    void mute();
    void unmute();

    void commit();
    void commit_led();

    bool relay_changed() const;

    void midi_write(uint8_t b) { Serial1.write(&b, 1); }
    void usb_midi_write(uint8_t b, uint8_t jack);
    void usb_midi_write(const midi_cmd_t& c, uint8_t jack);

private:
    using mute_out = artl::digital_out< artl::port::B, 4 >;
    using store_out = artl::digital_out< artl::port::C, 7 >;

    using sipo_t = uint16_t;

    sipo_t led_;
    sipo_t rel_;
    sipo_t new_led_;
    sipo_t new_rel_;
};


inline void
out::setup()
{
    serial_out::setup();

    mute_out::setup();
    store_out::setup();
}

inline void
out::loop_relay(uint8_t, bool)
{
}

inline void
out::loop_led(uint8_t, bool)
{
}

inline void
out::loop_led(uint16_t)
{
}

inline void
out::loop(uint8_t, bool)
{
}

inline void
out::store_led(bool v)
{
    store_out::write(v);
}

inline void
out::data_led(bool)
{
}

inline void
out::mute()
{
    debug(7, "mute");

    mute_out::low();
}

inline void
out::unmute()
{
    debug(7, "unmute");

    mute_out::high();
}

inline bool
out::relay_changed() const
{
    return new_rel_ != rel_;
}

inline void
out::usb_midi_write(uint8_t b, uint8_t jack)
{
    usb_midi::event_t ev{0, b, 0, 0};

    switch (b) {
    case CMD_SYS_TUNE_REQ:
    case CMD_SYS_EX_END:
        ev.header = 0x5;
        break;

    case CMD_SYS_CLOCK:
    case CMD_SYS_TICK:
    case CMD_SYS_START:
    case CMD_SYS_CONT:
    case CMD_SYS_STOP:
    case CMD_SYS_UNDEF:
    case CMD_SYS_ACTIVE_S:
    case CMD_SYS_RESET:
        ev.header = 0xF;
        break;

    default:
        return;
    }

    ev.header |= (jack << 4);

    usb_midi::port.send(ev);
}

inline void
out::usb_midi_write(const midi_cmd_t& c, uint8_t jack)
{
    usb_midi::event_t ev;

    switch (c.command()) {
    case 0:
    case CMD_SYS_EX:
        if (c.size() == 3) {
            ev.header = (c[2] == CMD_SYS_EX_END) ? 0x07 : 0x04;
        } else {
            ev.header = 0x06;
        }
        break;

    case CMD_NOTE_OFF:
    case CMD_NOTE_ON:
    case CMD_KEY_PRESSURE:
    case CMD_CTRL_CHANGE:
    case CMD_PROG_CHANGE:
    case CMD_CHAN_PRESSURE:
    case CMD_PITCH_CHANGE:
        ev.header = c.command() >> 4;
        break;

    case CMD_SYS_MTC:
    case CMD_SYS_SONG_SEL:
        ev.header = 0x2;
        break;

    case CMD_SYS_SONG_PP:
        ev.header = 0x3;
        break;

    case CMD_SYS_TUNE_REQ:
    case CMD_SYS_EX_END:
        ev.header = 0x5;
        break;

    case CMD_SYS_CLOCK:
    case CMD_SYS_TICK:
    case CMD_SYS_START:
    case CMD_SYS_CONT:
    case CMD_SYS_STOP:
    case CMD_SYS_UNDEF:
    case CMD_SYS_ACTIVE_S:
    case CMD_SYS_RESET:
        ev.header = 0xF;
        break;

    default:
        ev.header = 0;
        break;
    }

    ev.header |= (jack << 4);
    ev.byte1 = c[0];
    ev.byte2 = c.size() > 1 ? c[1] : 0;
    ev.byte3 = c.size() > 2 ? c[2] : 0;

    usb_midi::port.send(ev);
}

inline void
out::commit()
{
    if (new_rel_ == rel_ && new_led_ == led_) return;

    debug(7, "out commit: ", led_, ":", rel_, " -> ", new_led_, ":", new_rel_);

    led_ = new_led_;
    rel_ = new_rel_;
}

inline void
out::commit_led()
{
    if (new_led_ == led_) return;

    debug(7, "out commit_led: ", led_, " -> ", new_led_);

    led_ = new_led_;
}

}
