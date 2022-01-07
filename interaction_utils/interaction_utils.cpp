#include <interaction_utils.hpp>

namespace interaction_utils {

  // LcdI2CMenu

  LcdI2CMenu::LcdI2CMenu(
    LiquidCrystal_I2C * const lcd,
    Buttons const * const buttons,
    String const options[],
    uint8_t const & num_options
  ) {
    this->lcd = lcd;
    this->buttons = buttons;
    this->num_options = num_options;
    this->options = options;
  }

  LcdI2CMenu::~LcdI2CMenu() {}

  uint8_t LcdI2CMenu::select(
    uint8_t const & confirm_button_index,
    uint8_t const & prev_button_index,
    uint8_t const & next_button_index
  ) const {
    uint8_t selected_option = 0;
    bool is_option_selected = false;
    String num_options_str = String(this->num_options);

    while (!is_option_selected) {
      // display
      String pag_str = String(selected_option + 1) + " of " + num_options_str;

      this->lcd->clear();
      this->lcd->setCursor((16-this->options[selected_option].length())/2, 0);
      this->lcd->print(this->options[selected_option]);
      this->lcd->setCursor(0, 1);
      this->lcd->print("<");
      this->lcd->setCursor(15, 1);
      this->lcd->print(">");
      this->lcd->setCursor( 8 - pag_str.length()/2, 1);
      this->lcd->print(pag_str);

      // wait input
      uint16_t pressed_button = this->buttons->wait_first_press();

      // determine action
      if (pressed_button == confirm_button_index) {
        is_option_selected = true;
      } else if (pressed_button == next_button_index) {
        selected_option = (selected_option + 1 ) % this->num_options;
      } else if (pressed_button == prev_button_index) {
        selected_option = (selected_option + this->num_options - 1 ) % this->num_options;
      }
    }
    return selected_option;
  }

  // Buttons

  Buttons::Buttons(
    uint8_t const pin_btns[],
    uint8_t const & num_btns
  ) {
    this->num_btns = num_btns;
    this->pin_btns = (uint8_t *) malloc(sizeof(uint8_t)*(this->num_btns));
    for (uint8_t i = 0; i < this->num_btns; i++) {
      this->pin_btns[i] = pin_btns[i];
    }
  }

  Buttons::~Buttons() {
    free(this->pin_btns);
  }

  uint16_t Buttons::wait_press() const {
    uint16_t value = 0;
    while (!value) {
      delay(1);
      value = 0;
      uint8_t p = 1;
      for (uint8_t i = 0 ; i < this->num_btns; i++) {
        if (digitalRead(this->pin_btns[i])) {
          value += p;
        }
        p = p << 1;
      }
    };
    bool some_btn_active = true;
    while (some_btn_active) {
      some_btn_active = false;
      for (uint8_t i = 0 ; i < this->num_btns; i++) {
        some_btn_active |= digitalRead(this->pin_btns[i]);
      }
    }

    return value;
  }

  uint8_t Buttons::wait_first_press() const {
    uint16_t buttons_pressed = this->wait_press();
    for (uint8_t i = 0 ; i < this->num_btns; i++) {
      if (buttons_pressed % 2) {
        return i;
      }
      buttons_pressed = buttons_pressed >> 1;
    }
    return NO_BUTTON;
  }

}
