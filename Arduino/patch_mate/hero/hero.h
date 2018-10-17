
#pragma once

#include "../lcd.h"

namespace hero {

struct Hero {
    void start();
    void step();
    void jump() {
        if (pos_ <= LRUN_2) {
            pos_ = JUMP_1;
            ++jumps_;
        }
    }

    bool clashed() const { return clash_pos_ != OFF; }

    unsigned int distance() const { return distance_; }
    unsigned int jumps() const { return jumps_; }

private:
    enum {
        OFF,
        LRUN_1,
        LRUN_2,
        JUMP_1,
        JUMP_2,
        JUMP_3,
        JUMP_4,
        JUMP_5,
        JUMP_6,
        JUMP_7,
        JUMP_8,
        HRUN_1,
        HRUN_2,
    };

    enum {
        HPOS = 1,
    };

    enum {
        EMPTY = ' ',
        RUN1 = 1,
        RUN2 = 2,
        JUMP = 3,
        JUMP_UPPER = '.', // Use the '.' character for the head
        JUMP_LOWER = 4,
    };

    void draw();

    unsigned int distance_;
    unsigned int jumps_;
    uint8_t pos_;
    uint8_t clash_pos_;
};

inline void
Hero::start() {
    pos_ = OFF;
    distance_ = 0;
    jumps_ = 0;
    clash_pos_ = OFF;
}

inline void
Hero::step()
{
    if (clash_pos_ != OFF) {
        pos_ = (pos_ == OFF) ? clash_pos_ : (uint8_t) OFF;
        draw();
        return;
    }

    ++distance_;

    if (pos_ == LRUN_2 || pos_ == JUMP_8) {
        pos_ = LRUN_1;
    } else

    if (pos_ >= JUMP_3 && pos_ <= JUMP_5 && lcd_buf[HPOS][1] != EMPTY) {
        pos_ = HRUN_1;
    } else

    if (pos_ >= HRUN_1 && lcd_buf[HPOS][1] == EMPTY) {
        pos_ = JUMP_5;
    } else

    if (pos_ == HRUN_2) {
        pos_ = HRUN_1;
    } else {
        ++pos_;
    }

    draw();
}

inline void
Hero::draw()
{
    struct sprite {
        uint8_t h;
        uint8_t l;
    };

    static const sprite pos2sprite[] = {
        { EMPTY, EMPTY },
        { EMPTY, RUN1 },
        { EMPTY, RUN2 },
        { EMPTY, JUMP },
        { JUMP_UPPER, JUMP_LOWER },
        { JUMP, EMPTY },
        { JUMP, EMPTY },
        { JUMP, EMPTY },
        { JUMP, EMPTY },
        { JUMP_UPPER, JUMP_LOWER },
        { EMPTY, JUMP },
        { RUN1, EMPTY },
        { RUN2, EMPTY },
    };

    sprite s = pos2sprite[pos_];

    if (s.h != EMPTY) {
        lcd_write(HPOS, 0, s.h);

        if (lcd_buf[0][HPOS] != EMPTY) {
            clash_pos_ = pos_;
        }
    } else {
        lcd_update(HPOS, 0);
    }

    if (s.l != EMPTY) {
        lcd_write(HPOS, 1, s.l);

        if (lcd_buf[1][HPOS] != EMPTY) {
            clash_pos_ = pos_;
        }
    } else {
        lcd_update(HPOS, 1);
    }
}

}
