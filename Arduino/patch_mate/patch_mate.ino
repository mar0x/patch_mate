
#ifdef ARDUINO

#include <Arduino.h>

#include <EEPROM.h>
#include <LiquidCrystal.h>

#include "config.h"
#include "debug.h"

#if defined(PATCH_MATE_LITE)

#include "in_lite.h"
#include "out_lite.h"

#else

#include "in.h"
#include "out.h"

#endif

#include "controller.h"

void setup(void) {
    patch_mate::controller_.setup();
}

void loop(void) {
    patch_mate::controller_.loop();
}

#else

#include <ncurses.h>

WINDOW *pm_win;
WINDOW *key_win;
WINDOW *lcd_win;
WINDOW *loop_win;
WINDOW *log_win;

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int random(int) { return 0; }

#include "mock/serial.h"

#include "config.h"
#include "debug.h"

#include "mock/time.h"
#include "mock/eeprom.h"
#include "mock/liquid_crystal.h"
#include "mock/in.h"
#include "mock/out.h"


#include "controller.h"

int main() {
    int maxlines = LINES - 1;
    int maxcols = COLS - 1;
    int row, col;

    gettimeofday(&start_time, 0);

#if defined(DEBUG)
    debug_level_ = 7;
#endif

    initscr();
    raw();
    //cbreak();
    noecho();

    // clear();
    halfdelay(1);

    keypad(stdscr, TRUE);

    getmaxyx(stdscr, row, col);

    mvprintw(0, 0, "maxyx: %d x %d\n", row, col);

    pm_win = newwin(4 + LCD_LINES, 82, 4, 1);
    key_win = newwin(3, 5, 6, 10);
    lcd_win = newwin(2 + LCD_LINES, 2 + LCD_COLUMNS, 5, 20);
    loop_win = newwin(3, 18, 5, 26 + LCD_COLUMNS);

    WINDOW *log_tray = newwin(27, 82, 8 + LCD_LINES, 1);
    log_win = newwin(25, 80, 9 + LCD_LINES, 2);
    scrollok(log_win, true);

    wrefresh(lcd_win);
    wrefresh(log_win);

    WINDOW *key_win = newwin(1, 3, 1, 30);

    int c = getch();

    box(lcd_win, 0, 0);

    box(log_tray, 0, 0);
    wrefresh(log_tray);

    box(pm_win, 0, 0);
    wrefresh(pm_win);

    patch_mate::controller_.setup();

    bool quit = false;

    while (!quit) {
        int c = getch();

        patch_mate::last_key = c;

        mvwprintw(key_win, 0, 0, "%3d", c);
        wrefresh(key_win);

        if (c == 27 || c == 'q') {
            quit = true;
        }

        patch_mate::controller_.loop();

        if (lcd.cur_on) {
            lcd.commit_show_cursor();
        }
    }

    refresh();
    endwin();

    return 0;
}

#endif
