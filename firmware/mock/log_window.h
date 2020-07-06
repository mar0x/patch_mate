
#pragma once

#include <ncurses.h>

struct log_window {

    static void print(char* str) { wprintw(win, "%s", str); }
    static void print(const char* str) { wprintw(win, "%s", str); }
    static void print(int n) { wprintw(win, "%d", n); }
    static void print(short n) { wprintw(win, "%d", n); }
    static void print(unsigned char n) { wprintw(win, "%d", (int) n); }

    template<typename T1, typename ...Args>
    static
    void print(const T1& a1, Args... args) {
        print(a1);
        print(args...);
    }

    static void println(char* str) { wprintw(win, "%s\n", str); wrefresh(win); }
    static void println(const char* str) { wprintw(win, "%s\n", str); wrefresh(win); }
    static void println(int n) { wprintw(win, "%d\n", n); wrefresh(win); }
    static void println(short n) { wprintw(win, "%d\n", n); wrefresh(win); }
    static void println(unsigned char n) { wprintw(win, "%d\n", (int) n); wrefresh(win); }

    template<typename T1, typename ...Args>
    static
    void println(const T1& a1, Args... args) {
        print(a1);
        println(args...);
    }

    static void create(int lines, int cols, int y, int x) {
        win = newwin(lines, cols, y, x);
        scrollok(win, true);
    }

    static WINDOW *win;
};
