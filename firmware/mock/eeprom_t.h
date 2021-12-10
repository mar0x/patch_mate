
#pragma once

#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>

template<typename TRAITS>
class eeprom_t {
    using traits = TRAITS;

    static uint8_t *data;

public:
    static void setup() {
        int fd = open(traits::name(), O_RDWR | O_CREAT, 0777);
        if (fd < 0) {
            fprintf(stderr, "open failed: %s (%d)\n", strerror(errno), errno);
            exit(1);
        }

        ftruncate(fd, traits::size());

        data = (uint8_t *) mmap(0, traits::size(), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) {
            fprintf(stderr, "mmap failed: %s (%d)\n", strerror(errno), errno);
            exit(1);
        }
    }

    static uint8_t read(uint16_t addr) { return data[addr]; }
    static void write(uint16_t addr, uint8_t v) { data[addr] = v; }
    static void update(uint16_t addr, uint8_t v) { data[addr] = v; }

    static constexpr uint16_t length() { return traits::size(); }

    template<typename T>
    static void get(uint16_t addr, T& v) { memcpy(&v, data + addr, sizeof(v)); }

    template<typename T>
    static void put(uint16_t addr, const T& v) { memcpy(data + addr, &v, sizeof(v)); }
};

template<typename TRAITS>
uint8_t *eeprom_t<TRAITS>::data;
