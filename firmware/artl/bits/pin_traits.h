
#pragma once

#include <stdint.h>
#include <avr/io.h>

#include "port_traits.h"

namespace artl {

namespace pin {

  template<typename PORT, uint8_t BIT_NO>
  struct traits {

      using port_t = PORT;
      using port_traits = port::traits<port_t>;

      constexpr static uint8_t bit_mask = 1 << BIT_NO;
      constexpr static uint8_t bit_no = BIT_NO;

      static void input() { port_traits::dir() &= ~bit_mask; }

      static void output() { port_traits::dir() |= bit_mask; }

      static bool read() {
          return read_bit() != 0;
      }

      static uint8_t read_bit() {
          return port_traits::in() & bit_mask;
      }

      static void write(bool v) {
          if (v) {
              high();
          } else {
              low();
          }
      }

      static void high() { port_traits::out() |= bit_mask; }

      static void low() { port_traits::out() &= ~bit_mask; }
  };

} // namespace pin

} // namespace artl
