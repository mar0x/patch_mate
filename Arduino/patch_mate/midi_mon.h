
#pragma once

#include "debug.h"
#include "lcd.h"

namespace patch_mate {

struct midi_mon {
    void setup(unsigned long t);

    void update(unsigned long t);
    void read(uint8_t b);
    bool toggle_pause(unsigned long t);
    void scroll_up(unsigned long t);
    void scroll_down(unsigned long t);

private:
    enum {
        MAX_LINE = 10,
        MAX_COL = 14,
        LEFT_COL = 2,
    };

    inline uint8_t next_line(uint8_t l) { return (l + 1) % MAX_LINE; }
    inline uint8_t prev_line(uint8_t l) { return (l + MAX_LINE - 1) % MAX_LINE; }

    bool active_;
    char buf_[MAX_LINE][MAX_COL];

    uint8_t line_;
    uint8_t col_;

    uint8_t scroll_pos_; // index of first displayed line in buf_[]

    // update column, row and size in LCD coordinates
    struct {
        uint8_t col;
        uint8_t row;
        uint8_t size;
        bool all;
    } update_;

    unsigned long last_update_;
};

inline void
midi_mon::setup(unsigned long t) {
    active_ = true;
    memset(buf_[0], ' ', sizeof(buf_));
    line_ = 0;
    col_ = 0;
    scroll_pos_ = MAX_LINE - 1;

    update_.size = 0;
    update_.all = false;
    last_update_ = t;
}

inline void
midi_mon::read(uint8_t b) {
    if (!active_) {
        return;
    }

    uint8_t l = (b < 10) ? 1 :
                  (b < 100) ? 2 : 3;

    if (col_ > 0) {
        ++l;
    }

    if (l + col_ > MAX_COL) {
        col_ = 0;

        if (line_ != scroll_pos_) {
            update_.all = true;
            scroll_pos_ = line_;
        }

        line_ = next_line(line_);
        memset(buf_[line_], ' ', MAX_COL);
        --l;

        if (!update_.all) {
            if (update_.size == 0 || update_.row == 1) {
                update_.row = 1;
                update_.col = LEFT_COL;
                update_.size = MAX_COL;
            }
        }
    } else {
        if (line_ != scroll_pos_ && line_ != next_line(scroll_pos_)) {
            update_.all = true;
            scroll_pos_ = prev_line(line_);
        }
    }

    char *p = buf_[line_] + col_ + l;

    do {
        char c = b % 10;
        b /= 10;

        *--p = c + '0';
    } while (b);

    if (!update_.all) {
        uint8_t lcd_row = (line_ + MAX_LINE - scroll_pos_) % MAX_LINE;
        if (update_.size == 0) {
            update_.row = lcd_row;
            update_.col = col_ + LEFT_COL;
            update_.size = l;
        } else {
            if (update_.row == lcd_row) {
                if (update_.col + update_.size == col_ + LEFT_COL) {
                    update_.size += l;
                } else {
                    update_.col = LEFT_COL;
                    update_.size = MAX_COL;
                }
            } else {
                update_.all = true;
            }
        }
    }

    col_ += l;
}

inline bool
midi_mon::toggle_pause(unsigned long) {
    active_ = !active_;

    return active_;
}

inline void
midi_mon::update(unsigned long t) {
    if (t - last_update_ < 30) {
        return;
    }

    last_update_ = t;

    if (!update_.all && update_.size == 0) {
        return;
    }

    if (update_.all) {
        lcd_buf[0][0] = '0' + scroll_pos_;
        memcpy(lcd_buf[0] + LEFT_COL, buf_[scroll_pos_], MAX_COL);

        uint8_t l = next_line(scroll_pos_);
        lcd_buf[1][0] = '0' + l;
        memcpy(lcd_buf[1] + LEFT_COL, buf_[l], MAX_COL);

        lcd_update();

        update_.all = false;
        update_.size = 0;
    }

    if (update_.size > 0) {
        uint8_t l = (scroll_pos_ + update_.row) % MAX_LINE;

        memcpy(lcd_buf[update_.row] + update_.col, buf_[l] + update_.col - LEFT_COL, update_.size);
        lcd_update(update_.col, update_.row, update_.size);

        update_.size = 0;
    }

    if (line_ == scroll_pos_ || line_ == next_line(scroll_pos_)) {
        uint8_t l = (line_ + MAX_LINE - scroll_pos_) % MAX_LINE;
        lcd.setCursor(col_ + LEFT_COL, l);
        lcd.cursor();
    } else {
        lcd.noCursor();
    }
}

inline void
midi_mon::scroll_up(unsigned long) {
    scroll_pos_ = next_line(scroll_pos_);
    update_.all = true;
}

inline void
midi_mon::scroll_down(unsigned long) {
    scroll_pos_ = prev_line(scroll_pos_);
    update_.all = true;
}

}
