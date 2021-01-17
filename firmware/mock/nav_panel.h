
#pragma once

#include "../controller.h"

namespace patch_mate {

struct nav_panel {

    void create_window() {
        win_ = newwin(5, 15, 4, 2);
        box(win_, 0, 0);
        mvwprintw(win_, 1, 7, ".");
        mvwprintw(win_, 3, 7, ".");

        mvwprintw(win_, 2, 2, ".");
        mvwprintw(win_, 2, 12, ".");
        wrefresh(win_);
    }

    void update(unsigned long t, int c) {
        mvwprintw(win_, 2, 2, "%c", (controller_.in_.left().down() ? '<' : '.'));
        mvwprintw(win_, 2, 12, "%c", (controller_.in_.right().down() ? '>' : '.'));
        mvwprintw(win_, 1, 7, "%c", (c == KEY_UP ? '^' : '.'));
        mvwprintw(win_, 3, 7, "%c", (c == KEY_DOWN ? 'v' : '.'));
        wrefresh(win_);
    }

    WINDOW *win_;
};

}
