#include "at24c32.operations.hpp"

namespace at24c32 {
  SequentialWrite::SequentialWrite(Interface const * interface) {
    this->interface = interface;
  }

  SequentialWrite * SequentialWrite::init(addr_t const & addr) {
    this->current_addr = addr;
    this->ack = false;
    this->should_restart = true;
    return this;
  }

  SequentialWrite * SequentialWrite::send_next_word(word_t const & word) {
    if (this->should_restart) {
      this->interface->init(WRITE);
      this->ack |= this->interface->send_address(this->current_addr);
      this->should_restart = false;
    }
    // send byte
    this->ack |= this->interface->send_byte(word);
    // increment address
    this->current_addr++;
    // decide if change page
    if (!(this->current_addr & 0x1F)) {
      this->should_restart = true;
      this->interface->stop();
      delayMicroseconds(T_WR_US);
    }
    return this;
  }

  SequentialWrite * SequentialWrite::end() {
    if (!this->should_restart) {
      this->interface->stop();
      delayMicroseconds(T_WR_US);
    }
    return this;
  }

  bool SequentialWrite::had_errors() const {
    return this->ack;
  };
}
