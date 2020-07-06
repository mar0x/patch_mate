
#pragma once

#define HW_VERSION "2020.04.16"
#define FW_VERSION "2020.07.05"

#include "config.h"
#include "debug.h"

#include "print_number.h"
#include "rotate.h"

#if defined(ARDUINO)

#include <Arduino.h>
#include <EEPROM.h>

#include "spi_eeprom.h"
#include "spi_oled.h"
#include "in.h"
#include "out.h"

#else

#include "mock/spi_eeprom.h"
#include "mock/serial_pts.h"
#include "mock/time.h"
#include "mock/eeprom.h"
#include "mock/liquid_crystal.h"
#include "mock/in.h"
#include "mock/out.h"

#endif

#include "artl/button.h"
#include "artl/encoder.h"
#include "artl/timer.h"
#include "artl/tc.h"

#include "midi_cmd.h"
#include "serial_cmd.h"
#include "settings.h"
#include "program.h"

#include "artl/yield.h"

#include "lcd.h"
#include "midi_mon.h"
#include "edit_value.h"

namespace patch_mate {

struct controller_t {

    enum {
        MIN_CURSOR_POS = 4,
        MAX_CURSOR_POS = 16,
    };

    controller_t();

    void setup();
    void loop();

    bool read_eeprom();
    void reset_eeprom();

    void process_midi_cmd();
    void process_serial_cmd(unsigned long);
    void midi_out(const midi_cmd_t& cmd);

    void set_program(uint8_t prog);
    void set_loop(uint8_t loop, bool val);
    void send_loop(uint8_t loop, bool val);

    bool led4loop() const;

    void on_down(uint8_t, bool, unsigned long);
    void on_hold(uint8_t, unsigned long);
    void on_rotate(short, unsigned long);

    void mute(unsigned long t);
    void on_mute_timer(unsigned long t);

    void commit(unsigned long);

    template<typename T>
    void start_edit(T& value, unsigned int min, unsigned int max,
        unsigned int step = 1, unsigned int display_offset = 0,
        unsigned int off = (unsigned int) -1,
        unsigned int on = (unsigned int) -1);
    void commit_edit();
    void set_edit_loop(uint8_t edit_loop);
    void set_mode(uint8_t m, unsigned long t);

    void show_cursor(unsigned long t);
    void hide_cursor();

    void show_title_hint();
    void hide_hint();

    void set_dirty(bool dirty);

    void show_master(bool master);

    void print_version_line();

    struct button_handler : public artl::default_button_handler {
        button_handler(uint8_t id) : id_(id) { }

        void on_down(bool, unsigned long);
        void on_hold(unsigned long);

        uint8_t id_;
    };

    struct encoder_handler : public artl::default_encoder_handler {
        void on_rotate(short, unsigned long);
    };

    struct in_traits {
        using button_handler = controller_t::button_handler;
        using encoder_handler = controller_t::encoder_handler;

        enum {
            LOOP0 = 0,
            STORE = 10,
            LEFT,
            RIGHT,
        };
    };

    enum {
        STORE = in_traits::STORE,
        LEFT = in_traits::LEFT,
        RIGHT = in_traits::RIGHT,
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
    settings_t settings_;

    uint8_t edit_loop_;
    uint8_t edit_settings_offset_;
    edit_value_t edit_;

    midi_cmd_t midi_cmd_;
    midi_cmd_t last_out_cmd_;

    serial_cmd_t serial_cmd_;

    bool dirty_ = false;
    bool bad_magic_;

    uint8_t prog_id_ = 0;

    program_t program_;
    program_t program_master_;

    uint8_t cursor_pos_ = MIN_CURSOR_POS;
    hide_cursor_timer hide_cursor_timer_;

    unsigned long uptime_;
    unsigned long loop_count_ = 0;
    unsigned long pc_count_ = 0;
    unsigned long input_count_ = 0;
    unsigned long last_out_ms_ = 0;

    midi_mon_t midi_mon_[2];

    volatile bool enable_input_;
    using input_tc = artl::tc<1>;

    bool show_master_;

    static const char version_[];

    uint8_t version_line_ = 0;
};

controller_t controller_;

const char controller_t::version_[] PROGMEM =
    "HW " HW_VERSION "\n"
    "FW " FW_VERSION "\n"
    "BD " __DATE__ "\n"
    "BT " __TIME__ "\n";

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

inline
controller_t::controller_t()
{
}

inline void
controller_t::setup() {
    in_.setup();
    out_.setup();

    spi_eeprom::setup();

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

    artl::yield();
}

inline void
controller_t::loop() {
    unsigned long t = millis();

    ++loop_count_;

    while (!midi_cmd_ && in_.midi_available()) {
        uint8_t b = in_.midi_read();

        if (midi_mon_[0].active()) {
            midi_mon_[0].read(b);
        }

        midi_cmd_.read(b);
    }

    if (midi_cmd_) {
        process_midi_cmd();
    }

    while (!serial_cmd_ && in_.serial_available()) {
        uint8_t b = in_.serial_read();

        serial_cmd_.read(b);

        // Serial echo
        // out_.serial_print((char) b);
    }

    if (serial_cmd_) {
        process_serial_cmd(t);
    }

    in_.update(t, enable_input_);

#if defined(ISR) && defined(TIMER1_COMPA_vect)
    if (!enable_input_) {
        artl::yield();
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

    artl::yield();
}

static const char eemagic[4] = { 'M', 'P', 'M', '4' };

inline bool
controller_t::read_eeprom() {
    char magic_test[sizeof(eemagic)];

    EEPROM.get(0, magic_test);
    if (memcmp(eemagic, magic_test, sizeof(eemagic)) != 0) {
        bad_magic_ = true;
        return false;
    }

    memset(magic_test, 0, sizeof(magic_test));

    spi_eeprom::get(0, magic_test);
    if (memcmp(eemagic, magic_test, sizeof(eemagic)) != 0) {
        bad_magic_ = true;
        return false;
    }

    program_t::setup(sizeof(eemagic));
    settings_t::setup(sizeof(eemagic));

    settings_.read();

    bad_magic_ = false;
#if defined(DEBUG)
    debug_level_ = settings_.usb_debug;
#endif
    return true;
}

inline void
controller_t::reset_eeprom() {
    EEPROM.put(0, eemagic);

    spi_eeprom::put(0, eemagic);

    program_t::setup(sizeof(eemagic));

    for (uint8_t i = 0; i < MAX_PROGRAMS; i++) {
        program_t().write(i);
    }

    settings_t::setup(sizeof(eemagic));

    settings_ = settings_t();
    settings_.write();

    bad_magic_ = false;
}

inline void
controller_t::process_midi_cmd() {
    if (settings_.midi_fwd) {
        midi_out(midi_cmd_);
    }

    if (midi_cmd_.command() == midi_cmd_t::CMD_PROG_CHANGE &&
        midi_cmd_.channel() == settings_.midi_channel) {
        set_program(midi_cmd_.program());
    }

    if (midi_cmd_.command() == midi_cmd_t::CMD_CTRL_CHANGE &&
        midi_cmd_.channel() == settings_.midi_channel) {
        for(uint8_t i = 0; i < MAX_LOOP; i++) {
            if (settings_.midi_loop_ctrl_in[i] == midi_cmd_.controller()) {
                set_loop(i, midi_cmd_.value() != 0);
            }
        }
    }

    midi_cmd_.reset();
}

inline void
controller_t::process_serial_cmd(unsigned long t) {
    out_.serial_println("");

    bool old_dirty = dirty_;

    // out_.serial_println(serial_cmd_.buf_);

    switch (serial_cmd_.command()) {
    case serial_cmd_t::CMD_VERSION:
        for (uint16_t i = 0; i < sizeof(version_); ++i) {
            char b = pgm_read_byte_near(version_ + i);
            if (b == '\n') { out_.serial_print('\r'); }
            out_.serial_print(b);
        }
        break;

    case serial_cmd_t::CMD_HELP: {
        for (uint16_t i = 0; i < sizeof(help_); ++i) {
            char b = pgm_read_byte_near(help_ + i);
            if (b == '\n') { out_.serial_print('\r'); }
            out_.serial_print(b);
        }
        break;
    }
    case serial_cmd_t::CMD_PROG_CHANGE: {
        uint8_t p;
        if (serial_cmd_.get_arg(1, p)) { set_program(p); }

        out_.serial_println("PC ", prog_id_);
        break;
    }
    case serial_cmd_t::CMD_CTRL_CHANGE: {
        uint8_t c = 0, v = 0;
        if (serial_cmd_.get_arg(1, c) &&
            serial_cmd_.get_arg(2, v)) {
            for(uint8_t i = 0; i < MAX_LOOP; i++) {
                if (settings_.midi_loop_ctrl_in[i] == c) {
                    set_loop(i, v);
                }
            }
        }

        out_.serial_println("CC ", c, " ", v);
        break;
    }
    case serial_cmd_t::CMD_LOOP: {
        uint8_t t = 0;
        uint16_t v = 0;
        if (serial_cmd_.get_arg(1, t) &&
            serial_cmd_.get_arg(2, v)) { set_loop(t, v); }

        if (t >= MAX_LOOP) t = MAX_LOOP - 1;

        out_.serial_println("LP ", t, " ", program_.loop[t]);
        break;
    }
    case serial_cmd_t::CMD_NAME: {
        serial_cmd_.get_arg(1, program_.title);

        if (mode_ == MODE_NORMAL) {
            memcpy(lcd_buf[0] + 4, program_.title, 12);

            hide_cursor();

            lcd_update(0, 0, LCD_COLUMNS);
        }

        out_.serial_println("NM \"", program_.title, "\"");
        break;
    }
    case serial_cmd_t::CMD_MODE: {
        uint8_t m;
        if (serial_cmd_.get_arg(1, m)) { set_mode(m, t); }

        out_.serial_println("MD ", mode_);
        break;
    }
    case serial_cmd_t::CMD_STORE: {
        if (!dirty_) { out_.serial_println("CLEAR"); }
        else if (bad_magic_) { out_.serial_println("BAD MAGIC"); }
        else { commit(t); }

        out_.serial_print("PR ", prog_id_, " ", program_.loop[0]);
        for (uint8_t i = 1; i < MAX_LOOP; i++) {
            out_.serial_print(",", program_.loop[i]);
        }
        out_.serial_println(" \"", program_.title, "\"");
        break;
    }
    case serial_cmd_t::CMD_RESTORE: {
        if (!dirty_) { out_.serial_println("CLEAR"); }
        else { set_program(prog_id_); }

        out_.serial_print("PR ", prog_id_, " ", program_.loop[0]);
        for (uint8_t i = 1; i < MAX_LOOP; i++) {
            out_.serial_print(",", program_.loop[i]);
        }
        out_.serial_println(" \"", program_.title, "\"");
        break;
    }
    case serial_cmd_t::CMD_PROGRAM: {
        program_t pr = program_;
        uint8_t p = prog_id_;
        uint16_t l = pr.loop;

        if (serial_cmd_.get_arg(1, p)) {
            if (p >= MAX_PROGRAMS) p = prog_id_;

            if (p != prog_id_) {
                pr.read(p);
            }

            if (serial_cmd_.get_arg(2, l)) {
                pr.loop = l;

                serial_cmd_.get_arg(3, pr.title);

                pr.write(p);

                if (p == prog_id_) { set_program(prog_id_); }
            }
        }

        out_.serial_print("PR ", p, " ", pr.loop[0]);
        for (uint8_t i = 1; i < MAX_LOOP; i++) {
            out_.serial_print(",", pr.loop[i]);
        }
        out_.serial_println(" \"", pr.title, "\"");
        break;
    }
    case serial_cmd_t::CMD_MIDI_CHANNEL: {
        uint8_t c;
        if (serial_cmd_.get_arg(1, c)) {
            c = c % 16;
            if (mode_ == MODE_SETTINGS_CHANNEL) {
                edit_.value(c);
                edit_.print(lcd_buf[1] + 13);

                set_dirty(false);

                lcd_update(13, 1, 3);
            }

            settings_.midi_channel = c;
            settings_.write(settings_.midi_channel);
        }

        out_.serial_println("MC ", settings_.midi_channel);
        break;
    }
    case serial_cmd_t::CMD_MIDI_PROG_OUT: {
        // TODO
        out_.serial_println("MO ", settings_.midi_out_prog);
        break;
    }
    case serial_cmd_t::CMD_MIDI_FORWARD: {
        // TODO
        out_.serial_println("MF ", settings_.midi_fwd);
        break;
    }
#if defined(DEBUG)
    case serial_cmd_t::CMD_DEBUG_LEVEL: {
        uint8_t c;
        if (serial_cmd_.get_arg(1, c)) {
            if (c > 9) c = 9;
            settings_.usb_debug = c;
            settings_.write(settings_.usb_debug);
            debug_level_ = c;
        }

        out_.serial_println("DL ", settings_.usb_debug);
        break;
    }
#endif
    case serial_cmd_t::CMD_HIDE_CURSOR_DELAY: {
        // TODO
        out_.serial_println("HC ", settings_.hide_cursor_delay_s);
        break;
    }
    case serial_cmd_t::CMD_FACTORY_RESET: {
        set_mode(MODE_SETTINGS_FACTORY_RESET, t);
        break;
    }
    case serial_cmd_t::CMD_MIDI_LOOP_IN_CTRL: {
        // TODO
        break;
    }
    case serial_cmd_t::CMD_MIDI_LOOP_OUT_CTRL: {
        // TODO
        break;
    }
    case serial_cmd_t::CMD_MIDI_MON_IN: {
        // TODO
        break;
    }
    case serial_cmd_t::CMD_MIDI_MON_OUT: {
        // TODO
        break;
    }
    case serial_cmd_t::CMD_HEX: {
        // TODO
        break;
    }
    }

    if (mode_ == MODE_NORMAL) {
        set_dirty(program_ != program_master_);

        if (old_dirty != dirty_) {
            out_.serial_println(dirty_ ? "DIRTY" : "CLEAR");
        }
    }

    serial_cmd_.reset();

    out_.serial_print("> ");
}

inline void
controller_t::midi_out(const midi_cmd_t& cmd) {
    if (midi_mon_[1].active()) {
        for (uint8_t i = 0; i < cmd.size(); i++) {
            midi_mon_[1].read(cmd[i]);
        }
    }

    last_out_cmd_ = cmd;
    last_out_ms_ = millis();

    out_.midi_write(cmd, cmd.size());
}

inline void
controller_t::set_program(uint8_t prog) {
    debug(1, "set_program: ", prog_id_, " -> ", prog);

    if (prog >= MAX_PROGRAMS) return;

    prog_id_ = prog;

    program_.read(prog);
    program_master_ = program_;

    if (settings_.midi_out_prog) {
        midi_cmd_t cmd(settings_.midi_channel, midi_cmd_t::CMD_PROG_CHANGE, prog_id_);

        midi_out(cmd);
    }

    for(uint8_t i = 0; i < MAX_LOOP; i++) {
        out_.loop_relay(i, program_.loop[i]);
        send_loop(i, program_.loop[i]);
    }

    if (mode_ == MODE_NORMAL) {
        set_dirty(false);

        print_number(lcd_buf[0], 3, prog_id_ + 1);
        lcd_buf[0][3] = ' ';
        memcpy(lcd_buf[0] + 4, program_.title, 12);

        hide_cursor();

        lcd_update(0, 0, LCD_COLUMNS);
        lcd_update(0, 1, LCD_COLUMNS);
    }

    // adjust loop led for all modes except MODE_LED_IGNORE_LOOP_MASK
    if (led4loop()) {
        out_.loop_led(program_.loop);
    }
}

inline void
controller_t::set_loop(uint8_t i, bool v) {
    debug(1, "set_loop: ", i, " ", program_.loop[i], " -> ", v);

    out_.loop_relay(i, v);

    // send controller_t change MIDI command if configured
    send_loop(i, v);

    // adjust loop led for all modes except MODE_LED_IGNORE_LOOP_MASK
    if (led4loop()) {
        out_.loop_led(i, v);
    }

    program_.loop.set(i, v);

    if (mode_ == MODE_NORMAL) {
        set_dirty(program_ != program_master_);
    }
}

inline void
controller_t::send_loop(uint8_t i, bool v) {
    if (settings_.midi_loop_ctrl_out[i] <= 127) {
        midi_cmd_t cmd(settings_.midi_channel, midi_cmd_t::CMD_CTRL_CHANGE,
            settings_.midi_loop_ctrl_out[i], v ? 127 : 0);

        midi_out(cmd);
    }
}

inline bool
controller_t::led4loop() const {
    return ((1 << mode_) & MODE_LED_IGNORE_LOOP_MASK) == 0;
}

inline void
controller_t::on_down(uint8_t i, bool down, unsigned long t) {
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
            case 4: set_mode(MODE_SETTINGS_MUTE_DELAY, t); break;
            case 5: set_mode(MODE_SETTINGS_HIDE_CURSOR, t); break;
            case 6: set_mode(MODE_MIDI_OUT_MONITOR, t); break;
        }
        return;
    }

    if (mode_ == MODE_NORMAL) {
        // toggle LOOP
        if (i < MAX_LOOP && in_.left().up() && in_.right().up()) {
            bool v = program_.loop[i];

            debug(2, "toggle loop ", i, " ", v, " -> ", !v);
            set_loop(i, !v);

            return;
        }

        // move cursor LEFT
        if (i == LEFT && in_.right().up()) {
            cursor_pos_ = rotate(cursor_pos_, MIN_CURSOR_POS, MAX_CURSOR_POS, -1);
            hide_hint();
            show_cursor(t);

            return;
        }

        // move cursor RIGHT
        if (i == RIGHT && in_.left().up()) {
            cursor_pos_ = rotate(cursor_pos_, MIN_CURSOR_POS, MAX_CURSOR_POS, 1);
            hide_hint();
            show_cursor(t);

            return;
        }

        // cancel changes and stay in NORMAL
        // hold RIGHT then press LEFT or
        // hold LEFT then press RIGHT
        if ((i == LEFT && in_.right().hold()) || (i == RIGHT && in_.left().hold()))
        {
            if (dirty_) { set_program(prog_id_); }
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

        // controller_t -> LOOP mapping
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

    if (mode_ == MODE_ABOUT) {
        if (i == LEFT && in_.right().up()) {
            version_line_ = rotate(version_line_, 0, 4, -1);
            print_version_line();
            lcd_update(0, 1, LCD_COLUMNS);
        }

        if (i == RIGHT && in_.left().up()) {
            version_line_ = rotate(version_line_, 0, 4, 1);
            print_version_line();
            lcd_update(0, 1, LCD_COLUMNS);
        }

        return;
    }

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

        if (i < MAX_LOOP && i < midi_mon_t::MODE_MAX && in_.left().up() && in_.right().up()) {
            midi_mon_[m].mode(i, t);

            out_.loop_led(1 << i);
        }
    }
}

inline void
controller_t::on_hold(uint8_t i, unsigned long t) {
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

            commit(t);

            return;
        }

        if (i == RIGHT && in_.left().up()) {
            show_master(true);
        }

        if ((i == LEFT && in_.right().down()) || (i == RIGHT && in_.left().down())) {
            // cancel changes
            if (dirty_) { set_program(prog_id_); }
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
        case MODE_SETTINGS_CTRL_IN:
        case MODE_SETTINGS_CTRL_OUT:
            commit_edit();

            edit_.value(edit_.value());
            set_dirty(false);

            // stay in same mode for convenience
            store_blink_timer_.start(t);
            return;

        case MODE_SETTINGS_CHANNEL:
        case MODE_SETTINGS_PROG_OUT:
        case MODE_SETTINGS_MIDI_FWD:
        case MODE_SETTINGS_HIDE_CURSOR:
            commit_edit();
            break;

        case MODE_SETTINGS_MUTE_DELAY:
            settings_.mute_delay_ms = edit_.value();
            settings_.write(settings_.mute_delay_ms);
            break;

        case MODE_SETTINGS_FACTORY_RESET:
            reset_eeprom();
            set_mode(MODE_NORMAL, t);
            set_program(0);
            store_blink_timer_.start(t);
            return;

#if defined(DEBUG)
        case MODE_SETTINGS_USB_DEBUG:
            commit_edit();
            debug_level_ = edit_.value();
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

inline void
controller_t::commit(unsigned long t) {
    // commit program change
    program_.write(prog_id_);

    program_master_ = program_;

    store_blink_timer_.start(t);
    set_dirty(false);
}

/* Available alphabet:
  ABCDEFGHIJKLMNOPRSTUVWXYZ[\]^_
  !"#$%&'()*+,-./
  :;<=>?@
  0123456789
*/

/*
joints = {
    { ' ', 'A' },
    { '_', '!' },
    { '/', ':' },
    { '@', '0' },
    { '9', ' ' }
};

ranges = {
    { ' ', ' ' },
    { 'A', '_' },
    { '!', '/' },
    { ':', '@' }
    { '0', '9' },
};
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
controller_t::on_rotate(short dir, unsigned long t) {
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
            set_program(rotate(prog_id_, MAX_PROGRAMS, dir));

            return;
        }

        char& c = program_.title[cursor_pos_ - MIN_CURSOR_POS];
        c = rotate_char(c, dir);
        lcd_buf[0][cursor_pos_] = c;

        show_title_hint();

        lcd.setCursor(cursor_pos_, 0);
        lcd.write(c);
        show_cursor(t);

        set_dirty(program_ != program_master_);

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
        edit_.rotate(dir);
        edit_.print(lcd_buf[1] + 13);

        set_dirty(edit_.dirty());

        lcd_update(13, 1, 3);
        break;

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
controller_t::mute(unsigned long t) {
    if (wait_mute_) return;

    out_.mute();

    wait_mute_ = true;
    mute_timer_.schedule(t + (on_mute_ ? 0 : settings_.mute_delay_ms));
}

inline void
controller_t::on_mute_timer(unsigned long t) {
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

template<typename T>
inline void
controller_t::start_edit(T& value,
    unsigned int min, unsigned int max,
    unsigned int step, unsigned int display_offset,
    unsigned int off, unsigned int on)
{
    edit_settings_offset_ = (uint8_t *) &value - (uint8_t *) &settings_;
    edit_.start(value, min, max, step, display_offset, off, on);
    edit_.print(lcd_buf[1] + 13);
    set_dirty(false);
}

inline void
controller_t::commit_edit()
{
    uint8_t *v = ((uint8_t *) &settings_) + edit_settings_offset_;
    *v = edit_.value();
    settings_.write(*v);
}

inline void
controller_t::set_edit_loop(uint8_t edit_loop) {
    edit_loop_ = edit_loop;

    switch (mode_) {
    case MODE_SETTINGS_CTRL_IN:
        start_edit(settings_.midi_loop_ctrl_in[edit_loop_ - 1], 0, 128, 1, 0, 128);
        break;
    case MODE_SETTINGS_CTRL_OUT:
        start_edit(settings_.midi_loop_ctrl_out[edit_loop_ - 1], 0, 128, 1, 0, 128);
        break;
    }

    print_number(lcd_buf[1], 1, edit_loop_);

    out_.loop_led(1 << (edit_loop_ - 1));
}

inline void
controller_t::set_mode(uint8_t m, unsigned long t) {
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
        set_dirty(program_ != program_master_);

        print_number(lcd_buf[0], 3, prog_id_ + 1);
        lcd_buf[0][3] = ' ';
        memcpy(lcd_buf[0] + 4, program_.title, 12);
        lcd_buf[1][15] = ' ';
        break;

    case MODE_SETTINGS_CHANNEL:
        start_edit(settings_.midi_channel, 0, 15, 1, 1);
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
        start_edit(settings_.midi_out_prog, 0, 1, 1, 0, 0, 1);
        memcpy(lcd_buf[1], "Prog Chg OUT ", 13);
        break;

    case MODE_SETTINGS_MIDI_FWD:
        start_edit(settings_.midi_fwd, 0, 1, 1, 1, 0, 1);
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
        memcpy(lcd_buf[0], "About PatchMate ", 16);
        version_line_ = 0;
        print_version_line();
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
    }

    // adjust loop led for all modes except MODE_LED_IGNORE_LOOP_MASK
    if (led4loop()) {
        out_.loop_led(program_.loop);
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
controller_t::show_cursor(unsigned long t)
{
    lcd.setCursor(cursor_pos_, 0);
    lcd.cursor();
    hide_cursor_timer_.schedule(t + settings_.hide_cursor_delay_s * 1000);
}

inline void
controller_t::hide_cursor()
{
    hide_hint();
    lcd.noCursor();
    hide_cursor_timer_.cancel();
}

inline void
controller_t::show_title_hint()
{
    char c = lcd_buf[0][cursor_pos_];

    for (uint8_t i = cursor_pos_; i < LCD_COLUMNS; ++i, c = rotate_char(c, 1)) {
        lcd_buf[1][i] = c;
    }

    c = rotate_char(lcd_buf[0][cursor_pos_], -1);

    for (uint8_t i = cursor_pos_; i > 0; --i, c = rotate_char(c, -1)) {
        lcd_buf[1][i - 1] = c;
    }

    c = lcd_buf[0][cursor_pos_];

    lcd_update(0, 1, LCD_COLUMNS);
}

inline void
controller_t::hide_hint()
{
    if (mode_ == MODE_NORMAL) {
        memset(lcd_buf[1], ' ', LCD_COLUMNS);
        lcd_update(0, 1, LCD_COLUMNS);
    }
}

inline void
controller_t::set_dirty(bool dirty)
{
    if (dirty == dirty_) return;

    debug(3, "dirty = ", dirty);

    dirty_ = dirty;
}

inline void
controller_t::show_master(bool master)
{
    show_master_ = master;

    if (mode_ != MODE_NORMAL) {
        return;
    }

    if (master) {
        out_.loop_led(program_master_.loop);
    } else {
        out_.loop_led(program_.loop);
    }
}

inline void
controller_t::print_version_line()
{
    uint8_t l = 0;
    uint8_t n = 0;
    for (uint16_t i = 0; i < sizeof(version_); ++i) {
        char b = pgm_read_byte_near(version_ + i);

        if (l == version_line_ && b != '\n') {
            if (n < LCD_COLUMNS) {
                lcd_buf[1][n++] = b;
            }
        }

        if (b == '\n') {
            ++l;
            if (l > version_line_) {
                break;
            }
        }
    }

    while(n < LCD_COLUMNS) {
        lcd_buf[1][n++] = ' ';
    }
}

inline void
controller_t::button_handler::on_down(bool down, unsigned long t)
{
    controller_.on_down(id_, down, t);
}

inline void
controller_t::button_handler::on_hold(unsigned long t)
{
    controller_.on_hold(id_, t);
}

inline void
controller_t::encoder_handler::on_rotate(short dir, unsigned long t)
{
    controller_.on_rotate(dir, t);
}

inline void
controller_t::mute_timer_callback::operator()(unsigned long t)
{
    controller_.on_mute_timer(t);
}

inline void
controller_t::hide_cursor_timer_callback::operator()(unsigned long)
{
    controller_.hide_cursor();
}

inline void
controller_t::store_blink_timer_callback::operator()(unsigned long t)
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
controller_t::store_blink_timer_callback::start(unsigned long t, uint8_t count, unsigned long period)
{
    led_on_ = false;
    rest_ = count - 1;
    period_ = period;

    (*this)(t);
}

inline void
controller_t::store_blink_timer_callback::stop()
{
    rest_ = 0;
}

}
