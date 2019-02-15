
#pragma once

#include "debug.h"
#include "ring.h"

namespace patch_mate {

struct midi_mon {
    enum {
        MODE_HR,
        MODE_DEC,
        MODE_HEX,
        MODE_MAX
    };

    void setup(unsigned long t);

    void update(unsigned long t);
    void read(uint8_t b);
    bool toggle_pause(unsigned long t);

    void scroll_up(unsigned long t);
    void scroll_down(unsigned long t);

    uint8_t mode() const { return mode_; }
    void mode(uint8_t m, unsigned long t);
    void mode_next(unsigned long t);
    void mode_prev(unsigned long t);

    void show(unsigned long t);
    void hide(unsigned long t);

    bool active() const { return active_; }

private:
    enum {
        MAX_LINE = 10,
        LEFT_COL = 2,
        MAX_COL = LCD_COLUMNS - LEFT_COL,
        MAX_DATA = MAX_LINE * MAX_COL / 2,
        MAX_RENDER = 4,
    };

    using line_ring_t = ring<MAX_LINE, uint8_t>;
    using data_ring_t = ring<MAX_DATA, uint8_t>;

    line_ring_t lines_;
    data_ring_t data_;

    inline bool valid_line(uint8_t l) const {
        return lines_.diff(l) <= lines_.size;
    }

    inline uint8_t lcd_line(uint8_t l) const { return lines_.diff(l, scroll_pos_); }

    inline void update_line(uint8_t l) {
        l = lcd_line(l);

        if (l < LCD_LINES) {
            update_ |= (1 << l);
        }
    }

    inline bool valid_pos(uint8_t p) const {
        return data_.diff(p) < data_.size;
    }

    void clear();

    void set_mode(uint8_t mode);
    void reindex();
    uint8_t render(char *dst, uint8_t pos, bool &sep) const;
    void cr(uint8_t col);

    uint8_t mode_;

    bool active_;
    bool visible_;
    bool banner_visible_;

    uint8_t col_;

    uint8_t scroll_pos_; // index of first displayed line

    uint8_t update_;

    unsigned long last_update_;
    unsigned long last_active_toggle_;
};

inline void
midi_mon::setup(unsigned long t) {
    active_ = true;
    visible_ = false;
    last_active_toggle_ = t;

    data_.reset();
}

inline void
midi_mon::show(unsigned long t) {
    lines_.reset();
    col_ = 0;

    mode_ = MODE_HR;

    scroll_pos_ = lines_.prev();

    last_update_ = t;

    lines_[0] = data_.start;

    visible_ = true;
    banner_visible_ = true;

    reindex();

    update_ = 0;
    update_line(lines_.pos);
}

inline void
midi_mon::hide(unsigned long) {
    visible_ = false;
    active_ = true;
}

inline void
midi_mon::clear() {
    data_.reset();
    lines_.reset();

    col_ = 0;
    scroll_pos_ = lines_.pos;

    update_ = 0xFF;

    lines_[0] = data_.start;
}

inline void
midi_mon::set_mode(uint8_t mode) {
    if (mode_ == mode) {
        return;
    }

    mode_ = mode;

    reindex();

    update_ = 0xFFU;
}

inline void
midi_mon::reindex() {
    uint8_t pos = data_.pos;
    bool active = active_;

    active_ = true;

    data_.set_pos(data_.start);
    lines_.set_pos(lines_.start);
    col_ = 0;
    scroll_pos_ = lines_.prev();

    while (data_.pos != pos) {
        read(data_[data_.pos]);
    }

    active_ = active;
}

inline char hex_digit(uint8_t h) {
    if (h < 10) {
        return '0' + h;
    }

    return 'a' + (h - 10);
}

inline uint8_t dec1_render(char *dst, uint8_t b) {
    dst[0] = '0' + b;
    return 1;
}

inline uint8_t dec2_render(char *dst, uint8_t b) {
    if (b < 10) {
        return dec1_render(dst, b);
    }

    dst[0] = '0' + (b / 10);
    return 1 + dec1_render(dst + 1, b % 10);
}

inline uint8_t dec_render(char *dst, uint8_t b) {
    if (b < 100) {
        return dec2_render(dst, b);
    }

    dst[0] = '0' + (b / 100);
    return 1 + dec2_render(dst + 1, b % 100);
}

inline bool known_midi_cmd(uint8_t b) {
    switch (b & 0xF0U) {
    case midi_cmd::CMD_NOTE_OFF:
    case midi_cmd::CMD_NOTE_ON:
    case midi_cmd::CMD_KEY_PRESSURE:
    case midi_cmd::CMD_CTRL_CHANGE:
    case midi_cmd::CMD_PROG_CHANGE:
    case midi_cmd::CMD_CHAN_PRESSURE:
    case midi_cmd::CMD_PITCH_CHANGE:
        return true;
    }

    return false;
}

inline uint8_t
midi_mon::render(char *dst, uint8_t pos, bool &sep) const {
    uint8_t b = data_[pos];

    sep = false;

    switch (mode_) {
    case MODE_HR:
        if (known_midi_cmd(b)) {
            const static char cstr[] = {
                'o', 'n', 'k', 'c', 'p', 'h', 't' };

            uint8_t ch = (b & 0x0FU) + 1;
            uint8_t n = dec2_render(dst, ch);

            dst[n] = cstr[ ((b & 0xF0U) - midi_cmd::CMD_NOTE_OFF) >> 4 ];

            return n + 1;
        }

        pos = data_.prev(pos);
        if (valid_pos(pos) && (data_[pos] & 0xF0U) == midi_cmd::CMD_PROG_CHANGE) {
            b++;
        }

        sep = true;
        return dec_render(dst, b);

    case MODE_DEC:
        sep = true;
        return dec_render(dst, b);

    default:
        dst[0] = hex_digit(b >> 4);
        dst[1] = hex_digit(b & 0x0F);
        return 2;
    }
}

inline void
midi_mon::cr(uint8_t col) {
    scroll_pos_ = lines_.pos;

    ++lines_;
    col_ = col;

    if (lines_.pos == lines_.start) {
        lines_.pop_front();
        data_.set_start(lines_.front());
    }

    lines_[lines_.pos] = data_.pos;

    update_ = 0xFFU;
}

inline void
midi_mon::read(uint8_t b) {

    if (!active_) {
        return;
    }

    data_[data_.pos] = b;

    if (!visible_) {
        ++data_;

        if (data_.pos == data_.start) {
            data_.pop_front();
        }

        return;
    }

    bool sep = false;
    char str[MAX_RENDER];
    uint8_t n = render(str, data_.pos, sep);

    if (col_ + n <= MAX_COL) {
        col_ += n;

        update_line(lines_.pos);
    } else {
        cr(n);
    }

    ++data_;

    if (data_.pos == data_.start) {
        lines_.pop_front();
        data_.set_start(lines_.front());
    }

    n = sep ? 1 : 0;

    if (col_ + n < MAX_COL) {
        col_ += n;

        update_line(lines_.pos);
    } else {
        cr(0);
    }
}

inline bool
midi_mon::toggle_pause(unsigned long t) {
    if (t - last_active_toggle_ < 500) {
        clear();
    }

    last_active_toggle_ = t;

    active_ = !active_;

    return active_;
}

inline void
midi_mon::update(unsigned long t) {
    if (t - last_update_ < 30) {
        return;
    }

    last_update_ = t;

    if (!update_) {
        return;
    }

    for (uint8_t l = 0; l < LCD_LINES; l++) {
        if ((update_ & (1 << l)) == 0) {
            continue;
        }

        uint8_t line = lines_.sum(l, scroll_pos_);

        lcd_buf_clear(l);
        lcd_buf[l][0] = '0' + line;

        if (!valid_line(line)) {
            lcd_update(0, l, LCD_COLUMNS);
            continue;
        }

        uint8_t pos = lines_[line];
        uint8_t col = 0;

        while (valid_pos(pos)) {
            bool sep = false;
            char str[MAX_RENDER];
            uint8_t n = render(str, pos, sep);

            pos = data_.next(pos);

            if (col + n > MAX_COL) {
                break;
            }

            memcpy(lcd_buf[l] + LEFT_COL + col, str, n);

            col += n + (sep ? 1 : 0);
        }

        lcd_update(0, l, LCD_COLUMNS);
    }

    if (update_ & 0x01) {
        banner_visible_ = false;
    }

    update_ = 0;

    uint8_t l = lcd_line(lines_.pos);

    if (l < LCD_LINES) {
        lcd.setCursor(col_ + LEFT_COL, l);
        lcd.cursor();
    } else {
        lcd.noCursor();
    }
}

inline void
midi_mon::scroll_up(unsigned long) {
    uint8_t p = lines_.next(scroll_pos_);

    if (valid_line(lines_.sum(p, LCD_LINES - 1))) {
        scroll_pos_ = p;
        update_ = 0xFFU;
    } else {
        if (banner_visible_ && valid_line(scroll_pos_)) {
            update_ = 0x01;
        }
    }
}

inline void
midi_mon::scroll_down(unsigned long) {
    uint8_t p = lines_.prev(scroll_pos_);

    if (valid_line(p)) {
        scroll_pos_ = p;
        update_ = 0xFFU;
    } else {
        if (banner_visible_ && valid_line(scroll_pos_)) {
            update_ = 0x01;
        }
    }
}

inline void
midi_mon::mode(uint8_t m, unsigned long) {
    set_mode( m % MODE_MAX );
}

inline void
midi_mon::mode_next(unsigned long) {
    set_mode( (mode_ + 1) % MODE_MAX );
}

inline void
midi_mon::mode_prev(unsigned long) {
    set_mode( (mode_ + MODE_MAX - 1) % MODE_MAX );
}

}
