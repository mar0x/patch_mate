
#pragma once

#include <stdint.h>

#include "bits/pin_traits.h"

namespace artl {

template<uint8_t PIN_NO, typename PIN_TRAITS = pin_traits<PIN_NO> >
struct digital_in {

    using traits = PIN_TRAITS;

    void setup() { traits().input(); }

    void input() { traits().input(); }

    void pullup() { traits().high(); }

    bool read() { return traits().read(); }

    constexpr operator uint8_t() const { return PIN_NO; }
};

}
