
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
    /* 2x 74HC165 pinout
           +8 
      pin  msb  lsb  desc
      --------------------
       A    0    7   LOOP 1 (0)
       B    1    6   LOOP 2 (1)
       C    2    5   LOOP 3 (2)
       D    3    4   LOOP 4 (3)
       E    4    3   NC -> LOOP 8 (7) !!!
       F    5    2   LOOP 7 (6)
       G    6    1   LOOP 6 (5)
       H    7    0   LOOP 5 (4)
      CLK_INH        NC -> GND        !!!

                 +8
      pin  msb  lsb  desc
      --------------------
       A    0    7   LEFT
       B    1    6   STORE
       C    2    5   NC -> GND        !!!
       D    3    4   RIGHT
       E    4    3   GND
       F    5    2   GND
       G    6    1   GND
       H    7    0   GND

      QH'            A2
      CLK            A1
      SH/LD          A0

     */
    enum lsb_bits {
        LEFT  = 15,
        RIGHT = 12,
        STORE = 14,
        LOOP0 = 7,
        LOOP1 = 6,
        LOOP2 = 5,
        LOOP3 = 4,
        LOOP4 = 0,
        LOOP5 = 1,
        LOOP6 = 2,
        LOOP7 = 3,
    };

    enum {
        enc_a_pin = artl::pin::id::mini_13,  // or micro_15
        enc_b_pin = artl::pin::id::mini_12,  // or micro_14
    };

    using enc_a_in = artl::digital_in< enc_a_pin >;
    using enc_b_in = artl::digital_in< enc_b_pin >;

    using enc_a_int = artl::pin_change_int< enc_a_pin >;
    using enc_b_int = artl::pin_change_int< enc_a_pin >;

    using ld = artl::digital_out<A0>;
    using clk = artl::digital_out<A1>;
    using qh = artl::digital_in<A2>;

    using piso_t = uint16_t;
    using piso = artl::piso<piso_t, artl::lsb_first, qh, ld, clk>;

    piso_t val_;
};


inline void
in::setup()
{
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
    uint8_t m[] = { LOOP0, LOOP1, LOOP2, LOOP3, LOOP4, LOOP5, LOOP6, LOOP7 };
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
    return (val_ & (1 << STORE)) == 0;
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
