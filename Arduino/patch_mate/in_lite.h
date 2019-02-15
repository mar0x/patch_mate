
#pragma once

#include <HardwareSerial.h>

#include "artl/button.h"
#include "artl/encoder.h"

#include "artl/digital_out.h"
#include "artl/digital_in.h"

#include "artl/bit_order.h"
#include "artl/piso.h"
#include "artl/pin_change_int.h"

#include "config.h"

namespace patch_mate {

template<typename T>
struct in {
    using traits = T;

    in();

    void setup();
    void update(unsigned long t, bool slow);

    static constexpr
    HardwareSerial& midi() {
#if defined(HAVE_HWSERIAL1)
        return Serial1;
#else
        return Serial;
#endif
    }

    using button_type = artl::button<typename traits::button_handler>;

    const button_type& loop(uint8_t i) const { return loop_[i]; }
    const button_type& left() const { return left_; }
    const button_type& right() const { return right_; }
    const button_type& store() const { return store_; }

private:
    /* 1x 74HC165 pinout

      pin  msb  lsb  desc
      --------------------
       A    0    7   RIGHT
       B    1    6   NC -> GND
       C    2    5   LOOP 2 (1)
       D    3    4   LOOP 1 (0)
       E    4    3   GND
       F    5    2   GND
       G    6    1   LEFT
       H    7    0   NC -> GND

      QH'            A3
      CLK            A2
      SH/LD          A1

     */
    enum lsb_bits {
        LEFT  = 1,
        RIGHT = 7,
        LOOP0 = 4,
        LOOP1 = 5,
    };

    enum {
        enc_a_pin = 10,
        enc_b_pin = artl::pin::id::micro_14,  // or mini_12
    };

    using store_in = artl::digital_in<A0>;

    using enc_a_in = artl::digital_in< enc_a_pin >;
    using enc_b_in = artl::digital_in< enc_b_pin >;

    using enc_a_int = artl::pin_change_int< enc_a_pin >;
    using enc_b_int = artl::pin_change_int< enc_a_pin >;

    using ld = artl::digital_out<A1>;
    using clk = artl::digital_out<A2>;
    using qh = artl::digital_in<A3>;

    using piso_t = uint16_t;
    using piso = artl::piso<piso_t, artl::lsb_first, qh, ld, clk>;

    using encoder_type = artl::encoder<typename traits::encoder_handler>;

    piso_t val_;

    button_type loop_[8];
    button_type left_;
    button_type right_;
    button_type store_;
    encoder_type encoder_;
};

template<typename T> inline
in<T>::in() :
    loop_{ 0, 1, 2, 3, 4, 5, 6, 7 },
    left_(traits::LEFT),
    right_(traits::RIGHT),
    store_(traits::STORE)
{ }

template<typename T> inline void
in<T>::setup()
{
    store_in().setup();
    enc_a_in().setup();
    enc_b_in().setup();
    piso().setup();

    qh().setup();
    val_ = piso().read();

    midi().begin(31250);

    enc_a_int().enable();
    enc_b_int().enable();
}

template<typename T> inline void
in<T>::update(unsigned long t, bool slow)
{
    encoder_.update(enc_a_in().read(), enc_b_in().read(), t);

    if (!slow) return;

    qh().setup();
    val_ = piso().read();

    static const uint8_t m[] = { LOOP0, LOOP1 };

    for (uint8_t i = 0; i < 2; i++) {
        loop_[i].update((val_ & (1 << m[i])) == 0, t);
    }

    store_.update(store_in().read(), t);
    left_.update((val_ & (1 << LEFT)) == 0, t);
    right_.update((val_ & (1 << RIGHT)) == 0, t);
}

}
