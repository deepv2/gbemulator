#pragma once
#if defined(_WIN32) || defined(_WIN64)
#define WINDOWS
#endif
#if	defined(__linux__) || defined(__linux) || defined(linux__)
#define LINUX
#endif
#include <cstdint>
#include <fstream>
#include <string>
#include <iostream>
#include <array>

#ifdef LINUX
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#define MEM_MAX 8192

class CPU
{
private:
	int8_t a, b, c, d, e, f, h, l;
	uint16_t sp, pc;
	struct __attribute__((packed)) {
		uint8_t zero : 1;
		uint8_t sub : 1;
		uint8_t half_carry : 1;
		uint8_t carry : 1;
		uint8_t padding : 4;
	} flags;
	uint8_t * game_cart;
	size_t fsize;
	//std::ifstream game_cart;
	std::array<uint8_t, MEM_MAX> internal_memory;
	unsigned long cycles;

public:
	CPU();
	CPU(const char *str);
	CPU(const std::string &str);
	int decodeOpcode();
	int loadCart(const char *str);
	~CPU();
};

