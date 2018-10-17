
#pragma once

#include <stdint.h>
#include <avr/io.h>

namespace artl {

struct port {
    struct A { };
    struct B { };
    struct C { };
    struct D { };
    struct E { };
    struct F { };
};

template<typename PORT>
struct port_traits {
};

template<>
struct port_traits<port::B> {
    volatile uint8_t& dir() { return DDRB; }
    uint8_t in() { return PINB; }
    volatile uint8_t& out() { return PORTB; }
};

template<>
struct port_traits<port::C> {
    volatile uint8_t& dir() { return DDRC; }
    uint8_t in() { return PINC; }
    volatile uint8_t& out() { return PORTC; }
};

template<>
struct port_traits<port::D> {
    volatile uint8_t& dir() { return DDRD; }
    uint8_t in() { return PIND; }
    volatile uint8_t& out() { return PORTD; }
};

#ifdef DDRE

template<>
struct port_traits<port::E> {
    volatile uint8_t& dir() { return DDRE; }
    uint8_t in() { return PINE; }
    volatile uint8_t& out() { return PORTE; }
};

#endif

#ifdef DDRF

template<>
struct port_traits<port::F> {
    volatile uint8_t& dir() { return DDRF; }
    uint8_t in() { return PINF; }
    volatile uint8_t& out() { return PORTF; }
};

#endif

template<uint8_t PIN_NO>
struct pin_traits {
};

template<typename PORT, uint8_t BIT_NO>
struct base_pin_traits {

    using port = port_traits<PORT>;

    constexpr static uint8_t bit_mask = 1 << BIT_NO;

    void input() const { port().dir() &= ~bit_mask; }

    void output() const { port().dir() |= bit_mask; }

    bool read() const {
        return (port().in() & bit_mask) != 0;
    }

    void write(bool v) const {
        if (v) {
            high();
        } else {
            low();
        }
    }

    void high() const { port().out() |= bit_mask; }

    void low() const { port().out() &= ~bit_mask; }
};

}

#ifdef ARDUINO_AVR_MINI
#define ARTL_PINOUT_STANDARD
#endif

#ifdef ARDUINO_AVR_LEONARDO
#include "variants/leonardo/pin_traits.h"
#endif

#ifdef ARDUINO_AVR_PRO
#define ARTL_PINOUT_STANDARD
#endif

#ifdef ARDUINO_AVR_UNO
#define ARTL_PINOUT_STANDARD
#endif

#ifdef ARTL_PINOUT_STANDARD
#include "variants/standard/pin_traits.h"
#endif
