#include <pic12f509.interface.hpp>

namespace pic12f509 {

  Pic12f509Interface::Pic12f509Interface(
    uint8_t pin_vpp,
    uint8_t pin_vdd,
    uint8_t pin_clock,
    uint8_t pin_data
  ) {
    this->pin_vpp = pin_vpp;
    this->pin_vdd = pin_vdd;
    this->pin_clock = pin_clock;
    this->pin_data = pin_data;

    pinMode(this->pin_vpp, OUTPUT);
    pinMode(this->pin_vdd, OUTPUT);
    pinMode(this->pin_clock, OUTPUT);
    pinMode(this->pin_data, OUTPUT);

    digitalWrite(this->pin_vpp, LOW);
    digitalWrite(this->pin_vdd, LOW);
    digitalWrite(this->pin_clock, LOW);
    digitalWrite(this->pin_data, LOW);
  };

  Pic12f509Interface::~Pic12f509Interface() {};

  Pic12f509Interface const * Pic12f509Interface::init() const {
    digitalWrite(this->pin_vdd, HIGH);
    delayMicroseconds(T_PPDP_US);
    digitalWrite(this->pin_vpp, HIGH);
    delayMicroseconds(T_SET0_US);
    delayMicroseconds(T_HLD0_US);
    return this;
  };

  Pic12f509Interface const * Pic12f509Interface::end() const {
    digitalWrite(this->pin_vdd, LOW);
    digitalWrite(this->pin_vpp, LOW);
    // make the low effective
    delayMicroseconds(1);
    return this;
  };

  Pic12f509Interface const * Pic12f509Interface::send_bit(bool b) const {
    digitalWrite(this->pin_clock, HIGH);
    if (b) {
      digitalWrite(this->pin_data, HIGH);
    }
    delayMicroseconds(T_SET1_US);
    digitalWrite(this->pin_clock, LOW);
    delayMicroseconds(T_HLD1_US);
    if (b) {
      digitalWrite(this->pin_data, LOW);
    }
    return this;
  };

  bool Pic12f509Interface::get_bit() const {
    digitalWrite(this->pin_clock, HIGH);
    delayMicroseconds(T_DLY3_US);
    bool b = digitalRead(this->pin_data);
    digitalWrite(this->pin_clock, LOW);
    // the following delay is probably not needed
    delayMicroseconds(T_HLD1_US);
    return b;
  };

  Pic12f509Interface const * Pic12f509Interface::send_command(command_t const & command) const {
    for (uint8_t i = 0 ; i<4; i++) {
      this->send_bit((command >> i) % 2);
    }

    // send two don't care what bits
    this->send_bit(0);
    this->send_bit(0);

    // wait
    switch (0x0F & command) {
      case CMD_LOAD_DATA:
        delayMicroseconds(T_DLY2_US);
        break;
      case CMD_READ_DATA:
        delayMicroseconds(T_DLY2_US);
        break;
      case CMD_INCREMENT_ADDRESS:
        delayMicroseconds(T_DLY2_US);
        break;
      case CMD_BEGIN_PROGRAMMING:
        delayMicroseconds(T_PROG_US);
        break;
      case CMD_END_PROGRAMMING:
        delayMicroseconds(T_DIS_US);
        break;
      case CMD_BULK_ERASE:
        delayMicroseconds(T_ERA_US);
        break;
    }
    return this;
  };

  word_t Pic12f509Interface::get_word() const {
    word_t word = 0;
    // start bit
    this->send_bit(0);

    pinMode(this->pin_data, INPUT);
    word_t p = 1;
    for (uint8_t i = 0; i < 14; i++) {
      if (this->get_bit()) {
        word += p;
      }
      // following delay might be not necessary
      delayMicroseconds(1);
      p = p << 1;
    }
    pinMode(this->pin_data, OUTPUT);

    // stop bit
    this->send_bit(0);
    return word;
  };

  Pic12f509Interface const * Pic12f509Interface::send_word(word_t const & word) const {
    // start bit
    this->send_bit(0);

    // send 12 bits
    for (uint8_t i = 0 ; i<12; i++) {
      this->send_bit((word >> i) % 2);
    }

    // send two don't care what bits
    this->send_bit(0);
    this->send_bit(0);

    // stop bit
    this->send_bit(0);
    return this;
  };


  // configuration functions
  uint8_t get_mclr_selection(const word_t & conf_word) {
    return 0x10 & conf_word;
  }

  bool is_code_proctection_enabled(word_t const & conf_word) {
    return !(bool)(0x08 & conf_word);
  }

  bool is_watchdog_timer_enabled(word_t const & conf_word) {
    return 0x04 & conf_word;
  }

  uint8_t get_oscillator_selection(const word_t & conf_word) {
    return 0x03 & conf_word;
  }

}
