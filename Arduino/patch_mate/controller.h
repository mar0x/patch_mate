
#pragma once

#include "config.h"
#include "debug.h"

#include "artl/button.h"
#include "artl/encoder.h"
#include "artl/timer.h"
#include "artl/tc.h"

#include "midi_cmd.h"
#include "loop_state.h"
#include "program_title.h"
#include "settings.h"

#if defined(ARDUINO_ARCH_AVR)
#include "artl/yield.h"
#endif

#include "lcd.h"

#include "midi_mon.h"

#if defined(WITH_SNAKE)
#include "snake/game.h"
#endif

#if defined(WITH_HERO)
#include "hero/game.h"
#endif

namespace patch_mate {

struct controller {

    enum {
        MAX_CURSOR_POS = 15,
    };

    controller();

    void setup();
    void loop();

    bool read_eeprom();
    void reset_eeprom();

    void process_midi_cmd();
    void midi_out(const midi_cmd& cmd);

    void set_program(uint8_t prog);
    void set_loop(uint8_t loop, bool val);
    void send_loop(uint8_t loop, bool val);

    bool led4loop() const;

    void on_down(uint8_t, bool, unsigned long);
    void on_hold(uint8_t, unsigned long);
    void on_rotate(short, unsigned long);

    void mute(unsigned long t);
    void on_mute_timer(unsigned long t);

    void start_edit(unsigned int value, unsigned int min, unsigned int max, unsigned int step = 1);
    void print_edit_value();
    void set_edit_loop(uint8_t edit_loop);
    void set_mode(uint8_t m, unsigned long t);

    void show_cursor(unsigned long t);
    void hide_cursor();

    void set_dirty(bool dirty);

    void show_master(bool master);

    struct button_handler : public artl::default_button_handler {
        button_handler(uint8_t id) : id_(id) { }

        void on_down(bool, unsigned long);
        void on_hold(unsigned long);

        uint8_t id_;
    };

    using button_type = artl::button<button_handler>;


    struct encoder_handler : public artl::default_encoder_handler {
        void on_rotate(short, unsigned long);
    };

    using encoder_type = artl::encoder<encoder_handler>;


    struct in_traits {
        using button_handler = controller::button_handler;
        using encoder_handler = controller::encoder_handler;

        enum {
            LOOP0 = 0,
            STORE = 8,
            LEFT,
            RIGHT
        };
    };

    enum {
        STORE = in_traits::STORE,
        LEFT = in_traits::LEFT,
        RIGHT = in_traits::RIGHT
    };

    struct mute_timer_callback {
        void operator ()(unsigned long);
    };

    using mute_timer = artl::timer<mute_timer_callback>;


    struct hide_cursor_timer_callback {
        void operator ()(unsigned long);
    };

    using hide_cursor_timer = artl::timer<hide_cursor_timer_callback>;


    struct store_blink_timer_callback {
        void operator ()(unsigned long);

        void start(unsigned long t, uint8_t count = 3, unsigned long period = 100);
        void stop();

        uint8_t rest_;
        bool led_on_;
        unsigned long period_;
    };

    using store_blink_timer = artl::timer<store_blink_timer_callback>;


    enum {
        MODE_INITIAL,
        MODE_NORMAL,
        MODE_SETTINGS_FIRST,
        MODE_SETTINGS_CHANNEL = MODE_SETTINGS_FIRST,
        MODE_SETTINGS_CTRL_IN,
        MODE_SETTINGS_CTRL_OUT,
        MODE_SETTINGS_PROG_OUT,
        MODE_SETTINGS_MIDI_FWD,
        MODE_SETTINGS_MUTE_DELAY,
        MODE_SETTINGS_HIDE_CURSOR,
#if defined(DEBUG)
        MODE_SETTINGS_USB_DEBUG,
#endif
        MODE_SETTINGS_FACTORY_RESET,
        MODE_SETTINGS_LAST = MODE_SETTINGS_FACTORY_RESET,
        MODE_MIDI_IN_MONITOR,
        MODE_MIDI_OUT_MONITOR,
        MODE_ABOUT,
        MODE_UPTIME,
        MODE_LOOP_COUNT,
        MODE_INPUT_COUNT,
        MODE_PC_COUNT,
#if defined(WITH_SNAKE)
        MODE_GAME_SNAKE,
#endif
#if defined(WITH_HERO)
        MODE_GAME_HERO,
#endif
        MODE_MAX,

        MODE_LED_IGNORE_LOOP_MASK = (1 << MODE_SETTINGS_CTRL_IN) |
                                    (1 << MODE_SETTINGS_CTRL_OUT) |
                                    (1 << MODE_MIDI_IN_MONITOR) |
                                    (1 << MODE_MIDI_OUT_MONITOR),
    };

    in<in_traits> in_;
    out out_;

    mute_timer mute_timer_;
    bool on_mute_ = true;
    bool wait_mute_ = false;

    store_blink_timer store_blink_timer_;

    uint8_t mode_ = MODE_INITIAL;
    settings settings_;

    uint8_t edit_loop_;
    unsigned int edit_value_;
    unsigned int edit_value_master_;
    unsigned int edit_value_min_;
    unsigned int edit_value_max_;
    unsigned int edit_value_step_;

    midi_cmd midi_cmd_;
    midi_cmd last_out_cmd_;

    bool dirty_ = false;
    bool bad_magic_;

    uint8_t program_ = 0;

    loop_state loop_state_;
    loop_state loop_state_master_;

    program_title program_title_;
    program_title program_title_master_;

    uint8_t cursor_pos_ = 0;
    hide_cursor_timer hide_cursor_timer_;

    unsigned long uptime_;
    unsigned long loop_count_ = 0;
    unsigned long pc_count_ = 0;
    unsigned long input_count_ = 0;
    unsigned long last_out_ms_ = 0;

    midi_mon midi_mon_[2];

#if defined(WITH_SNAKE)
    snake::Game snake_;
#endif
#if defined(WITH_HERO)
    hero::Game hero_;
#endif


    volatile bool enable_input_;
    using input_tc = artl::tc<1>;

    bool show_master_;
};

controller controller_;

#if defined(ISR) && defined(PCINT0_vect)
ISR(PCINT0_vect) {
    ++controller_.pc_count_;
}
#endif

#if defined(ISR) && defined(TIMER1_COMPA_vect)
ISR(TIMER1_COMPA_vect)
{
    controller_.enable_input_ = true;
}
#endif

inline void
print_number(char *buf, uint8_t size, unsigned long n)
{
    char *p = buf + size;

    do {
        char c = n % 10;
        n /= 10;

        *--p = c + '0';
    } while (n);

    while (p > buf) { *--p = ' '; }
}

template<typename T> T rotate(T v, unsigned int max, int dir) {
    T res = (v + max + dir) % max;
    debug(2, "rotate: ", v, ", ", max, ", ", dir, " = ", res);
    return res;
}

template<typename T> T rotate(T v, unsigned int min, unsigned int max, int dir) {
    unsigned int range = max - min;
    T res = min + (v - min + range + dir) % range;
    debug(2, "rotate: ", v, ", ", min, ", ", max, ", ", dir, " = ", res);
    return res;
}

inline void
yield()
{
#if defined(ARDUINO_ARCH_AVR)
    artl::yield();
#endif
}

inline
controller::controller()
{
}

inline void
controller::setup() {
#if defined(HAVE_HWSERIAL1) && defined(DEBUG)
    Serial.begin(115200);
#endif
    in_.setup();
    out_.setup();

    unsigned long t = millis();

    midi_mon_[0].setup(t);
    midi_mon_[1].setup(t);

    lcd_setup();

    out_.mute();
    on_mute_ = true;

    if (read_eeprom()) {
        set_mode(MODE_NORMAL, t);
        set_program(0);
    } else {
        set_mode(MODE_SETTINGS_FACTORY_RESET, t);
    }

    mute(t);

    enable_input_ = true;

#if defined(ARDUINO_ARCH_AVR)
    input_tc().setup(0, 0, 4, input_tc::cs::presc_256);
    input_tc().ocra() = 200;
    input_tc().cnt() = 0;
    input_tc().oca().enable();
#endif

    yield();
}

inline void
controller::loop() {
    unsigned long t = millis();

    ++loop_count_;

    while (!midi_cmd_ && in_.midi().available()) {
        uint8_t b = in_.midi().read();

        if (midi_mon_[0].active()) {
            midi_mon_[0].read(b);
        }

        midi_cmd_.read(b);
    }

    if (midi_cmd_) {
        process_midi_cmd();
    }

    in_.update(t, enable_input_);

#if defined(ISR) && defined(TIMER1_COMPA_vect)
    if (!enable_input_) {
        yield();
        return;
    }

    enable_input_ = false;
#endif

    ++input_count_;

    mute_timer_.update(t);
    hide_cursor_timer_.update(t);
    store_blink_timer_.update(t);

    switch (mode_) {
    case MODE_MIDI_IN_MONITOR:
        midi_mon_[0].update(t);
        break;

    case MODE_MIDI_OUT_MONITOR:
        midi_mon_[1].update(t);
        break;

    case MODE_UPTIME: {
        unsigned long s = t / 1000;
        if (uptime_ < s) {
            uptime_ = s;
            print_number(lcd_buf[1], 12, uptime_);
            lcd_update(0, 1, 12);
        }
        break;
    }
    case MODE_LOOP_COUNT: {
        unsigned long s = loop_count_ / 1000;
        if (uptime_ < s) {
            uptime_ = s;
            print_number(lcd_buf[1], 14, uptime_);
            lcd_update(0, 1, 14);
        }
        break;
    }
    case MODE_INPUT_COUNT: {
        unsigned long s = input_count_ / 1000;
        if (uptime_ < s) {
            uptime_ = s;
            print_number(lcd_buf[1], 14, uptime_);
            lcd_update(0, 1, 14);
        }
        break;
    }
    case MODE_PC_COUNT: {
        unsigned long s = pc_count_;
        if (uptime_ < s) {
            uptime_ = s;
            print_number(lcd_buf[1], 14, uptime_);
            lcd_update(0, 1, 14);
        }
        break;
    }
#if defined(WITH_SNAKE)
    case MODE_GAME_SNAKE:
        snake_.update(t);
        break;
#endif
#if defined(WITH_HERO)
    case MODE_GAME_HERO:
        hero_.update(t);
        break;
#endif
    }

    if (!store_blink_timer_.active()) {
        out_.store_led(dirty_);
    }

    if (out_.relay_changed()) {
        out_.commit_led();
        mute(t);
    } else {
        out_.commit();
    }

    yield();
}

static const char eemagic[4] = { 'M', 'P', 'M', '3' };

inline bool
controller::read_eeprom() {
    char magic_test[sizeof(eemagic)];

    EEPROM.get(0, magic_test);

    if (memcmp(eemagic, magic_test, sizeof(eemagic)) != 0) {
        bad_magic_ = true;
        return false;
    }

    int s = loop_state::setup(sizeof(eemagic));

    s = settings::setup(s);
    settings_.read();

    program_title::setup(s);

    bad_magic_ = false;
#if defined(DEBUG)
    debug_level_ = settings_.usb_debug;
#endif
    return true;
}

inline void
controller::reset_eeprom() {
    EEPROM.put(0, eemagic);
    int s = loop_state::setup(sizeof(eemagic));

    for (uint8_t i = 0; i < MAX_PROGRAMS; i++) {
        loop_state().write(i);
    }

    s = settings::setup(s);

    settings_ = settings();
    settings_.write();

    EEPROM.update(s, 0);
    program_title::setup(s);

    bad_magic_ = false;
}

inline void
controller::process_midi_cmd() {
    if (midi_cmd_.command() == midi_cmd::CMD_PROG_CHANGE &&
        midi_cmd_.channel() == settings_.midi_channel) {
        set_program(midi_cmd_.program());
    }

    if (midi_cmd_.command() == midi_cmd::CMD_CTRL_CHANGE &&
        midi_cmd_.channel() == settings_.midi_channel) {
        for(uint8_t i = 0; i < MAX_LOOP; i++) {
            if (settings_.midi_loop_ctrl_in[i] == midi_cmd_.controller()) {
                set_loop(i, midi_cmd_.value() != 0);
            }
        }
    }

    if (settings_.midi_out & settings::MIDI_OUT_ALL) {
        midi_out(midi_cmd_);
    }

    midi_cmd_.reset();
}

inline void
controller::midi_out(const midi_cmd& cmd) {
    if (midi_mon_[1].active()) {
        for (uint8_t i = 0; i < cmd.size(); i++) {
            midi_mon_[1].read(cmd[i]);
        }
    }

    last_out_cmd_ = cmd;
    last_out_ms_ = millis();

    out_.midi().write(cmd, cmd.size());
}

inline void
controller::set_program(uint8_t prog) {
    debug(1, "set_program: ", program_, " -> ", prog);

    program_ = prog;

    loop_state_.read(prog);
    loop_state_master_ = loop_state_;

    if (settings_.midi_out & settings::MIDI_OUT_PROG) {
        midi_cmd cmd(settings_.midi_channel, midi_cmd::CMD_PROG_CHANGE, program_);

        midi_out(cmd);
    }

    for(uint8_t i = 0; i < MAX_LOOP; i++) {
        out_.loop_relay(i, loop_state_[i]);
        send_loop(i, loop_state_[i]);
    }

    program_title_.read(prog);
    program_title_master_ = program_title_;

    if (mode_ == MODE_NORMAL) {
        set_dirty(false);

        print_number(lcd_buf[0], 3, program_ + 1);
        memcpy(lcd_buf[1], program_title_.title, 15);

        hide_cursor();

        lcd_update(0, 0, 3);
        lcd_update(0, 1, 15);
    }

    // adjust loop led for all modes except MODE_LED_IGNORE_LOOP_MASK
    if (led4loop()) {
        out_.loop_led(loop_state_);
    }
}

inline void
controller::set_loop(uint8_t i, bool v) {
    debug(1, "set_loop: ", i, " ", loop_state_[i], " -> ", v);

    out_.loop_relay(i, v);

    // send controller change MIDI command if configured
    send_loop(i, v);

    // adjust loop led for all modes except MODE_LED_IGNORE_LOOP_MASK
    if (led4loop()) {
        out_.loop_led(i, v);
    }

    loop_state_.set(i, v);

    if (mode_ == MODE_NORMAL) {
        set_dirty((loop_state_ != loop_state_master_) ||
            (program_title_ != program_title_master_));
    }
}

inline void
controller::send_loop(uint8_t i, bool v) {
    if (settings_.midi_loop_ctrl_out[i] <= 127) {
        midi_cmd cmd(settings_.midi_channel, midi_cmd::CMD_CTRL_CHANGE,
            settings_.midi_loop_ctrl_out[i], v ? 127 : 0);

        midi_out(cmd);
    }
}

inline bool
controller::led4loop() const {
    return ((1 << mode_) & MODE_LED_IGNORE_LOOP_MASK) == 0;
}

inline void
controller::on_down(uint8_t i, bool down, unsigned long t) {
    if (!down) {
        debug(5, "on_down: release ", i);

        if (i == RIGHT && show_master_) {
            show_master(false);
        }

        return; // no reaction on key UP
    }

    debug(4, "on_down: ", i);

    // change mode: hold LEFT and press LOOP
    if (i < MAX_LOOP && in_.left().down()) {
        switch(i) {
            case 0: set_mode(MODE_SETTINGS_CHANNEL, t); break;
            case 1: set_mode(MODE_SETTINGS_CTRL_IN, t); break;
            case 2: set_mode(MODE_SETTINGS_CTRL_OUT, t); break;
            case 3: set_mode(MODE_SETTINGS_PROG_OUT, t); break;
            case 4: set_mode(MODE_SETTINGS_MIDI_FWD, t); break;
            case 6: set_mode(MODE_MIDI_IN_MONITOR, t); break;
#if defined(DEBUG)
            case 7: set_mode(MODE_SETTINGS_USB_DEBUG, t); break;
#endif
        }
        return;
    }

    // change mode ex: hold RIGHT and press LOOP
    if (i < MAX_LOOP && in_.right().down()) {
        switch(i) {
            case 0: set_mode(MODE_ABOUT, t); break;
            case 1: set_mode(MODE_UPTIME, t); break;
#if defined(WITH_SNAKE)
            case 2: set_mode(MODE_GAME_SNAKE, t); break;
#endif
#if defined(WITH_HERO)
            case 3: set_mode(MODE_GAME_HERO, t); break;
#endif
            case 4: set_mode(MODE_SETTINGS_MUTE_DELAY, t); break;
            case 5: set_mode(MODE_SETTINGS_HIDE_CURSOR, t); break;
            case 6: set_mode(MODE_MIDI_OUT_MONITOR, t); break;
        }
        return;
    }

    if (mode_ == MODE_NORMAL) {
        // toggle LOOP
        if (i < MAX_LOOP && in_.left().up() && in_.right().up()) {
            bool v = loop_state_[i];

            debug(2, "toggle loop ", i, " ", v, " -> ", !v);
            set_loop(i, !v);

            return;
        }

        // move cursor LEFT
        if (i == LEFT && in_.right().up()) {
            cursor_pos_ = rotate(cursor_pos_, MAX_CURSOR_POS, -1);
            show_cursor(t);

            return;
        }

        // move cursor RIGHT
        if (i == RIGHT && in_.left().up()) {
            cursor_pos_ = rotate(cursor_pos_, MAX_CURSOR_POS, 1);
            show_cursor(t);

            return;
        }

        // cancel changes and stay in NORMAL
        // hold RIGHT then press LEFT or
        // hold LEFT then press RIGHT
        if ((i == LEFT && in_.right().hold()) || (i == RIGHT && in_.left().hold()))
        {
            if (dirty_) { set_program(program_); }
            return;
        }

        return;
    }

    // cancel changes and back to NORMAL mode
    // hold RIGHT then press LEFT or
    // hold LEFT then press RIGHT
    if ((i == LEFT && in_.right().hold()) || (i == RIGHT && in_.left().hold()))
    {
        set_mode(MODE_NORMAL, t);
        return;
    }

    if (mode_ == MODE_SETTINGS_CTRL_IN || mode_ == MODE_SETTINGS_CTRL_OUT) {
        bool loop_changed = false;

        // controller -> LOOP mapping
        if (i == LEFT && in_.right().up()) {
            set_edit_loop(rotate(edit_loop_, 1, MAX_LOOP + 1, -1));
            loop_changed = true;
        }

        if (i == RIGHT && in_.left().up()) {
            set_edit_loop(rotate(edit_loop_, 1, MAX_LOOP + 1, 1));
            loop_changed = true;
        }

        if (i < MAX_LOOP && in_.left().up() && in_.right().up()) {
            if (i + 1 != edit_loop_) {
                set_edit_loop(i + 1);
                loop_changed = true;
            }
        }

        if (loop_changed) {
            lcd_update(0, 1);
            lcd_update(13, 1, 3);
        }

        return;
    }

#if defined(WITH_SNAKE)
    if (mode_ == MODE_GAME_SNAKE) {
        if (i == LEFT && in_.right().up()) {
            snake_.turn_left(t);
        }

        if (i == RIGHT && in_.left().up()) {
            snake_.turn_right(t);
        }
    }
#endif

#if defined(WITH_HERO)
    if (mode_ == MODE_GAME_HERO) {
        if (i == STORE) {
            hero_.jump(t);
        }
    }
#endif

    if (mode_ == MODE_MIDI_IN_MONITOR ||
        mode_ == MODE_MIDI_OUT_MONITOR) {
        uint8_t m = mode_ - MODE_MIDI_IN_MONITOR;
        if (i == STORE) {
            // enable/pause capturing
            set_dirty(midi_mon_[m].toggle_pause(t));
        }

        if (i == LEFT && in_.right().up()) {
            midi_mon_[m].mode_prev(t);

            out_.loop_led(1 << midi_mon_[m].mode());
        }

        if (i == RIGHT && in_.left().up()) {
            midi_mon_[m].mode_next(t);

            out_.loop_led(1 << midi_mon_[m].mode());
        }

        if (i < MAX_LOOP && i < midi_mon::MODE_MAX && in_.left().up() && in_.right().up()) {
            midi_mon_[m].mode(i, t);

            out_.loop_led(1 << i);
        }
    }
}

inline void
controller::on_hold(uint8_t i, unsigned long t) {
    debug(4, "on_hold: ", i);

    if (i == STORE && in_.left().hold() && in_.right().hold()) {
        set_mode(MODE_SETTINGS_FACTORY_RESET, t);
        return;
    }

    if (mode_ == MODE_NORMAL) {
        // hold STORE
        if (i == STORE) {
            if (!dirty_) { return; }

            if (bad_magic_) {
                set_mode(MODE_SETTINGS_FACTORY_RESET, t);
                return;
            }

            // commit program change
            loop_state_.write(program_);
            program_title_.write(program_);

            loop_state_master_ = loop_state_;
            program_title_master_ = program_title_;

            store_blink_timer_.start(t);
            set_dirty(false);

            return;
        }

        if (i == RIGHT && in_.left().up()) {
            show_master(true);
        }

        if ((i == LEFT && in_.right().down()) || (i == RIGHT && in_.left().down())) {
            // cancel changes
            if (dirty_) { set_program(program_); }
        }

        return;
    }

    if (i == STORE) {
        if (mode_ >= MODE_SETTINGS_FIRST &&
            mode_ <= MODE_SETTINGS_LAST) {
            // No changes made - switch back to NORMAL;
            if (!dirty_) {
                set_mode(MODE_NORMAL, t);
                return;
            }

            // Changes made, but EEPROM not ready - confirm EEPROM Reset first;
            if (bad_magic_ && mode_ != MODE_SETTINGS_FACTORY_RESET) {
                set_mode(MODE_SETTINGS_FACTORY_RESET, t);
                return;
            }
        }

        switch (mode_) {
        case MODE_SETTINGS_CHANNEL:
            settings_.midi_channel = edit_value_ - 1;
            settings_.write(settings_.midi_channel);
            break;

        case MODE_SETTINGS_CTRL_IN:
            settings_.midi_loop_ctrl_in[edit_loop_ - 1] = edit_value_;
            settings_.write(settings_.midi_loop_ctrl_in[edit_loop_ - 1]);

            edit_value_master_ = edit_value_;
            set_dirty(false);

            // stay in MODE_SETTINGS_CTRL_IN for convenience
            store_blink_timer_.start(t);
            return;

        case MODE_SETTINGS_CTRL_OUT:
            settings_.midi_loop_ctrl_out[edit_loop_ - 1] = edit_value_;
            settings_.write(settings_.midi_loop_ctrl_out[edit_loop_ - 1]);

            edit_value_master_ = edit_value_;
            set_dirty(false);

            // stay in MODE_SETTINGS_CTRL_OUT for convenience
            store_blink_timer_.start(t);
            return;

        case MODE_SETTINGS_PROG_OUT:
            if (edit_value_) {
                settings_.midi_out |= settings::MIDI_OUT_PROG;
            } else {
                settings_.midi_out &= ~settings::MIDI_OUT_PROG;
            }
            settings_.write(settings_.midi_out);
            break;

        case MODE_SETTINGS_MIDI_FWD:
            if (edit_value_) {
                settings_.midi_out |= settings::MIDI_OUT_ALL;
            } else {
                settings_.midi_out &= ~settings::MIDI_OUT_ALL;
            }
            settings_.write(settings_.midi_out);
            break;

        case MODE_SETTINGS_MUTE_DELAY:
            settings_.mute_delay_ms = edit_value_;
            settings_.write(settings_.mute_delay_ms);
            break;

        case MODE_SETTINGS_HIDE_CURSOR:
            settings_.hide_cursor_delay_s = edit_value_;
            settings_.write(settings_.hide_cursor_delay_s);
            break;

        case MODE_SETTINGS_FACTORY_RESET:
            reset_eeprom();
            set_mode(MODE_NORMAL, t);
            set_program(0);
            store_blink_timer_.start(t);
            return;

#if defined(DEBUG)
        case MODE_SETTINGS_USB_DEBUG:
            settings_.usb_debug = edit_value_;
            settings_.write(settings_.usb_debug);
            debug_level_ = edit_value_;
            break;
#endif
        }

        set_mode(MODE_NORMAL, t);
        store_blink_timer_.start(t);

        return;
    }

    // cancel changes and back to NORMAL mode
    // hold RIGHT then press LEFT or
    // hold LEFT then press RIGHT
    if ((i == LEFT && in_.right().down()) || (i == RIGHT && in_.left().down())) {
        set_mode(MODE_NORMAL, t);
    }
}

/* Available alphabet:
  ABCDEFGHIJKLMNOPRSTUVWXYZ[\]^_
  !"#$%&'()*+,-./
  :;<=>?@
  0123456789
*/

inline char
rotate_char(char c, short dir) {
    if (dir > 0) {
        if (c == ' ') return 'A';
        if (c >= '0' && c < '9') return c + 1;
        if (c >= 'A' && c < '_') return c + 1;
        if (c >= '!' && c < '/') return c + 1;
        if (c >= ':' && c < '@') return c + 1;
        if (c == '_') return '!';
        if (c == '/') return ':';
        if (c == '@') return '0';
        if (c == '9') return ' ';
        return ' ';
    }

    if (c == ' ') return '9';
    if (c > '0' && c <= '9') return c - 1;
    if (c > 'A' && c <= '_') return c - 1;
    if (c > '!' && c <= '/') return c - 1;
    if (c > ':' && c <= '@') return c - 1;
    if (c == '0') return '@';
    if (c == ':') return '/';
    if (c == '!') return '_';
    if (c == 'A') return ' ';
    return ' ';
}

inline void
controller::on_rotate(short dir, unsigned long t) {
    debug(4, "on_rotate: ", dir);

    // rotate on RIGHT hold - change mode
    if (in_.right().hold()) {
        uint8_t m = rotate(mode_, MODE_NORMAL, MODE_MAX, dir);

        set_mode(m, t);
        return;
    }

    switch(mode_) {
    case MODE_NORMAL: {

        // rotate on LEFT hold - change program
        if (in_.left().hold()) {
            set_program(rotate(program_, MAX_PROGRAMS, dir));

            return;
        }

        char& c = program_title_.title[cursor_pos_];
        c = rotate_char(c, dir);
        lcd_buf[1][cursor_pos_] = c;

        lcd.setCursor(cursor_pos_, 1);
        lcd.write(c);
        show_cursor(t);

        set_dirty((loop_state_ != loop_state_master_) ||
            (program_title_ != program_title_master_));

        return;
    }

    case MODE_SETTINGS_CHANNEL:
    case MODE_SETTINGS_CTRL_IN:
    case MODE_SETTINGS_CTRL_OUT:
    case MODE_SETTINGS_PROG_OUT:
    case MODE_SETTINGS_MIDI_FWD:
    case MODE_SETTINGS_MUTE_DELAY:
    case MODE_SETTINGS_HIDE_CURSOR:
#if defined(DEBUG)
    case MODE_SETTINGS_USB_DEBUG:
#endif
        edit_value_ = rotate(edit_value_, edit_value_min_,
            edit_value_max_ + edit_value_step_, edit_value_step_ * dir);

        set_dirty(edit_value_ != edit_value_master_);

        print_edit_value();

        lcd_update(13, 1, 3);
        break;

#if defined(WITH_SNAKE)
    case MODE_GAME_SNAKE:
        if (dir > 0) {
            snake_.turn_right(t);
        } else {
            snake_.turn_left(t);
        }
        break;
#endif

    case MODE_MIDI_IN_MONITOR:
    case MODE_MIDI_OUT_MONITOR: {
        uint8_t m = mode_ - MODE_MIDI_IN_MONITOR;
        if (dir > 0) {
            midi_mon_[m].scroll_down(t);
        } else {
            midi_mon_[m].scroll_up(t);
        }
        break;
    }

    }
}


inline void
controller::mute(unsigned long t) {
    if (wait_mute_) return;

    out_.mute();

    wait_mute_ = true;
    mute_timer_.schedule(t + (on_mute_ ? 0 : settings_.mute_delay_ms));
}

inline void
controller::on_mute_timer(unsigned long t) {
    if (wait_mute_) {
        on_mute_ = true;

        out_.commit();

        wait_mute_ = false;
        mute_timer_.schedule(t + settings_.mute_delay_ms);
    } else {
        on_mute_ = false;
        out_.unmute();
    }
}

inline void
controller::start_edit(unsigned int value, unsigned int min, unsigned int max, unsigned int step) {
    edit_value_master_ = value;
    edit_value_ = edit_value_master_;
    edit_value_min_ = min;
    edit_value_max_ = max;
    edit_value_step_ = step;

    print_edit_value();
    set_dirty(false);
}

inline void
controller::print_edit_value() {
    if (edit_value_ > 127 &&
          (mode_ == MODE_SETTINGS_CTRL_IN ||
           mode_ == MODE_SETTINGS_CTRL_OUT)) {
        memcpy(lcd_buf[1] + 13, "OFF", 3);
    } else if (edit_value_ == 0 &&
          (mode_ == MODE_SETTINGS_PROG_OUT ||
           mode_ == MODE_SETTINGS_MIDI_FWD
#if defined(DEBUG)
           || mode_ == MODE_SETTINGS_USB_DEBUG
#endif
          )) {
        memcpy(lcd_buf[1] + 13, "OFF", 3);
    } else if (edit_value_ != 0 &&
        (mode_ == MODE_SETTINGS_PROG_OUT ||
        mode_ == MODE_SETTINGS_MIDI_FWD)) {
        memcpy(lcd_buf[1] + 13, "ON ", 3);
    } else {
        print_number(lcd_buf[1] + 13, 3, edit_value_);
    }
}

inline void
controller::set_edit_loop(uint8_t edit_loop) {
    edit_loop_ = edit_loop;

    switch (mode_) {
    case MODE_SETTINGS_CTRL_IN:
        start_edit(settings_.midi_loop_ctrl_in[edit_loop_ - 1], 0, 128);
        break;
    case MODE_SETTINGS_CTRL_OUT:
        start_edit(settings_.midi_loop_ctrl_out[edit_loop_ - 1], 0, 128);
        break;
    }

    print_number(lcd_buf[1], 1, edit_loop_);

    out_.loop_led(1 << (edit_loop_ - 1));
}

inline void
controller::set_mode(uint8_t m, unsigned long t) {
    debug(1, "set_mode: ", mode_, " -> ", m);

    if (m == mode_) return;

    if (mode_ == MODE_SETTINGS_FACTORY_RESET) {
        store_blink_timer_.stop();
    }

    if (mode_ == MODE_MIDI_IN_MONITOR ||
        mode_ == MODE_MIDI_OUT_MONITOR) {
        midi_mon_[mode_ - MODE_MIDI_IN_MONITOR].hide(t);
    }

    mode_ = m;

    set_dirty(false);

    if (mode_ >= MODE_SETTINGS_FIRST &&
        mode_ <= MODE_SETTINGS_LAST) {
        memcpy(lcd_buf[0], " Settings       ", 16);
    }

    switch (mode_) {
    case MODE_INITIAL:
        break;

    case MODE_NORMAL:
        set_dirty((loop_state_ != loop_state_master_) ||
            (program_title_ != program_title_master_));

        print_number(lcd_buf[0], 3, program_ + 1);
        memcpy(lcd_buf[0] + 3, " MIDI        ", 13);
        memcpy(lcd_buf[1], program_title_.title, 15);
        lcd_buf[1][15] = ' ';
        break;

    case MODE_SETTINGS_CHANNEL:
        start_edit(settings_.midi_channel + 1, 1, 16);
        memcpy(lcd_buf[1], "MIDI Channel ", 13);
        break;

    case MODE_SETTINGS_CTRL_IN:
        set_edit_loop(1);
        memcpy(lcd_buf[1] + 1, " Ctrl IN    ", 12);
        break;

    case MODE_SETTINGS_CTRL_OUT:
        set_edit_loop(1);
        memcpy(lcd_buf[1] + 1, " Ctrl OUT   ", 12);
        break;

    case MODE_SETTINGS_PROG_OUT:
        start_edit((settings_.midi_out & settings::MIDI_OUT_PROG) != 0, 0, 1, 1);
        memcpy(lcd_buf[1], "Prog Chg OUT ", 13);
        break;

    case MODE_SETTINGS_MIDI_FWD:
        start_edit((settings_.midi_out & settings::MIDI_OUT_ALL) != 0, 0, 1, 1);
        memcpy(lcd_buf[1], "MIDI Forward ", 13);
        break;

    case MODE_SETTINGS_MUTE_DELAY:
        start_edit(settings_.mute_delay_ms, 10, 900, 10);
        memcpy(lcd_buf[0] + 14, "ms", 2);
        memcpy(lcd_buf[1], " Mute delay  ", 13);
        break;

    case MODE_SETTINGS_HIDE_CURSOR:
        start_edit(settings_.hide_cursor_delay_s, 1, 60);
        lcd_buf[0][15] = 's';
        memcpy(lcd_buf[1], " Hide cursor ", 13);
        break;

    case MODE_SETTINGS_FACTORY_RESET:
        memcpy(lcd_buf[1], " Factory Reset  ", 16);
        set_dirty(true);
        store_blink_timer_.start(t, 0, 150);
        break;

#if defined(DEBUG)
    case MODE_SETTINGS_USB_DEBUG:
        start_edit(settings_.usb_debug, 0, 9);
        memcpy(lcd_buf[1], "USB Debug    ", 13);
        break;
#endif

    case MODE_MIDI_IN_MONITOR:
        midi_mon_[0].show(t);
        memcpy(lcd_buf[0], "MIDI IN Monitor ", 16);
        memcpy(lcd_buf[1], "0               ", 16);
        set_dirty(midi_mon_[0].active());  // turn 'STORE' led 'ON', capture enabled

        out_.loop_led(1 << midi_mon_[0].mode());
        break;

    case MODE_MIDI_OUT_MONITOR:
        midi_mon_[1].show(t);
        memcpy(lcd_buf[0], "MIDI OUT Monitor", 16);
        memcpy(lcd_buf[1], "0               ", 16);
        set_dirty(midi_mon_[1].active());  // turn 'STORE' led 'ON', capture enabled

        out_.loop_led(1 << midi_mon_[1].mode());
        break;

    case MODE_ABOUT:
        memcpy(lcd_buf[0], "About 2019.02.14", 16);
        memcpy(lcd_buf[1], " MIDI PatchMate ", 16);
        break;

    case MODE_UPTIME:
        memcpy(lcd_buf[0], " Uptime         ", 16);
        memcpy(lcd_buf[1] + 12, " sec", 4);
        uptime_ = t / 1000;
        print_number(lcd_buf[1], 12, uptime_);
        break;

    case MODE_LOOP_COUNT:
        memcpy(lcd_buf[0], " Loop count     ", 16);
        memcpy(lcd_buf[1] + 14, " k", 2);
        uptime_ = loop_count_ / 1000;
        print_number(lcd_buf[1], 14, uptime_);
        break;

    case MODE_INPUT_COUNT:
        memcpy(lcd_buf[0], " Input count    ", 16);
        memcpy(lcd_buf[1] + 14, " k", 2);
        uptime_ = input_count_ / 1000;
        print_number(lcd_buf[1], 14, uptime_);
        break;

    case MODE_PC_COUNT:
        memcpy(lcd_buf[0], " PC count       ", 16);
        memcpy(lcd_buf[1] + 14, "  ", 2);
        uptime_ = pc_count_;
        print_number(lcd_buf[1], 14, uptime_);
        break;

#if defined(WITH_SNAKE)
    case MODE_GAME_SNAKE:
        snake_.setup();
        memcpy(lcd_buf[0], " Snake \4\3\3\3\3 \6  ", 16);
        memcpy(lcd_buf[1], " \1\1\1\1\1\1\4 16x2   ", 16);
        break;
#endif

#if defined(WITH_HERO)
    case MODE_GAME_HERO:
        hero_.setup();
        memcpy(lcd_buf[0], " Hero  \3        ", 16);
        memcpy(lcd_buf[1], "  \1   \6\5\7  16x2 ", 16);
        break;
#endif
    }

    // adjust loop led for all modes except MODE_LED_IGNORE_LOOP_MASK
    if (led4loop()) {
        out_.loop_led(loop_state_);
    }

    hide_cursor();

    lcd_update();
/*
    if (mode_ == MODE_MIDI_IN_MONITOR ||
        mode_ == MODE_MIDI_OUT_MONITOR) {
        lcd.setCursor(2, 1);
        lcd.cursor();
    }
*/
}

inline void
controller::show_cursor(unsigned long t)
{
    lcd.setCursor(cursor_pos_, 1);
    lcd.cursor();
    hide_cursor_timer_.schedule(t + settings_.hide_cursor_delay_s * 1000);
}

inline void
controller::hide_cursor()
{
    lcd.noCursor();
    hide_cursor_timer_.cancel();
}

inline void
controller::set_dirty(bool dirty)
{
    if (dirty == dirty_) return;

    debug(3, "dirty = ", dirty);

    dirty_ = dirty;
}

inline void
controller::show_master(bool master)
{
    show_master_ = master;

    if (mode_ != MODE_NORMAL) {
        return;
    }

    if (master) {
        out_.loop_led(loop_state_master_);
    } else {
        out_.loop_led(loop_state_);
    }
}

inline void
controller::button_handler::on_down(bool down, unsigned long t)
{
    controller_.on_down(id_, down, t);
}

inline void
controller::button_handler::on_hold(unsigned long t)
{
    controller_.on_hold(id_, t);
}

inline void
controller::encoder_handler::on_rotate(short dir, unsigned long t)
{
    controller_.on_rotate(dir, t);
}

inline void
controller::mute_timer_callback::operator()(unsigned long t)
{
    controller_.on_mute_timer(t);
}

inline void
controller::hide_cursor_timer_callback::operator()(unsigned long)
{
    controller_.hide_cursor();
}

inline void
controller::store_blink_timer_callback::operator()(unsigned long t)
{
    led_on_ = !led_on_;

    controller_.out_.store_led(led_on_);

    if (!led_on_) {
        if (rest_ == 0) {
            return;
        }

        if (rest_ != 0xFF) { rest_--; }
    }

    controller_.store_blink_timer_.schedule(t + period_);
}

inline void
controller::store_blink_timer_callback::start(unsigned long t, uint8_t count, unsigned long period)
{
    led_on_ = false;
    rest_ = count - 1;
    period_ = period;

    (*this)(t);
}

inline void
controller::store_blink_timer_callback::stop()
{
    rest_ = 0;
}

}
