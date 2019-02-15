
#pragma once

struct serial {
    void begin(int) { }

    int read() { return read_buf[pos++]; }

    int available() { return end - pos; }

    void write(const void* data, int s) {
        print("write: ");

        uint8_t *b = (uint8_t *) data;
        for (uint8_t i = 0; i < s; i++) {
            print(b[i]); print(" ");
        }
        println("");
    }

    void print(const char* str) { wprintw(log_win, "%s", str); }
    void print(int n) { wprintw(log_win, "%d", n); }

    void println(const char* str) { wprintw(log_win, "%s\n", str); wrefresh(log_win); }
    void println(int n) { wprintw(log_win, "%d\n", n); wrefresh(log_win); }

    void simulate(const void* data, int s) {
        memcpy(read_buf, data, s);
        pos = 0;
        end = s;
    }

    uint8_t read_buf[256];
    uint8_t pos = 0;
    uint8_t end = 0;
};

serial Serial;
#define HAVE_HWSERIAL1 1

