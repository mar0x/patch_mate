
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

#ifdef DDRA

template<>
struct port_traits<port::A> {
    static volatile uint8_t& dir() { return DDRA; }
    static uint8_t in() { return PINA; }
    static volatile uint8_t& out() { return PORTA; }
};

#endif

#ifdef DDRB

template<>
struct port_traits<port::B> {
    static volatile uint8_t& dir() { return DDRB; }
    static uint8_t in() { return PINB; }
    static volatile uint8_t& out() { return PORTB; }
};

#endif

#ifdef DDRC

template<>
struct port_traits<port::C> {
    static volatile uint8_t& dir() { return DDRC; }
    static uint8_t in() { return PINC; }
    static volatile uint8_t& out() { return PORTC; }
};

#endif

#ifdef DDRD

template<>
struct port_traits<port::D> {
    static volatile uint8_t& dir() { return DDRD; }
    static uint8_t in() { return PIND; }
    static volatile uint8_t& out() { return PORTD; }
};

#endif

#ifdef DDRE

template<>
struct port_traits<port::E> {
    static volatile uint8_t& dir() { return DDRE; }
    static uint8_t in() { return PINE; }
    static volatile uint8_t& out() { return PORTE; }
};

#endif

#ifdef DDRF

template<>
struct port_traits<port::F> {
    static volatile uint8_t& dir() { return DDRF; }
    static uint8_t in() { return PINF; }
    static volatile uint8_t& out() { return PORTF; }
};

#endif

template<uint8_t PIN_NO>
struct pin_traits {
};

template<typename PORT, uint8_t BIT_NO>
struct base_pin_traits {

    using port = port_traits<PORT>;

    constexpr static uint8_t bit_mask = 1 << BIT_NO;
    constexpr static uint8_t bit_no = BIT_NO;

    static void input() { port::dir() &= ~bit_mask; }

    static void output() { port::dir() |= bit_mask; }

    static bool read() {
        return read_bit() != 0;
    }

    static uint8_t read_bit() {
        return port::in() & bit_mask;
    }

    static void write(bool v) {
        if (v) {
            high();
        } else {
            low();
        }
    }

    static void high() { port::out() |= bit_mask; }

    static void low() { port::out() &= ~bit_mask; }
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


#ifdef __AVR_ATtiny24__
#include "variants/tiny14/pin_traits.h"
#endif

#ifdef __AVR_ATtiny44__
#include "variants/tiny14/pin_traits.h"
#endif

#ifdef __AVR_ATtiny84__
#include "variants/tiny14/pin_traits.h"
#endif


#ifdef __AVR_ATtiny25__
#include "variants/tiny8/pin_traits.h"
#endif

#ifdef __AVR_ATtiny45__
#include "variants/tiny8/pin_traits.h"
#endif

#ifdef __AVR_ATtiny85__
#include "variants/tiny8/pin_traits.h"
#endif
