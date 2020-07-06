
#pragma once

#include "log_window.h"

namespace patch_mate {

struct out {
    void setup();

    void loop_relay(uint8_t i, bool v);
    void loop_led(uint8_t i, bool v);
    void loop_led(uint16_t mask);
    void loop(uint8_t i, bool v);
    void store_led(bool v);

    void mute();
    void unmute();

    void commit();
    void commit_led();

    bool relay_changed() const;

    void midi_write(const void* data, int s);

    template<typename T1>
    void serial_print(const T1& a1) { pts.print(a1); }

    template<typename T1, typename ...Args>
    void serial_print(const T1& a1, Args... args) {
        pts.print(a1);
        serial_print(args...);
    }

    template<typename T1>
    void serial_println(const T1& a1) { pts.println(a1); }

    template<typename T1, typename ...Args>
    void serial_println(const T1& a1, Args... args) {
        pts.print(a1);
        serial_println(args...);
    }

private:

    uint16_t led_;
    uint16_t rel_;
    uint16_t new_led_;
    uint16_t new_rel_;
};


inline void
out::setup()
{
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

    mvwprintw(loop_win, 0, 3 + (i * 2), "%c", (v ? '=' : '-'));
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

    mvwprintw(loop_win, 1, 3 + (i * 2), "%c", (v ? 'o' : '.'));
}

inline void
out::loop_led(uint16_t mask)
{
    uint16_t m = 1;

    for (uint8_t i = 0; i < MAX_LOOP; i++, m = m << 1) {
        loop_led(i, mask & m);
    }
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
    mvwprintw(loop_win, 1, 0, "%c", (v ? 'o' : '.'));
    wrefresh(loop_win);
}

inline void
out::mute()
{
    debug(7, "mute");
    mvwprintw(loop_win, 0, 0, "%c", 'M');
    wrefresh(loop_win);
}

inline void
out::unmute()
{
    debug(7, "unmute");
    mvwprintw(loop_win, 0, 0, "%c", ' ');
    wrefresh(loop_win);
}

inline bool
out::relay_changed() const
{
    return new_rel_ != rel_;
}

inline void
out::commit()
{
    if (new_rel_ == rel_ && new_led_ == led_) return;

    debug(7, "out commit: ", led_, ":", rel_, " -> ", new_led_, ":", new_rel_);

    led_ = new_led_;
    rel_ = new_rel_;

    wrefresh(loop_win);
}

inline void
out::midi_write(const void* data, int s) {
    log_window::print("midi write: ");

    uint8_t *b = (uint8_t *) data;
    for (uint8_t i = 0; i < s; i++) {
        log_window::print(b[i], " ");
    }
    log_window::println("");
}

inline void
out::commit_led()
{
    if (new_led_ == led_) return;

    debug(7, "out commit_led: ", led_, " -> ", new_led_);

    led_ = new_led_;

    wrefresh(loop_win);
}

}
