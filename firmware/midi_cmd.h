
#pragma once

namespace patch_mate {

struct midi_cmd_t {
    enum {
        CMD_BIT           = 0x80U,

        CMD_NOTE_OFF      = 0x80U,
        CMD_NOTE_ON       = 0x90U,
        CMD_KEY_PRESSURE  = 0xA0U,
        CMD_CTRL_CHANGE   = 0xB0U,
        CMD_PROG_CHANGE   = 0xC0U,
        CMD_CHAN_PRESSURE = 0xD0U,
        CMD_PITCH_CHANGE  = 0xE0U,
        CMD_SYS_EX        = 0xF0U,
        CMD_SYS_EX_END    = 0xF7U,
    };

    enum {
        CTRL_ALL_SOUND_OFF  = 120,
        CTRL_RESET_ALL      = 121,
        CTRL_LOCAL_CONTROL  = 122,
    };

    midi_cmd_t() = default;
    midi_cmd_t(uint8_t ch, uint8_t cmd, uint8_t val) :
        size_(2)
    {
        cmd_[0] = cmd | (ch & 0x0FU);
        cmd_[1] = val & 0x7FU;
    }

    midi_cmd_t(uint8_t ch, uint8_t cmd, uint8_t ctl, uint8_t val) :
        size_(3)
    {
        cmd_[0] = cmd | (ch & 0x0FU);
        cmd_[1] = ctl & 0x7FU;
        cmd_[2] = val & 0x7FU;
    }

    uint8_t command() const { return cmd_[0] & 0xF0U; }
    uint8_t channel() const { return cmd_[0] & 0x0FU; }
    uint8_t controller() const { return cmd_[1]; }
    uint8_t value() const { return cmd_[2]; }
    uint8_t program() const { return cmd_[1]; }
    uint8_t key() const { return cmd_[1]; }
    uint16_t pitch() const { return cmd_[1] | (cmd_[2] << 7); }

    uint8_t size() const { return size_; }

    operator bool() const {
        static const uint8_t correct_size[] = {
            3, /* CMD_NOTE_OFF */
            3, /* CMD_NOTE_ON */
            3, /* CMD_KEY_PRESSURE */
            3, /* CMD_CTRL_CHANGE */
            2, /* CMD_PROG_CHANGE */
            2, /* CMD_CHAN_PRESSURE */
            3, /* CMD_PITCH_CHANGE */
            0, /* CMD_SYS_EX */
        };

        return size_ == correct_size[ (cmd_[0] >> 4) & 0x07 ];
    }

    operator const uint8_t*() const { return cmd_; }

    uint8_t operator[](uint8_t i) const { return cmd_[i]; }

    bool operator==(const midi_cmd_t& cmd) const {
        return size_ == cmd.size_ && memcmp(cmd_, cmd.cmd_, size_) == 0;
    }

    void reset() { size_ = 0; }

    template<typename STREAM>
    bool read(STREAM& s) {
        uint8_t b;

        while (!*this && s.available()) {
            b = s.read();

            read(b);
        }

        return *this;
    }

    template<typename STREAM>
    void write(STREAM& s) {
#if defined(DEBUG)
        for (uint8_t i = 0; i < size_; ++i) {
            debug(4, "midi write: ", cmd_[i]);
        }
#endif

        s.write(cmd_, size_);
    }

    bool read(uint8_t b) {
        debug(4, "midi read: ", b);

        if ((b & CMD_BIT) == CMD_BIT) {
            cmd_[0] = b;
            size_ = 1;

            return *this;
        }

        if (size_ > 0 && size_ < MAX_SIZE) {
            cmd_[size_++] = b;
        }

        return *this;
    }

private:
    enum {
        MAX_SIZE = 3
    };

    uint8_t cmd_[MAX_SIZE];
    uint8_t size_ = 0;
};

}
