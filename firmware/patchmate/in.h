
#pragma once

#include "../artl/button.h"
#include "../artl/encoder.h"
#include "../artl/digital_in.h"
#include "../artl/digital_out.h"
#include "../artl/pin_change_int.h"
#include "../artl/bit_order.h"
#include "../artl/piso.h"

#include "../config.h"
#include "../usb_midi.h"
#include "../midi_cmd.h"

namespace patch_mate {

template<typename T>
struct in {
    using traits = T;

    in();

    void setup();
    void update(unsigned long t, bool slow);

    bool midi_available() const { return Serial1.available(); }
    int midi_read() const { return Serial1.read(); }
    bool usb_midi_read(midi_cmd_t& c, uint8_t& jack) const;

    bool serial_connected() const { return Serial.dtr() || Serial.rts(); }
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
    enum lsb_mask {
        LEFT  = 1 << 15,
        RIGHT = 1 << 12,
        STORE = 1 << 14,
        LOOP0 = 1 << 7,
        LOOP1 = 1 << 6,
        LOOP2 = 1 << 5,
        LOOP3 = 1 << 4,
        LOOP4 = 1 << 0,
        LOOP5 = 1 << 1,
        LOOP6 = 1 << 2,
        LOOP7 = 1 << 3,
    };

    using enc_a_in = artl::digital_in< artl::port::B, 1 >;
    using enc_b_in = artl::digital_in< artl::port::B, 3 >;

    using enc_a_int = artl::pin::change_int< artl::port::B, 1 >;
    using enc_b_int = artl::pin::change_int< artl::port::B, 3 >;

    using ld = artl::digital_out< artl::port::F, 7 >;
    using clk = artl::digital_out< artl::port::F, 6 >;
    using qh = artl::digital_in< artl::port::F, 5 >;

    using piso_t = uint16_t;
    using piso = artl::piso<piso_t, artl::lsb_first, qh, ld, clk>;

    using encoder_type = artl::encoder<typename traits::encoder_handler>;

    button_type loop_[MAX_LOOP];
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
    enc_a_in::setup();
    enc_b_in::setup();

    piso::setup();

    qh::setup();

    Serial1.begin(31250);

    enc_a_int::enable();
    enc_b_int::enable();
}

template<typename T> inline void
in<T>::update(unsigned long t, bool slow)
{
    encoder_.update(enc_a_in().read(), enc_b_in().read(), t);

    if (!slow) return;

    qh::setup();
    piso_t v = piso::read();

    loop_[0].update((v & LOOP0) == 0, t);
    loop_[1].update((v & LOOP1) == 0, t);
    loop_[2].update((v & LOOP2) == 0, t);
    loop_[3].update((v & LOOP3) == 0, t);
    loop_[4].update((v & LOOP4) == 0, t);
    loop_[5].update((v & LOOP5) == 0, t);
    loop_[6].update((v & LOOP6) == 0, t);
    loop_[7].update((v & LOOP7) == 0, t);

    store_.update((v & STORE) == 0, t);
    left_.update((v & LEFT) == 0, t);
    right_.update((v & RIGHT) == 0, t);
}

template<typename T> inline bool
in<T>::usb_midi_read(midi_cmd_t &c, uint8_t& jack) const {
    usb_midi::event_t ev;
    if (!usb_midi::port.recv(ev)) return false;

    uint8_t size = ev.size();

    if (size == 0) return false;

    c.sys_ex(ev.sys_ex());
    c.read(ev.byte1);

    if (size > 1) {
        c.read(ev.byte2);

        if (size > 2) {
            c.read(ev.byte3);
        }
    }

    jack = ev.jack();

    return true;
}

}
