
#pragma once

#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>

struct eeprom {
    enum {
        SIZE = 1024,
    };

    uint8_t *data;
    int fd;

    eeprom() {
        fd = open("eeprom.bin", O_RDWR | O_CREAT, 0777);
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

    uint8_t read(int addr) { return data[addr]; }
    void update(int addr, uint8_t v) { data[addr] = v; }
    int length() { return SIZE; }

    template<typename T>
    void get(int addr, T& v) { memcpy(&v, data + addr, sizeof(v)); }

    template<typename T>
    void put(int addr, const T& v) { memcpy(data + addr, &v, sizeof(v)); }
};

eeprom EEPROM;


