
#pragma once

#include "../artl/button.h"
#include "../artl/encoder.h"

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
    Serial1.begin(31250);
}

template<typename T> inline void
in<T>::update(unsigned long t, bool slow)
{
    encoder_.update(0, 0, t);

    if (!slow) return;

    loop_[0].update(0, t);
    loop_[1].update(0, t);
    loop_[2].update(0, t);
    loop_[3].update(0, t);
    loop_[4].update(0, t);
    loop_[5].update(0, t);
    loop_[6].update(0, t);
    loop_[7].update(0, t);

    store_.update(0, t);
    left_.update(0, t);
    right_.update(0, t);
}

template<typename T> inline bool
in<T>::usb_midi_read(midi_cmd_t &c, uint8_t& jack) const {
    usb_midi::event_t ev;
    if (!usb_midi::port.recv(ev)) return false;

    uint8_t size = ev.size();

    if (size == 0) return false;

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
