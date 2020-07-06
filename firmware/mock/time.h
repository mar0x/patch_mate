
#pragma once

#include <unistd.h>
#include <sys/time.h>


void delay(int ms) { usleep(ms * 1000); }


static timeval start_time;

unsigned long micros() {
    timeval t;

    gettimeofday(&t, 0);

    return (t.tv_sec - start_time.tv_sec) * 1000000 + t.tv_usec - start_time.tv_usec;
}

unsigned long millis() {
    timeval t;

    gettimeofday(&t, 0);

    return (t.tv_sec - start_time.tv_sec) * 1000 + t.tv_usec / 1000 - start_time.tv_usec / 1000;
}


