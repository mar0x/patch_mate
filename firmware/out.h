
#pragma once

#include <SPI.h>

#include "artl/digital_out.h"

#include "config.h"
#include "debug.h"
#include "usb_midi.h"
#include "midi_cmd.h"

namespace patch_mate {

struct out {
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

#if 0
#if defined(DEBUG)
    template<typename T1>
    static void debug_println(const T1& a1) {
      if (Serial.dtr()) {
        Serial.println(a1);
      }
    }

    template<typename T1, typename ...Args>
    static void debug_println(const T1& a1, Args... args) {
      if (Serial.dtr()) {
        Serial.print(a1);
        debug_println(args...);
      }
    }
#else
    template<typename ...Args>
    static void debug_println(Args... args) { }
#endif
#endif

    template<typename T1>
    void serial_print(const T1& a1) {
      if (Serial.dtr()) {
        Serial.print(a1);
      }
    }

    template<typename T1, typename ...Args>
    void serial_print(const T1& a1, Args... args) {
      if (Serial.dtr()) {
        Serial.print(a1);
        serial_print(args...);
      }
    }

    template<typename T1>
    void serial_println(const T1& a1) {
      if (Serial.dtr()) {
        Serial.println(a1);
      }
    }

    template<typename T1, typename ...Args>
    void serial_println(const T1& a1, Args... args) {
      if (Serial.dtr()) {
        Serial.print(a1);
        serial_println(args...);
      }
    }

private:
    /* 3x 74HC595 pinout
           +16
      pin  msb  lsb  desc
      --------------------
       Q0   0    7   NC
       Q1   1    6   LOOP 1 (0) LED
       Q2   2    5   LOOP 2 (1) LED
       Q3   3    4   LOOP 3 (2) LED
       Q4   4    3   LOOP 4 (3) LED
       Q5   5    2   LOOP 5 (4) LED
       Q6   6    1   LOOP 6 (5) LED
       Q7   7    0   LOOP 7 (6) LED

           +8    +8
      pin  msb  lsb  desc
      --------------------
       Q0   0    7   LOOP 8 (7) LED
       Q1   1    6   LOOP 4 (3) RELAY
       Q2   2    5   LOOP 3 (2) RELAY
       Q3   3    4   LOOP 2 (1) RELAY
       Q4   4    3   LOOP 1 (0) RELAY
       Q5   5    2   LOOP 10 (9) RELAY
       Q6   6    1   LOOP 9 (8) RELAY
       Q7   7    0   LOOP 8 (7) RELAY

                +16
      pin  msb  lsb  desc
      --------------------
       Q0   0    7   LOOP 9 (8) LED
       Q1   1    6   LOOP 10 (9) LED
       Q2   2    5   NC
       Q3   3    4   NC
       Q4   4    3   NC
       Q5   5    2   LOOP 7 (6) RELAY
       Q6   6    1   LOOP 6 (5) RELAY
       Q7   7    0   LOOP 5 (4) RELAY
     */

    enum hi_mask {
        LED8 = 1 << 0,
        LED9 = 1 << 1,
        REL6 = 1 << 5,
        REL5 = 1 << 6,
        REL4 = 1 << 7,
    };

    enum mid_mask {
        LED7 = 1 << 0,
        REL3 = 1 << 1,
        REL2 = 1 << 2,
        REL1 = 1 << 3,
        REL0 = 1 << 4,
        REL9 = 1 << 5,
        REL8 = 1 << 6,
        REL7 = 1 << 7,
    };

    enum lo_mask {
        LED0 = 1 << 1,
        LED1 = 1 << 2,
        LED2 = 1 << 3,
        LED3 = 1 << 4,
        LED4 = 1 << 5,
        LED5 = 1 << 6,
        LED6 = 1 << 7,
    };

    using mute_out = artl::digital_out< artl::port::F, 0 >;
    using store_out = artl::digital_out< artl::port::C, 7 >;
    using data_out = artl::digital_out< artl::port::D, 7 >;
    using rel_cs = artl::digital_out< artl::port::B, 7 >;

    void commit(uint16_t led, uint16_t rel);

    uint16_t led_;
    uint16_t rel_;
    uint16_t new_led_;
    uint16_t new_rel_;
};


inline void
out::setup()
{
#if defined(DEBUG)
    Serial.begin(115200);
#endif

    mute_out::setup();
    store_out::setup();
    data_out::setup();

    rel_cs::setup();
    rel_cs::high();
}

inline void
out::loop_relay(uint8_t i, bool v)
{
    if (i >= MAX_LOOP) return;

    uint16_t m = 1;
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

    uint16_t m = 1;
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
out::data_led(bool v)
{
    data_out::write(v);
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

    uint8_t hi = 0;
    uint8_t mid = 0;
    uint8_t lo = 0;

    if (led_ & (1 << 0)) lo  |= LED0;
    if (led_ & (1 << 1)) lo  |= LED1;
    if (led_ & (1 << 2)) lo  |= LED2;
    if (led_ & (1 << 3)) lo  |= LED3;
    if (led_ & (1 << 4)) lo  |= LED4;
    if (led_ & (1 << 5)) lo  |= LED5;
    if (led_ & (1 << 6)) lo  |= LED6;
    if (led_ & (1 << 7)) mid |= LED7;
    if (led_ & (1 << 8)) hi  |= LED8;
    if (led_ & (1 << 9)) hi  |= LED9;

    if (rel_ & (1 << 0)) mid |= REL0;
    if (rel_ & (1 << 1)) mid |= REL1;
    if (rel_ & (1 << 2)) mid |= REL2;
    if (rel_ & (1 << 3)) mid |= REL3;
    if (rel_ & (1 << 4)) hi  |= REL4;
    if (rel_ & (1 << 5)) hi  |= REL5;
    if (rel_ & (1 << 6)) hi  |= REL6;
    if (rel_ & (1 << 7)) mid |= REL7;
    if (rel_ & (1 << 8)) mid |= REL8;
    if (rel_ & (1 << 9)) mid |= REL9;

    rel_cs::low();

    SPI.transfer(hi);
    SPI.transfer(mid);
    SPI.transfer(lo);

    rel_cs::high();
}

inline void
out::commit_led()
{
    if (new_led_ == led_) return;

    debug(7, "out commit_led: ", led_, " -> ", new_led_);

    commit(new_led_, rel_);
}

}
