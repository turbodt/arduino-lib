#ifndef INCLUDE_INTERACTION_UTILS
#define INCLUDE_INTERACTION_UTILS

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

namespace interaction_utils {

  const uint8_t NO_BUTTON = 16;
  class Buttons {
    protected:
      uint8_t num_btns;
      uint8_t * pin_btns;
    public:
      Buttons(
        uint8_t const pin_btns[],
        uint8_t const & num_btns
      );
      virtual ~Buttons();
      // up to 16 buttons
      uint16_t wait_press() const;
      uint8_t wait_first_press() const;
  };

  class LcdI2CMenu {
    protected:
      LiquidCrystal_I2C * lcd;
      Buttons const * buttons;
      uint8_t num_options;
      String const * options;

    public:
      LcdI2CMenu(
        LiquidCrystal_I2C * const lcd,
        Buttons const * const buttons,
        String const options[],
        uint8_t const & num_options
      );
      virtual ~LcdI2CMenu();

      uint8_t select(
        uint8_t const & confirm_button_index,
        uint8_t const & prev_button_index,
        uint8_t const & next_button_index
      ) const;
  };
}

#endif
