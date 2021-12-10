
#pragma once

#include "../artl/digital_out.h"
#include "../artl/bit_order.h"
#include "../artl/sipo.h"

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
    /* 2x 74HC595 pinout
                 +8
      pin  msb  lsb  desc
      --------------------
       Q0   0    7   LOOP 1 (0) RELAY
       Q1   1    6   LOOP 2 (1) RELAY
       Q2   2    5   LOOP 3 (2) RELAY
       Q3   3    4   LOOP 4 (3) RELAY
       Q4   4    3   LOOP 5 (4) RELAY
       Q5   5    2   LOOP 6 (5) RELAY
       Q6   6    1   LOOP 7 (6) RELAY
       Q7   7    0   LOOP 8 (7) RELAY

            +8
      pin  msb  lsb  desc
      --------------------
       Q0   0    7   LOOP 1 (0) LED
       Q1   1    6   LOOP 2 (1) LED
       Q2   2    5   LOOP 3 (2) LED
       Q3   3    4   LOOP 4 (3) LED
       Q4   4    3   LOOP 5 (4) LED
       Q5   5    2   LOOP 6 (5) LED
       Q6   6    1   LOOP 7 (6) LED
       Q7   7    0   LOOP 8 (7) LED

       DS            A2
      ST_CP          10
      SH_CP          A1

     */

    enum {
        LED_START   = 8,
        LED_MASK    = 0xFF00U,
        RELAY_START = 0,
        RELAY_MASK  = 0x00FFU,
    };

    using mute_out = artl::digital_out< artl::port::B, 4 >;
    using store_out = artl::digital_out< artl::port::F, 4 >;

    using data = artl::digital_out< artl::port::F, 5 >;
    using latch = artl::digital_out< artl::port::B, 6 >;
    using clock = artl::digital_out< artl::port::F, 6 >;

    using sipo_t = uint16_t;
    using sipo = artl::sipo<sipo_t, artl::msb_first, data, latch, clock>;

    sipo_t led_;
    sipo_t rel_;
    sipo_t new_led_;
    sipo_t new_rel_;

    void commit(uint16_t led, uint16_t rel);
};


inline void
out::setup()
{
    serial_out::setup();

    mute_out::setup();
    store_out::setup();
    sipo::setup();

    data::setup();
}

inline void
out::loop_relay(uint8_t i, bool v)
{
    if (i >= MAX_LOOP) return;

    sipo_t m = 1;
    m = (m << i);

    if (v) {
        new_rel_ |= m;
    } else {
        new_rel_ &= ~m;
    }
}

inline void
out::loop_led(uint8_t i, bool v)
{
    if (i >= MAX_LOOP) return;

    sipo_t m = 1;
    m = (m << i);

    if (v) {
        new_led_ |= m;
    } else {
        new_led_ &= ~m;
    }
}

inline void
out::loop_led(uint16_t mask)
{
    new_led_ = mask;
}

inline void
out::loop(uint8_t i, bool v)
{
    loop_led(i, v);
    loop_relay(i, v);
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

    commit(new_led_, new_rel_);
}

inline void
out::commit(uint16_t led, uint16_t rel)
{
    led_ = led;
    rel_ = rel;

    data::setup();

    sipo_t v = (led_ << LED_START) | (rel_ << RELAY_START);
    sipo::write(v);
}

inline void
out::commit_led()
{
    if (new_led_ == led_) return;

    debug(7, "out commit_led: ", led_, " -> ", new_led_);

    commit(new_led_, rel_);
}

}
