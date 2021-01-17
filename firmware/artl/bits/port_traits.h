
#pragma once

#include <stdint.h>
#include <avr/io.h>

#include "port.h"

namespace artl {

namespace port {

    template<typename PORT>
    struct traits {
    };

#if defined(DDRA) && defined(PINA) && defined(PORTA)

    template<>
    struct traits<A> {
        static volatile uint8_t& dir() { return DDRA; }
        static uint8_t in() { return PINA; }
        static volatile uint8_t& out() { return PORTA; }
    };

#endif

#if defined(DDRB) && defined(PINB) && defined(PORTB)

    template<>
    struct traits<B> {
        static volatile uint8_t& dir() { return DDRB; }
        static uint8_t in() { return PINB; }
        static volatile uint8_t& out() { return PORTB; }
    };

#endif

#if defined(DDRC) && defined(PINC) && defined(PORTC)

    template<>
    struct traits<C> {
        static volatile uint8_t& dir() { return DDRC; }
        static uint8_t in() { return PINC; }
        static volatile uint8_t& out() { return PORTC; }
    };

#endif

#if defined(DDRD) && defined(PIND) && defined(PORTD)

    template<>
    struct traits<D> {
        static volatile uint8_t& dir() { return DDRD; }
        static uint8_t in() { return PIND; }
        static volatile uint8_t& out() { return PORTD; }
    };

#endif

#if defined(DDRE) && defined(PINE) && defined(PORTE)

    template<>
    struct traits<E> {
        static volatile uint8_t& dir() { return DDRE; }
        static uint8_t in() { return PINE; }
        static volatile uint8_t& out() { return PORTE; }
    };

#endif

#if defined(DDRF) && defined(PINF) && defined(PORTF)

    template<>
    struct traits<F> {
        static volatile uint8_t& dir() { return DDRF; }
        static uint8_t in() { return PINF; }
        static volatile uint8_t& out() { return PORTF; }
    };

#endif

} // namespace port

} // namespace artl

