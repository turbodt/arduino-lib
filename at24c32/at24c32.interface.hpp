#ifndef INCLUDE_AT24C32_INTERFACE
#define INCLUDE_AT24C32_INTERFACE

#include <Arduino.h>

namespace at24c32 {

  /** 5v settings **/
  const uint8_t T_LOW_US = 2; // >= 1.2 us
  const uint8_t T_HIGH_US = 1; // >= 0.6 us
  const uint8_t T_L_US = 1; // <= 50 ns
  const uint8_t T_BUF_US = 2; // >= 1.2 us
  const uint8_t T_HD_STA_US = 1; // >= 0.6 us
  const uint8_t T_SU_STA_US = 1; // >= 0.6 us
  const uint8_t T_HD_DAT_US = 1; // >= 0 us
  const uint8_t T_SU_DAT_US = 1; // >= 100 ns
  const uint8_t T_SU_STO_US = 1; // >= 0.6 us
  const uint8_t T_DH_US = 1; // >= 50 ns
  const uint16_t T_WR_US = 11000; // <= 10 ms


  typedef uint8_t word_t;
  typedef uint16_t addr_t;

  const addr_t MEM_LEN = 0x1000;

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

  };


}

#endif
