#pragma once

#if defined(_WIN32) || defined(_WIN64)
	#define WINDOWS
#endif

#if	defined(__linux__) || defined(__linux) || defined(linux__)
	#define LINUX
#endif

#include <cstdint>
#include <string>

#include "memory.hpp"

#define REG_MAX 8
#define PC_START 0x100

class CPU
{
private:
	int8_t reg[REG_MAX];
	uint16_t sp, pc;
	struct __attribute__((packed)) {
		uint8_t zero : 1;
		uint8_t sub : 1;
		uint8_t half_carry : 1;
		uint8_t carry : 1;
		uint8_t padding : 4;
	} flags;
	unsigned long cycles;
	Memory & mem;

public:
	CPU(Memory &);
	int decodeOpcode();
	~CPU();
};

enum {
	A, B, C, D, E, F, H, L
} register_index;

