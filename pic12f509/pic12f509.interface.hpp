#ifndef INCLUDE_PIC12F509_INTERFACE
#define INCLUDE_PIC12F509_INTERFACE

#include <Arduino.h>

#define T_PPDP_US 10 // >= 5us
#define T_SET0_US 2 // >= 100ns
#define T_HLD0_US 20 // >= 5us. Expirementally, 15us was not enought, and seems it works with 20us
#define T_DLY1_US 2 // >= 1us
#define T_DLY2_US 2 // >= 1us
#define T_DLY3_US 2 // <= 80ns -> time from they
#define T_SET1_US 1 // >= 100ns
#define T_HLD1_US 1 // >= 100ns
#define T_PROG_US 2500 // <= 2ms -> time from they. Min time for ensuring
#define T_DIS_US 200 // >= 100us
#define T_ERA_US 10000 // <= 10ms -> time from they. Min time for ensuring

namespace pic12f509 {

  typedef uint16_t addr_t;
  typedef uint16_t word_t;
  typedef uint8_t command_t;

  const uint16_t MEM_LEN = 0x800;
  const uint16_t USER_MEM_LEN = 0x400;

  const command_t CMD_LOAD_DATA = 0x02;
  const command_t CMD_READ_DATA = 0x04;
  const command_t CMD_INCREMENT_ADDRESS = 0x06;
  const command_t CMD_BEGIN_PROGRAMMING = 0x08;
  const command_t CMD_END_PROGRAMMING = 0x0E;
  const command_t CMD_BULK_ERASE = 0x09;

  const addr_t LOC_RESET_VECTOR = 0x03FF;
  const addr_t LOC_OSCCAL_VALUE = 0x0404;
  const addr_t LOC_CONF_WORD = 0x07FF;

  // MCLRE
  const uint8_t CONF_MCLRE_MASK = 0x10;
  const uint8_t CONF_MCLRE_MCLR = 0x10;
  const uint8_t CONF_MCLRE_GP3 = 0x00;
  // Code protection
  const uint8_t CONF_CODE_PROTECTION_MASK = 0x08;
  const uint8_t CONF_CODE_PROTECTION_ON = 0x00;
  const uint8_t CONF_CODE_PROTECTION_OFF = 0x08;
  // Watchdog timer
  const uint8_t CONF_WDTE_MASK = 0x04;
  const uint8_t CONF_WDTE_ON = 0x04;
  const uint8_t CONF_WDTE_OFF = 0x00;
  // Oscilator selection
  const uint8_t CONF_OSC_SEL_MASK = 0x03;
  const uint8_t CONF_OSC_SEL_LP = 0x00;
  const uint8_t CONF_OSC_SEL_XP = 0x01;
  const uint8_t CONF_OSC_SEL_INTOSC = 0x02;
  const uint8_t CONF_OSC_SEL_EXTRC = 0x03;

  // Instructions
  const word_t INS_DESTINATION_MASK = 0x020;
  const word_t INS_DESTINATION_W = 0x000;
  const word_t INS_DESTINATION_F = 0x020;

  const word_t INS_ADDWF = 0x1C0;
  const word_t INS_ADDWF_MASK = 0xFC0;
  const word_t INS_ANDWF = 0x140;
  const word_t INS_ANDWF_MASK = 0xFC0;

  class Pic12f509Interface {

    private:

    protected:
      uint8_t pin_vpp;
      uint8_t pin_vdd;
      uint8_t pin_clock;
      uint8_t pin_data;

      Pic12f509Interface const * send_bit(bool) const;
      bool get_bit() const;

    public:

      Pic12f509Interface(
        uint8_t pin_vpp,
        uint8_t pin_vdd,
        uint8_t pin_clock,
        uint8_t pin_data
      );
      virtual ~Pic12f509Interface();

      Pic12f509Interface const * init() const;
      Pic12f509Interface const * end() const;
      Pic12f509Interface const * send_command(command_t const &) const;
      word_t get_word() const;
      Pic12f509Interface const * send_word(word_t const &) const;

  };

  uint8_t get_mclr_selection(word_t const &);
  bool is_code_proctection_enabled(word_t const &);
  bool is_watchdog_timer_enabled(word_t const &);
  uint8_t get_oscillator_selection(word_t const &);

}

#endif
