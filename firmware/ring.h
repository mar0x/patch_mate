#pragma once

template<uint8_t MAX, typename T>
struct ring {

    using value_t = T;

    uint8_t next(uint8_t i) const { return (i + 1) % MAX; }
    uint8_t next() const { return next(pos); }
    uint8_t prev(uint8_t i) const { return (i + MAX - 1) % MAX; }
    uint8_t prev() const { return prev(pos); }

    bool empty() const { return size == 0; }
    bool full() const { return size == MAX; }

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

    value_t operator[](uint8_t i) const {
        return data[i];
    }

    value_t& operator[](uint8_t i) {
        return data[i];
    }

    const value_t& front() const {
        return data[start];
    }

    value_t& back() {
        return data[pos];
    }

    const value_t& back() const {
        return data[pos];
    }

    void push_back(const value_t& v) {
        back() = v;
        ++(*this);
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

    value_t data[MAX];
    uint8_t start = 0;
    uint8_t pos = 0;
    uint8_t size = 0;
};
