
#pragma once

#include "log_window.h"
#include "button.h"
#include "../midi_cmd.h"

namespace patch_mate {

int last_key = -1;

template<typename T>
struct in {
    using traits = T;

    in();

    void setup();
    void update(unsigned long t, bool slow);

    bool midi_available() const { return false; }
    int midi_read() const { return 0; }
    bool usb_midi_read(midi_cmd_t &c, uint8_t &jack) const { return false; }

    bool serial_connected() const { return true; }
    bool serial_available() const { return pts.available(); }
    int serial_read() { return pts.read(); }

    using button_type = button<typename traits::button_handler>;

    const button_type& loop(uint8_t i) const { return loop_[i]; }
    const button_type& left() const { return left_; }
    const button_type& right() const { return right_; }
    const button_type& store() const { return store_; }

private:
    button_type loop_[10];
    button_type left_;
    button_type right_;
    button_type store_;

    typename traits::encoder_handler encoder_handler_;

    bool lock_left_ = false;
    bool lock_right_ = false;
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
    pts.begin(0);
}

template<typename T> inline void
in<T>::update(unsigned long t, bool)
{
    for (uint8_t i = 0; i < 9; i++) {
        loop_[i].update(last_key == ('1' + i), t);
    }
    loop_[9].update(last_key == '0', t);

    if (last_key == '<') {
        lock_left_ = !lock_left_;
    }

    if (last_key == '>') {
        lock_right_ = !lock_right_;
    }

    left_.update(lock_left_ || last_key == KEY_LEFT, t);
    right_.update(lock_right_ || last_key == KEY_RIGHT, t);

    store_.update(last_key == 10, t);

    if (last_key == KEY_UP) {
        encoder_handler_.on_rotate(1, t);
    }

    if (last_key == KEY_DOWN) {
        encoder_handler_.on_rotate(-1, t);
    }

    if (last_key >= 'I' && last_key <= 'P') {
        // const uint8_t b[] = { 192, (uint8_t) (last_key - 'I') };
        // midi_.simulate(b, 2);
    }

    if (last_key >= 'a' && last_key <= 'h') {
        // const uint8_t b[] = { 176, (uint8_t) (last_key - 'a'), 0 };
        // midi_.simulate(b, 3);
    }

    if (last_key >= 'A' && last_key <= 'H') {
        // const uint8_t b[] = { 176, (uint8_t) (last_key - 'A'), 127 };
        // midi_.simulate(b, 3);
    }
}

}
