#ifndef INCLUDE_AT24C32_INTERFACE
#define INCLUDE_AT24C32_INTERFACE

#include <Arduino.h>
#include "at24c32.definitions.hpp"

namespace at24c32 {

  const uint8_t READ = 0x01;
  const uint8_t WRITE = 0x00;

  class SequentialWrite;
  class Interface {

    private:

    protected:
      uint8_t device_addr;
      uint8_t gpio_data;
      uint8_t gpio_clock;

      Interface const * start() const;
      Interface const * stop() const;
      Interface const * prepare_restart() const;

      Interface const * send_bit(bool) const;
      bool get_bit() const;

      bool send_byte(uint8_t const &) const;
      uint8_t get_byte(bool = 1) const;
      bool send_address(addr_t const &) const;

      bool init(uint8_t const &, uint8_t const & = 5) const;

    public:
      Interface(
          const uint8_t &,
          const uint8_t &,
          const uint8_t &
      );

      Interface const * send_word(addr_t const &, word_t const &) const;
      word_t get_word() const;
      word_t get_word(addr_t const &) const;

      Interface const * init_sequential_read() const;
      Interface const * init_sequential_read(addr_t const &) const;
      uint8_t get_next_word(bool = 0) const;
      Interface const * end_sequential_read() const;

      friend class SequentialWrite;
  };
}

#endif
