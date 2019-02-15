
#pragma once

#include <HardwareSerial.h>

#include "artl/digital_out.h"

#include "artl/bit_order.h"
#include "artl/sipo.h"

#include "config.h"
#include "debug.h"

namespace patch_mate {

struct out {
    void setup();

    void loop_relay(uint8_t i, bool v);
    void loop_led(uint8_t i, bool v);
    void loop_led(uint8_t mask);
    void loop(uint8_t i, bool v);
    void store_led(bool v);

    void mute();
    void unmute();

    void commit();
    void commit_led();

    bool relay_changed() const;

    static constexpr
    HardwareSerial& midi() {
#if defined(HAVE_HWSERIAL1)
        return Serial1;
#else
        return Serial;
#endif
    }

private:
    /* 1x 74HC595 pinout

      pin  msb  lsb  desc
      --------------------
       Q0   0    7   -
       Q1   1    6   LOOP 1 (0) LED
       Q2   2    5   LOOP 2 (1) LED
       Q3   3    4   LOOP 1 (0) RELAY
       Q4   4    3   LOOP 2 (1) RELAY
       Q5   5    2   -
       Q6   6    1   -
       Q7   7    0   -

       DS            A3
      ST_CP          16 (micro) / 11 (pro mini)
      SH_CP          A2

     */

    enum {
        LED_START   = 1,
        LED_MASK    = 0b00000110,
        RELAY_START = 3,
        RELAY_MASK  = 0b00011000,
    };

    using mute_out = artl::digital_out<4>;
    using store_out = artl::digital_out<artl::pin::id::micro_15>; // or mini_13

    using data = artl::digital_out<A3>;
    using latch = artl::digital_out<artl::pin::id::micro_16>; // or mini_11
    using clock = artl::digital_out<A2>;

    using sipo_t = uint16_t;
    using sipo = artl::sipo<sipo_t, artl::msb_first, data, latch, clock>;

    sipo_t current_ = 0;
    sipo_t new_ = 0;
};


inline void
out::setup()
{
    mute_out().setup();
    store_out().setup();
    sipo().setup();

    data().setup();
    sipo().write(current_);
}

inline void
out::loop_relay(uint8_t i, bool v)
{
    if (i >= MAX_LOOP) return;

    sipo_t m = 1;
    m = (m << (i + RELAY_START));

    if (v) {
        new_ |= m;
    } else {
        new_ &= ~m;
    }
}

inline void
out::loop_led(uint8_t i, bool v)
{
    if (i >= MAX_LOOP) return;

    sipo_t m = 1;
    m = (m << (i + LED_START));

    if (v) {
        new_ |= m;
    } else {
        new_ &= ~m;
    }
}

inline void
out::loop_led(uint8_t mask)
{
    sipo_t m = mask;

    new_ = (new_ & ~LED_MASK) | ((m << LED_START) & LED_MASK);
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
    store_out().write(v);
}

inline void
out::mute()
{
    debug(7, "mute");

    mute_out().low();
}

inline void
out::unmute()
{
    debug(7, "unmute");

    mute_out().high();
}

inline bool
out::relay_changed() const
{
    return ((new_ ^ current_) & RELAY_MASK) != 0;
}

inline void
out::commit()
{
    if (new_ == current_) return;

    debug(7, "out commit: ", current_, " -> ", new_);

    data().setup();
    sipo().write(new_);
    current_ = new_;
}

inline void
out::commit_led()
{
    sipo_t new_led = (new_ & ~RELAY_MASK) | (current_ & RELAY_MASK);
    if (new_led == current_) return;

    debug(7, "out commit_led: ", current_, " -> ", new_led);

    data().setup();
    sipo().write(new_led);
    current_ = new_led;
}

}
