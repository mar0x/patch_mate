
#pragma once

#include <SPI.h>

#include "artl/button.h"
#include "artl/encoder.h"
#include "artl/digital_in.h"
#include "artl/digital_out.h"
#include "artl/pin_change_int.h"

#include "config.h"

namespace patch_mate {

template<typename T>
struct in {
    using traits = T;

    in();

    void setup();
    void update(unsigned long t, bool slow);

    bool midi_available() const { return Serial1.available(); }
    int midi_read() const { return Serial1.read(); }

    bool serial_available() const { return Serial.available(); }
    int serial_read() const { return Serial.read(); }

    using button_type = artl::button<typename traits::button_handler>;

    const button_type& loop(uint8_t i) const { return loop_[i]; }
    const button_type& left() const { return left_; }
    const button_type& right() const { return right_; }
    const button_type& store() const { return store_; }

private:
    /* 2x 74HC165 pinout
                  +8 
      pin        msb  lsb  desc
      --------------------
       A/D0/11    0    7   LOOP 1 (0)
       B/D1/12    1    6   LOOP 2 (1)
       C/D2/13    2    5   GND
       D/D3/14    3    4   GND
       E/D4/ 3    4    3   LOOP 6 (5)
       F/D5/ 4    5    2   LOOP 5 (4)
       G/D6/ 5    6    1   LOOP 4 (3)
       H/D7/ 6    7    0   LOOP 3 (2)

                       +8
      pin        msb  lsb  desc
      --------------------
       A/D0/11    0    7   GND
       B/D1/12    1    6   GND
       C/D2/13    2    5   GND
       D/D3/14    3    4   GND
       E/D4/ 3    4    3   LOOP 10 (9)
       F/D5/ 4    5    2   LOOP 9 (8)
       G/D6/ 5    6    1   LOOP 8 (7)
       H/D7/ 6    7    0   LOOP 7 (6)

     */
    enum hi_mask {
        LOOP0 = 1 << 0,
        LOOP1 = 1 << 1,
        LOOP2 = 1 << 7,
        LOOP3 = 1 << 6,
        LOOP4 = 1 << 5,
        LOOP5 = 1 << 4,
    };

    enum lo_lask {
        LOOP6 = 1 << 7,
        LOOP7 = 1 << 6,
        LOOP8 = 1 << 5,
        LOOP9 = 1 << 4,
    };

    enum {
        enc_a_pin = artl::pin::id::pb4,
        enc_b_pin = artl::pin::id::pb5,
    };

    using enc_a_in = artl::digital_in< enc_a_pin >;
    using enc_b_in = artl::digital_in< enc_b_pin >;

    using enc_a_int = artl::pin_change_int< enc_a_pin >;
    using enc_b_int = artl::pin_change_int< enc_a_pin >;

    using encoder_type = artl::encoder<typename traits::encoder_handler>;

    using left_in = artl::digital_in< artl::pin::id::pd4 >;
    using right_in = artl::digital_in< artl::pin::id::pd6 >;
    using store_in = artl::digital_in< artl::pin::id::pf5 >;

    using sw_cs = artl::digital_out< artl::pin::id::pc6 >;
    using ext_cs = artl::digital_out< artl::pin::id::pd1 >;

    button_type loop_[10];
    button_type left_;
    button_type right_;
    button_type store_;
    encoder_type encoder_;
};

template<typename T> inline
in<T>::in() :
    loop_{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
    left_(traits::LEFT),
    right_(traits::RIGHT),
    store_(traits::STORE)
{ }

template<typename T> inline void
in<T>::setup()
{
    enc_a_in::setup();
    enc_b_in::setup();

    left_in::setup();
    right_in::setup();
    store_in::setup();

    sw_cs::setup();
    sw_cs::high();

    ext_cs::setup();
    ext_cs::high();

    Serial1.begin(31250);

    enc_a_int::enable();
    enc_b_int::enable();
}

template<typename T> inline void
in<T>::update(unsigned long t, bool slow)
{
    encoder_.update(!enc_a_in::read(), !enc_b_in::read(), t);

    if (!slow) return;

    static uint8_t last_hi = 0;
    static uint8_t last_lo = 0;

    store_.update(!store_in::read(), t);
    left_.update(!left_in::read(), t);
    right_.update(!right_in::read(), t);

    sw_cs::low();
    sw_cs::high();

    ext_cs::low();

    uint8_t hi = SPI.transfer(0);
    uint8_t lo = SPI.transfer(0);

    ext_cs::high();

    if (last_hi != hi || last_lo != lo) {
        debug(7, "in::update: ", hi, ":", lo);
        last_hi = hi;
        last_lo = lo;
    }

    loop_[0].update((hi & LOOP0) == 0, t);
    loop_[1].update((hi & LOOP1) == 0, t);
    loop_[2].update((hi & LOOP2) == 0, t);
    loop_[3].update((hi & LOOP3) == 0, t);
    loop_[4].update((hi & LOOP4) == 0, t);
    loop_[5].update((hi & LOOP5) == 0, t);
    loop_[6].update((lo & LOOP6) == 0, t);
    loop_[7].update((lo & LOOP7) == 0, t);
    loop_[8].update((lo & LOOP8) == 0, t);
    loop_[9].update((lo & LOOP9) == 0, t);
}

}
