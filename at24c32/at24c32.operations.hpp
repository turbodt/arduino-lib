#ifndef INCLUDE_AT24C32_OPERATIONS
#define INCLUDE_AT24C32_OPERATIONS

#include "at24c32.definitions.hpp"
#include "at24c32.interface.hpp"

namespace at24c32 {
  class Interface;
  class SequentialWrite  {
    private:
    protected:
      Interface const * interface;
      addr_t current_addr;
      bool ack;
      bool should_restart;
    public:
      SequentialWrite(Interface const *);
      SequentialWrite * init(addr_t const &);
      SequentialWrite * send_next_word(word_t const &);
      SequentialWrite * end();
      bool had_errors() const;
  };
}

#endif
