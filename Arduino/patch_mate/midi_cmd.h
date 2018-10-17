
#pragma once

namespace patch_mate {

struct midi_cmd {
  enum {
      CMD_BIT         = 0x80U,
      CMD_NOTE_OFF    = 0x80U,
      CMD_NOTE_ON     = 0x90U,
      CMD_CTRL_CHANGE = 0xB0U,
      CMD_PROG_CHANGE = 0xC0U,
  };

  uint8_t cmd_[3];
  uint8_t size_ = 0;

  uint8_t command() const { return cmd_[0] & 0xF0U; }
  uint8_t channel() const { return cmd_[0] & 0x0FU; }
  uint8_t controller() const { return cmd_[1]; }
  uint8_t value() const { return cmd_[2]; }
  uint8_t program() const { return cmd_[1]; }

  operator bool() const {
      return
          (command() == CMD_PROG_CHANGE && size_ == 2) ||
          (command() == CMD_CTRL_CHANGE && size_ == 3) ||
          (command() == CMD_NOTE_OFF && size_ == 3) ||
          (command() == CMD_NOTE_ON && size_ == 3);
  }

  void reset() { size_ = 0; }

  template<typename STREAM>
  bool read(STREAM& s) {
      uint8_t b;

      while (!*this && s.available()) {
          b = s.read();

          debug(1, "midi read: ", b);

          if ((b & CMD_BIT) == CMD_BIT) {
              cmd_[0] = b;
              size_ = 1;
              continue;
          }

          if (size_ == 0) { continue; }
          if (size_ >= 3) { size_ = 0; continue; }

          cmd_[size_++] = b;
      }

      return *this;
  }
};

}
