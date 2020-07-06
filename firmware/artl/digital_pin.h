
#pragma once

#include <stdint.h>
#include "bits/pin_traits.h"

namespace artl {

template<uint8_t PIN_NO, typename TRAITS = pin_traits<PIN_NO> >
struct digital_pin {

    enum {
        pin = PIN_NO
    };

    using traits = TRAITS;


    static void output() { traits::output(); }

    static void input() { traits::input(); }


    static void high() { traits::high(); }

    static void low() { traits::low(); }

    static void write(bool v) { traits::write(v); }

    static bool read() { return traits::read(); }

    static uint8_t read_bit() { return traits::read_bit(); }


    constexpr operator uint8_t() const { return PIN_NO; }
};

}
