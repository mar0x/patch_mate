
#pragma once

struct LiquidCrystal {

    void begin(uint8_t, uint8_t) { }

    void clear() { debug(7, "lcd clear"); }

    void setCursor(int pos, int line) {
        cur_pos = pos;
        cur_line = line;
        wmove(lcd_win, line + 1, pos + 1);

        debug(7, "setCursor: ", pos, ", ", line);
    }

    void print(const char * str) { debug(7, "print: ", str); }
    void print(int n) { debug(7, "printN: ", n); }

    void createChar(int, const uint8_t *) { }

    void write(const char c) {
        wprintw(lcd_win, "%c", c);
        wrefresh(lcd_win);

        debug(7, "writeC: ", c);
    }
    void write(const char * str, int n) {
        wprintw(lcd_win, "%.*s", n, str);
        wrefresh(lcd_win);

        debug(7, "write: ", str, ", ", n);
    }

    void cursor() { cur_on = true; }
    void noCursor() { curs_set(0); cur_on = false; }

    void commit_show_cursor() {
        wmove(lcd_win, cur_line + 1, cur_pos + 1);
        curs_set(1);
        wrefresh(lcd_win);
    }

    int cur_pos = 0;
    int cur_line = 0;
    bool cur_on = false;
};

using spi_oled = LiquidCrystal;

