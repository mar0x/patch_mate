
#pragma once

#include <stdint.h>

#include "bits/pin_traits.h"

namespace artl {

template<uint8_t PIN_NO, typename PIN_TRAITS = pin_traits<PIN_NO> >
struct digital_in {

    enum {
        pin = PIN_NO
    };

    using traits = PIN_TRAITS;

    static void setup() { traits::input(); }

    static void input() { traits::input(); }

    static void pullup() { traits::high(); }

    static bool read() { return traits::read(); }

    static uint8_t read_bit() { return traits::read_bit(); }

    constexpr operator uint8_t() const { return PIN_NO; }
};

}
