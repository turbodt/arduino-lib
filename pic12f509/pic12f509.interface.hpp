#ifndef INCLUDE_PIC12F509_INTERFACE
#define INCLUDE_PIC12F509_INTERFACE

#include <Arduino.h>

#define T_PPDP_US 10 // >= 5us
#define T_HLD0_US 10 // >= 5us
#define T_DLY1_US 2 // >= 1us
#define T_DLY2_US 2 // >= 1us
#define T_DLY3_US 2 // <= 80ns -> time from they
#define T_SET1_US 1 // >= 100ns
#define T_HLD1_US 1 // >= 100ns
#define T_PROG_US 2 // <= 2ms -> time from they. Min time for ensuring
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


  };

}

#endif
