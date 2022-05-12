#ifndef INCLUDE_AT24C32_DEFINITIONS
#define INCLUDE_AT24C32_DEFINITIONS

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
}

#endif
