
#pragma once

#include <stdint.h>

#include "bits/pin_traits.h"

namespace artl {

template<uint8_t PIN_NO, typename PIN_TRAITS = pin_traits<PIN_NO> >
struct digital_out {

    enum {
        pin = PIN_NO
    };

    using traits = PIN_TRAITS;

    static void setup() { traits::output(); }

    static void output() { traits::output(); }

    static void high() { traits::high(); }

    static void low() { traits::low(); }

    static void write(bool v) { traits::write(v); }

    constexpr operator uint8_t() const { return PIN_NO; }
};

}
