#pragma once
#include <array>
#include <string>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <vector>

#include "cpu.hpp"

#ifdef LINUX
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#endif

#define MEM_MAX 8192
#define _2KB 0x800
#define _8KB 0x2000
#define _16KB 0x4000
#define _32KB 0x8000
#define _128KB 0x20000
#define ROM_MIN 32768
#define GRAPHIC_HEADER_SIZE 48
#define GRAPHIC_OFFSET 0x104
#define CART_TYPE_OFFSET 0x147
#define ROM_SIZE_OFFSET 0x148
#define RAM_SIZE_OFFSET 0x149

typedef void (*memcntl)(const uint16_t &);

class Memory {
    private:
        uint8_t * game_cart;
        uint8_t * gc_switchable_mem;
        int bank_num_selected;
        size_t fsize;
        uint8_t cart_type;
        unsigned int rom_size, rom_max_banks, ram_ext_size, ram_ext_banks;
        uint8_t internal_memory[_8KB];
        std::vector<uint8_t> extern_memory;
        int extern_memory_switchable_offset;
        void (Memory::*MemoryController)(const uint16_t &, const uint8_t &);
        bool ext_ram_enabled;

        int loadCart(const char *);
        int initializeMemory();
        void MBC1_handler(const uint16_t &, const uint8_t &);
        void MBC2_handler(const uint16_t &, const uint8_t &);
        void MBC3_handler(const uint16_t &, const uint8_t &);
        void MBC5_handler(const uint16_t &, const uint8_t &);
    public:
        Memory();
        Memory(const char *);
        Memory(const std::string &);
        ~Memory();
        inline int addrInRange(const uint16_t &, const uint16_t &, const uint16_t &) const;
        uint8_t readByte(const uint16_t &) const;
        uint16_t readWord(const uint16_t &) const;
        int writeByte(const uint16_t &, const uint8_t &);
        int writeWord(const uint16_t &, const uint16_t &);

};

