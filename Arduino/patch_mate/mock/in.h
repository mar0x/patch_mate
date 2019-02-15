
#pragma once

namespace patch_mate {

template<typename HANDLERS>
struct button : public HANDLERS {
    using HANDLERS::HANDLERS;

    bool up() const { return !down_; }
    bool down() const { return down_; }
    bool hold() const { return hold_; }

    bool update(bool down, unsigned long t) {
        if (down != down_) {
            down_ = down;
            state_change_time_ = t;

            this->on_down(down_, t);

            hold_ = false;
            return true;
        }

        if (down_ && !hold_ &&
            t - state_change_time_ >= 500)
        {
            hold_ = true;
            this->on_hold(t);
        }

        return false;
    }

private:
    bool down_ = false;
    bool hold_ = false;
    unsigned long state_change_time_ = 0;
};

int last_key = -1;

template<typename T>
struct in {
    using traits = T;

    in();

    void setup();
    void update(unsigned long t, bool slow);

    serial& midi()
    {
        return midi_;
    }

    using button_type = button<typename traits::button_handler>;

    const button_type& loop(uint8_t i) const { return loop_[i]; }
    const button_type& left() const { return left_; }
    const button_type& right() const { return right_; }
    const button_type& store() const { return store_; }

private:
    serial midi_;

    button_type loop_[8];
    button_type left_;
    button_type right_;
    button_type store_;

    typename traits::encoder_handler encoder_handler_;

    bool lock_left_ = false;
    bool lock_right_ = false;
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
    midi().begin(31250);
}

template<typename T> inline void
in<T>::update(unsigned long t, bool)
{
    for (uint8_t i = 0; i < 8; i++) {
        loop_[i].update(last_key == ('1' + i), t);
    }

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

    mvwprintw(key_win, 1, 0, "%c", (left_.down() ? '<' : '.'));
    mvwprintw(key_win, 1, 4, "%c", (right_.down() ? '>' : '.'));
    mvwprintw(key_win, 0, 2, "%c", (last_key == KEY_UP ? '^' : '.'));
    mvwprintw(key_win, 2, 2, "%c", (last_key == KEY_DOWN ? 'v' : '.'));
    wrefresh(key_win);

    if (last_key >= 'I' && last_key <= 'P') {
        const uint8_t b[] = { 192, (uint8_t) (last_key - 'I') };
        midi_.simulate(b, 2);
    }

    if (last_key >= 'a' && last_key <= 'h') {
        const uint8_t b[] = { 176, (uint8_t) (last_key - 'a'), 0 };
        midi_.simulate(b, 3);
    }

    if (last_key >= 'A' && last_key <= 'H') {
        const uint8_t b[] = { 176, (uint8_t) (last_key - 'A'), 127 };
        midi_.simulate(b, 3);
    }
}

}
