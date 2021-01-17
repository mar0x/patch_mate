
#pragma once

#include <stdlib.h> 
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "log_window.h"

struct serial_pts {
    void begin(int) { open_pts(); }

    int read() { return read_buf[pos++]; }

    int available() {
        if (end > pos) {
            return end - pos;
        }

        end = 0;
        pos = 0;

        pollfd pfd;

        pfd.fd = pts_;
        pfd.events = POLLIN | POLLPRI;
        pfd.revents = 0;

        int res = poll(&pfd, 1, 0);
        if (res < 0) {
            log_window::println("poll() failed: ", strerror(errno), " (", errno, ")");
            return false;
        }

        if (res > 0) {
            // log_window::println("poll() res: ", (int) res, " ", (int) pfd.revents);
            ssize_t rr = ::read(pts_, read_buf, sizeof(read_buf));
            // log_window::println("poll() read: ", (int) rr);
            if (rr <= 0) {
                open_pts();
                return false;
            }

            end += rr;
        }

        return end - pos;
    }

    void write(const void* data, int s) {
        ::write(pts_, data, s);
    }

    void print(const char* str) { write(str, strlen(str)); }
    void print(char c) { write(&c, 1); }
    void print(int n) {
        char buf[50];
        int s = snprintf(buf, sizeof(buf), "%d", n);
        write(buf, s);
    }

    void println(const char* str) { print(str); write("\r\n", 2); }
    // void println(char c) { char buf[3]; buf[0] = c; buf[1] = '\r'; buf[2] = '\n'; write(buf, 3); }
    void println(int n) { print(n); write("\r\n", 2); }

    void simulate(const void* data, int s) {
        memcpy(read_buf, data, s);
        pos = 0;
        end = s;
    }

    void open_pts()
    {
        pts_ = posix_openpt(O_RDWR);

        char *n = ptsname(pts_);
        log_window::println("pts name: ", (const char*) n);

        grantpt(pts_);
        unlockpt(pts_);
    }

    uint8_t read_buf[256];
    uint8_t pos = 0;
    uint8_t end = 0;

    int pts_;
};

serial_pts pts;
