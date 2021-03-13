
#pragma once

#include "log_window.h"

struct serial {
    void begin(int) { }

    int read() { return read_buf[pos++]; }

    int available() const { return end - pos; }

    void write(const void* data, int s) {
        print("write: ");

        uint8_t *b = (uint8_t *) data;
        for (uint8_t i = 0; i < s; i++) {
            print(b[i]); print(" ");
        }
        println("");
    }

    bool dtr() const { return true; }

    void print(const char* str) { log_window::print(str); }
    void print(int n) { log_window::print(n); }

    void println(const char* str) { log_window::println(str); }
    void println(int n) { log_window::println(n); }

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

