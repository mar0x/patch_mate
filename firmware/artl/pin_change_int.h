
#pragma once

#include <stdint.h>
#include "bits/pin_change_traits.h"

namespace artl {

template<uint8_t PIN_NO, typename TRAITS = pin_change_traits<PIN_NO> >
struct pin_change_int {

    using traits = TRAITS;

    static void enable() {
        icr_enable();
        traits::enable();
    }

    static void disable() { traits::disable(); }

    static void icr_enable() { PCICR |= traits::icr_bit_mask; }

    static void icr_disable() { PCICR &= ~traits::icr_bit_mask; }
};

}
