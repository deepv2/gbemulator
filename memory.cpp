#include "memory.hpp"


Memory::Memory() {
    game_cart = nullptr;
    gc_switchable_mem = nullptr;
    bank_num_selected = -1;
    fsize = 0;
    cart_type = 0;
    rom_size = 0;
    rom_max_banks = 0;
    ram_ext_banks = 0;
    ram_ext_size = 0;
    extern_memory = nullptr;
    extern_memory_switchable = nullptr;
}

int Memory::loadCart(const char *str) {
	#ifdef LINUX
    uint8_t graphic_header[48] = {
        0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83,
        0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
        0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
        0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
    };

	// open file to check if actual file
	int fd = open(str, O_RDONLY | O_EXCL, "rb");
	if(fd == -1) return -1;

	// calculate file size with istream
	std::ifstream file(str, std::ios::binary);
	int fsize_start = file.tellg();
	file.seekg(0, file.end);
	fsize = file.tellg();
	fsize = fsize - fsize_start;

    // Minimum ROM size is 32KiB
    if(fsize < ROM_MIN) return -2;

	// use mmap to map game cart address to CPU object
	game_cart = (uint8_t*)mmap(NULL, fsize, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if(game_cart == MAP_FAILED) return -3;

    // check if standard Nintendo graphic header exists in file
    if(std::memcmp(game_cart + GRAPHIC_OFFSET, graphic_header, GRAPHIC_HEADER_SIZE) != 0) return -4;

	// By default, selects bank 1
	gc_switchable_mem = game_cart + _16KB;
	bank_num_selected = 1;

	// TO-DO: Windows File-mapping implementation
	#elif defined(WINDOWS)
	return -1;
	#endif
}

Memory::Memory(const char *str) : Memory() {
    int ret = loadCart(str);
    if(ret < 0) throw ret;
    ret = initializeMemory();
    if(ret < 0) throw ret;
}

Memory::Memory(const std::string &str) : Memory() {
    int ret = loadCart(str.c_str());
    if(ret < 0) throw ret;
    ret = initializeMemory();
    if(ret < 0) throw ret;
}

int Memory::initializeMemory() {
    if(game_cart == NULL) return -5;
    cart_type = game_cart[CART_TYPE_OFFSET];
    rom_size = (1 << game_cart[ROM_SIZE_OFFSET]) * _32KB;
    rom_max_banks = rom_size / _16KB;
    ram_ext_size = game_cart[RAM_SIZE_OFFSET];
    switch(ram_ext_size) {
        case 0:
            ram_ext_size = 0;
            ram_ext_banks = 0;
            break;
        case 1:
            ram_ext_size = _2KB;
            ram_ext_banks = 1;
            break;
        case 2:
            ram_ext_size = _8KB;
            ram_ext_banks = 1;
            break;
        case 3:
            ram_ext_size = _32KB;
            ram_ext_banks = _32KB / _8KB;
            break;
        case 4:
            ram_ext_size = _128KB;
            ram_ext_banks = _128KB / _8KB;
            break;
    }
    if(ram_ext_size != 0) {
        extern_memory = new uint8_t[ram_ext_size];
        extern_memory_switchable = extern_memory;
    }
    return 0;
}

void Memory::MBC1_handler(const uint16_t &addr) {

}

void Memory::MBC2_handler(const uint16_t &addr) {

}

void Memory::MBC3_handler(const uint16_t &addr) {

}

void Memory::MBC5_handler(const uint16_t &addr) {

}

uint8_t Memory::readByte(const uint16_t &addr) {
    return 0;
}

uint16_t Memory::readWord(const uint16_t &addr) {
    return 0;
}

int Memory::writeByte(const uint16_t &addr) {
    return 0;
}

int Memory::writeWord(const uint16_t &addr) {
    return 0;
}

Memory::~Memory() {
	if(game_cart != NULL) munmap(game_cart, fsize);
}