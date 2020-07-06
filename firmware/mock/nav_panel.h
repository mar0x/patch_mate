
#pragma once

#include "../controller.h"

namespace patch_mate {

struct nav_panel {

    void create_window() {
        win_ = newwin(5, 19, 9, 2 + 10);
        box(win_, 0, 0);
        mvwprintw(win_, 1, 9, ".");
        mvwprintw(win_, 3, 9, ".");

        mvwprintw(win_, 2, 2, ".");
        mvwprintw(win_, 2, 16, ".");
        wrefresh(win_);

        np_win = win_;
    }

    void update(unsigned long t, int c) {
        mvwprintw(win_, 2, 2, "%c", (controller_.in_.left().down() ? '<' : '.'));
        mvwprintw(win_, 2, 16, "%c", (controller_.in_.right().down() ? '>' : '.'));
        mvwprintw(win_, 1, 9, "%c", (c == KEY_UP ? '^' : '.'));
        mvwprintw(win_, 3, 9, "%c", (c == KEY_DOWN ? 'v' : '.'));
        wrefresh(win_);
    }

    WINDOW *win_;
};

}
