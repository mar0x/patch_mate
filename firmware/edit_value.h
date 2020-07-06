
#pragma once

#include "print_number.h"
#include "rotate.h"

namespace patch_mate {

struct edit_value_t {

    void start(unsigned int value, unsigned int min, unsigned int max,
        unsigned int step = 1, unsigned int display_offset = 0,
        unsigned int off = (unsigned int) -1,
        unsigned int on = (unsigned int) -1)
    {
        value_ = value;
        master_ = value;
        min_ = min;
        max_ = max;
        step_ = step;
        display_offset_ = display_offset;
        on_ = on;
        off_ = off;
    }

    void rotate(short dir) {
        value_ = ::patch_mate::rotate(value_, min_, max_ + step_, step_ * dir);
    }

    void print(char *buf) {
        if (value_ == off_) {
            memcpy(buf, "OFF", 3);
        } else if (value_ == on_) {
            memcpy(buf, "ON ", 3);
        } else {
            print_number(buf, 3, value_ + display_offset_);
        }
    }

    bool dirty() const { return value_ != master_; }

    unsigned int value() const { return value_; }
    void value(unsigned int v) { master_ = value_ = v; }

private:
    unsigned int value_;
    unsigned int master_;
    unsigned int min_;
    unsigned int max_;
    unsigned int step_;
    unsigned int display_offset_;
    unsigned int on_;
    unsigned int off_;
};

}
