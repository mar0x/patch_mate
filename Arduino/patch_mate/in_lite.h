
#pragma once

#include <HardwareSerial.h>

#include "artl/digital_out.h"
#include "artl/digital_in.h"

#include "artl/bit_order.h"
#include "artl/piso.h"
#include "artl/pin_change_int.h"

#include "config.h"

namespace patch_mate {

struct in {
    void setup();
    void update(unsigned long t);

    bool loop(uint8_t i) const;
    bool left() const;
    bool right() const;
    bool encoder_a() const;
    bool encoder_b() const;
    bool store() const;

    static constexpr
    HardwareSerial& midi();

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

    piso_t val_;
};


inline void
in::setup()
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

    // PCICR = 1 << PCIE0; // enable interrupt
    // PCMSK0 = (1 << PCINT6) | (1 << PCINT3); // encoder inputs
}

inline void
in::update(unsigned long)
{
    qh().setup();
    val_ = piso().read();
}

inline bool
in::loop(uint8_t i) const
{
    uint8_t m[] = { LOOP0, LOOP1 };
    return (val_ & (1 << m[i])) == 0;
}

inline bool
in::left() const
{
    return (val_ & (1 << LEFT)) == 0;
}

inline bool
in::right() const
{
    return (val_ & (1 << RIGHT)) == 0;
}

inline bool
in::encoder_a() const
{
    return enc_a_in().read();
}

inline bool
in::encoder_b() const
{
    return enc_b_in().read();
}

inline bool
in::store() const
{
    return store_in().read();
}

inline constexpr HardwareSerial&
in::midi()
{
#if defined(HAVE_HWSERIAL1)
    return Serial1;
#else
    return Serial;
#endif
}

}
