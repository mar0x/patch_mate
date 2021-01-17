
#if !defined(ARDUINO)

#include <ncurses.h>

WINDOW *pm_win;
WINDOW *key_win;
WINDOW *lcd_win;
WINDOW *loop_win;

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int random(int) { return 0; }

#define PROGMEM
#define pgm_read_word
#define pgm_read_byte_near(p) (*(p))
#define strcpy_P strcpy

#include "serial.h"

#include "../config.h"
#include "../debug.h"

#include "serial_pts.h"
#include "time.h"
#include "eeprom.h"
#include "liquid_crystal.h"
#include "in.h"
#include "out.h"

#include "log_window.h"
WINDOW *log_window::win;

#include "../controller.h"
#include "nav_panel.h"

int main() {
/*
    int maxlines = LINES - 1;
    int maxcols = COLS - 1;
*/
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

    patch_mate::nav_panel np;
    np.create_window();

    lcd_win = newwin(2 + LCD_LINES, 2 + LCD_COLUMNS, 5, 20);
    loop_win = newwin(3, patch_mate::MAX_LOOP * 2 + 2, 5, 26 + LCD_COLUMNS);

    WINDOW *log_tray = newwin(27, 82, 8 + LCD_LINES, 1);
    log_window::create(25, 80, 9 + LCD_LINES, 2);

    wrefresh(lcd_win);
    wrefresh(log_window::win);

    WINDOW *key_win = newwin(1, 3, 1, 30);

    (void) getch();

    box(lcd_win, 0, 0);

    box(log_tray, 0, 0);
    wrefresh(log_tray);

    box(pm_win, 0, 0);
    wrefresh(pm_win);

    patch_mate::controller_.setup();

    /* Get all the mouse events */
    mousemask(ALL_MOUSE_EVENTS, NULL);

    bool quit = false;

    while (!quit) {
        int c = getch();
        //int c = wgetch(pm_win);

        if (c != -1) {
            //log_window::println("key: ", c);
            debug(2, "key: ", c);
        }

        patch_mate::last_key = c;

        mvwprintw(key_win, 0, 0, "%3d", c);
        wrefresh(key_win);

        if (c == 927 || c == 'q') {
            quit = true;
        }

        patch_mate::controller_.loop();

        np.update(0, c);

        if (lcd.cur_on) {
            lcd.commit_show_cursor();
        }
    }

    refresh();
    endwin();

    return 0;
}

#endif
