#pragma once

template<uint8_t MAX, typename T>
struct ring {

    using value_t = T;

    uint8_t next(uint8_t i) const { return (i + 1) % MAX; }
    uint8_t next() const { return next(pos); }
    uint8_t prev(uint8_t i) const { return (i + MAX - 1) % MAX; }
    uint8_t prev() const { return prev(pos); }

    uint8_t sum(uint8_t a, uint8_t b) const { return (a + b) % MAX; }

    uint8_t diff(uint8_t a, uint8_t b) const { return (a + MAX - b) % MAX; }
    uint8_t diff(uint8_t a) const { return (a + MAX - start) % MAX; }

    void reset() {
        start = 0;
        pos = 0;
        size = 0;
    }

    void operator++ () {
        pos = next(pos);
        size++;
    }

    uint8_t operator[](uint8_t i) const {
        return data[i];
    }

    uint8_t& operator[](uint8_t i) {
        return data[i];
    }

    uint8_t front() const {
        return data[start];
    }

    void pop_front() {
        start = next(start);
        size = diff(pos, start);
    }

    void set_pos(uint8_t p) {
        pos = p;
        size = diff(pos, start);
    }

    void set_start(uint8_t s) {
        start = s;
        size = diff(pos, start);
    }

    T data[MAX];
    uint8_t start;
    uint8_t pos;
    uint8_t size;
};
