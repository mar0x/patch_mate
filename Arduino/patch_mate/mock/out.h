
#pragma once

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

    serial& midi() { return midi_; }

private:
    enum {
        LED_START   = 8,
        RELAY_START = 0,
        RELAY_MASK  = 0x00FF,
    };

    uint16_t current_;
    uint16_t new_;

    serial midi_;
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
    m = (m << (i + RELAY_START));

    if (v) {
        new_ |= m;
    } else {
        new_ &= ~m;
    }

    mvwprintw(loop_win, 0, 3 + (i * 2), "%c", (v ? '=' : '-'));
}

inline void
out::loop_led(uint8_t i, bool v)
{
    if (i >= MAX_LOOP) return;

    uint16_t m = 1;
    m = (m << (i + LED_START));

    if (v) {
        new_ |= m;
    } else {
        new_ &= ~m;
    }

    mvwprintw(loop_win, 1, 3 + (i * 2), "%c", (v ? 'o' : '.'));
}

inline void
out::loop_led(uint8_t mask)
{
    uint8_t m = 1;

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
    return ((new_ ^ current_) & RELAY_MASK) != 0;
}

inline void
out::commit()
{
    if (new_ == current_) return;

    debug(7, "out commit: ", current_, " -> ", new_);

    current_ = new_;

    wrefresh(loop_win);
}

inline void
out::commit_led()
{
    uint16_t new_led = (new_ & ~RELAY_MASK) | (current_ & RELAY_MASK);
    if (new_led == current_) return;

    debug(7, "out commit_led: ", current_, " -> ", new_led);

    current_ = new_led;

    wrefresh(loop_win);
}

}
