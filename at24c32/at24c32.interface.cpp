#include "at24c32.interface.hpp"

namespace at24c32 {

  Interface::Interface(
    const uint8_t & device_addr,
    const uint8_t & gpio_data,
    const uint8_t & gpio_clock
  ) {
    this->device_addr = device_addr;
    this->gpio_data = gpio_data;
    this->gpio_clock = gpio_clock;

    pinMode(gpio_data, OUTPUT);
    pinMode(gpio_clock, OUTPUT);
    digitalWrite(this->gpio_data, HIGH);
    digitalWrite(this->gpio_clock, HIGH);
  }

  Interface const * Interface::start() const {
    delayMicroseconds(T_SU_STA_US);
    digitalWrite(this->gpio_data, LOW);
    delayMicroseconds(T_HD_STA_US);
    digitalWrite(this->gpio_clock, LOW);
    delayMicroseconds(T_LOW_US);
    return this;
  }

  Interface const * Interface::stop() const {
    digitalWrite(this->gpio_clock, HIGH);
    delayMicroseconds(T_SU_STO_US);
    digitalWrite(this->gpio_data, HIGH);
    delayMicroseconds(T_BUF_US);
    return this;
  }

  Interface const * Interface::prepare_restart() const {
    // put both gpios to HIGH without perform stop condition
    digitalWrite(this->gpio_data, HIGH);
    delayMicroseconds(1);
    digitalWrite(this->gpio_clock, HIGH);
    return this;
  }

  Interface const * Interface::send_bit(bool b) const {
    if (b) {
      digitalWrite(this->gpio_data, HIGH);
    }
    delayMicroseconds(T_SU_DAT_US);
    digitalWrite(this->gpio_clock, HIGH);
    delayMicroseconds(T_HIGH_US);
    digitalWrite(this->gpio_clock, LOW);
    delayMicroseconds(T_HD_DAT_US);
    if (b) {
      digitalWrite(this->gpio_data, LOW);
    }
    delayMicroseconds(T_LOW_US);
    return this;
  }

  bool Interface::get_bit() const {
    pinMode(this->gpio_data, INPUT);

    digitalWrite(this->gpio_clock, HIGH);
    delayMicroseconds(T_HIGH_US);
    bool b = digitalRead(this->gpio_data);
    digitalWrite(this->gpio_clock, LOW);

    delayMicroseconds(T_DH_US);
    delayMicroseconds(T_LOW_US);

    pinMode(this->gpio_data, OUTPUT);
    return b;
  }

  bool Interface::send_byte(uint8_t const & byte) const {
    for (uint8_t i = 8; i > 0; i--) {
      bool b = (byte >> (i-1)) % 2;
      this->send_bit(b);
    }
    return this->get_bit();
  };

  uint8_t Interface::get_byte(bool is_last) const {
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
      byte = byte << 1;
      byte |= this->get_bit();
    }
    if (!is_last) {
      this->send_bit(0); //send ack
    }
    return byte;
  };

  bool Interface::send_address(addr_t const & addr) const {
    bool ack;
    ack = this->send_byte(addr >> 8);
    ack |= this->send_byte(addr);
    return ack;
  }

  bool Interface::init(uint8_t const & rw_bit, uint8_t const & max_tries) const {
    bool ack;
    this->start();
    ack = this->send_byte((this->device_addr << 1 ) | rw_bit);
    for (uint8_t i = 1 ; ack && (!max_tries || i < max_tries); i++) {
      delayMicroseconds(T_WR_US);
      ack = this->send_byte((this->device_addr << 1 ) | rw_bit);
    }
    return ack;
  }

  Interface const * Interface::send_word(
    addr_t const & addr,
    word_t const & word
  ) const {
    bool ack;
    ack = this->init(WRITE);
    ack |= this->send_address(addr);
    ack |= this->send_byte(word);
    this->stop();
    delayMicroseconds(T_WR_US);
    return this;
  }

  word_t Interface::get_word() const {
    this->init(READ);
    word_t word = this->get_byte();
    this->stop();
    return word;
  }

  word_t Interface::get_word(addr_t const & addr) const {
    bool ack;
    // perform "dummy writte"
    ack = this->init(WRITE);
    ack |= this->send_address(addr);
    this->prepare_restart();
    // perform actual read
    return this->get_word();
  }

  Interface const * Interface::init_sequential_read() const {
    this->init(READ);
    return this;
  }

  Interface const * Interface::init_sequential_read(addr_t const & addr) const {
    bool ack;
    // perform "dummy writte"
    ack = this->init(WRITE);
    ack |= this->send_address(addr);
    this->prepare_restart();
    this->init_sequential_read();
    return this;
  }

  Interface const * Interface::end_sequential_read() const {
    this->get_byte(true); // needed to perform stop
    this->stop();
    return this;
  }

  word_t Interface::get_next_word(bool is_last) const {
    word_t word = this->get_byte(is_last);
    if (is_last) {
      this->stop();
    }
    return word;
  }
}
