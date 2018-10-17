
#pragma once

#include "../lcd.h"
#include "graphic.h"
#include "hero.h"
#include "block.h"

namespace hero {

struct Game {
    void step() {
        if (!hero_.clashed()) {
            // move all blocks
            for (uint8_t i = 0; i < block_count_; i++) {
                block_[i].step();
            }

            // remove zero-length block(s)
            while (block_count_ > 0 && block_[0].length_ == 0) {
                for (uint8_t i = 1; i < block_count_; i++) {
                    block_[i - 1] = block_[i];
                }
                --block_count_;
            }

            if (skip_new_block_ == 0) {
                block_[block_count_].generate();

                skip_new_block_ = block_[block_count_].length_ + 10 + random(10);

                ++block_count_;
            } else {
                --skip_new_block_;
            }
        }

        hero_.step();

        // TODO decrease step_interval_
    }

    void setup() {
        graphic_.setup();
        started_ = false;
    }

    void start(unsigned long t) {
        last_step_ = t;
        step_interval_ = 150;

        hero_.start();

        block_count_ = 0;
        skip_new_block_ = 10 + random(10);

        lcd_buf_clear();
        lcd_update();

        started_ = true;
    }

    void update(unsigned long t) {
        if (!started_) {
            return;
        }

        if (t - last_step_ > step_interval_) {
            step();
            last_step_ = t;
        }
    }

    void jump(unsigned long t) {
        if (!started_ || hero_.clashed()) {
            start(t);
            return;
        }

        hero_.jump();
    }

    Block block_[8];
    uint8_t block_count_;

    unsigned long last_step_;
    unsigned long step_interval_;

    Hero hero_;

    bool started_;
    uint8_t skip_new_block_;

    Graphic graphic_;
};

}
