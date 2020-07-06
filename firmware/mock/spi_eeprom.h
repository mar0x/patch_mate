
#pragma once

#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>

class spi_eeprom {
    enum {
        SIZE = 0x2000,
    };

    static uint8_t *data;

public:
    static void setup() {
        static int fd = open("spi_eeprom.bin", O_RDWR | O_CREAT, 0777);
        if (fd < 0) {
            fprintf(stderr, "open failed: %s (%d)\n", strerror(errno), errno);
            exit(1);
        }

        ftruncate(fd, SIZE);

        data = (uint8_t *) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) {
            fprintf(stderr, "mmap failed: %s (%d)\n", strerror(errno), errno);
            exit(1);
        }
    }

    template<typename T>
    static void get(uint16_t addr, T& v) { memcpy(&v, data + addr, sizeof(v)); }

    template<typename T>
    static void put(uint16_t addr, const T& v) { memcpy(data + addr, &v, sizeof(v)); }

    static void write_enable() { }
    static void write_disable() { }
};

uint8_t *spi_eeprom::data;
