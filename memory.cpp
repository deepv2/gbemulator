#include "memory.hpp"
#define ROM_LOW 0x0000
#define ROM_HIGH 0x7FFF
#define LOW_NIBBLE 0x0F
#define HIGH_NIBBLE 0xF0
#define EXT_RAM_ENABLE 0x0A
#define ROM_BANK_SEL_MASK 0x1F
#define MBC3_ROM_BANK_MASK 0x7F


/* addrInRange()
 * Checks if a given address is in range of a low and high address passed
 * as parameters. Returns 1 if in range and 0 if not.
 */
inline int Memory::addrInRange(const uint16_t & low, const uint16_t & high, const uint16_t & addr) const {
    if(addr >= low && addr <= high) return 1;
    else return 0;
}

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
    extern_memory_switchable_offset = 0;
    MemoryController = nullptr;
    ext_ram_enabled = false;
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

int Memory::initializeMemory() {
    if(game_cart == NULL) return -5;
    cart_type = game_cart[CART_TYPE_OFFSET];
    switch(cart_type) {
        case 0x01:
        case 0x02:
        case 0x03:
            MemoryController = &Memory::MBC1_handler;
            break;
        case 0x05:
        case 0x06:
            MemoryController = &Memory::MBC2_handler;
            break;
        case 0x12:
        case 0x13:
            MemoryController = &Memory::MBC3_handler;
            break;
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
            MemoryController = &Memory::MBC5_handler;
            break;
    }
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
        extern_memory = std::vector<uint8_t>(ram_ext_size, 0);
        extern_memory_switchable_offset = 0;
    }
    return 0;
}

void Memory::MBC1_handler(const uint16_t &addr, const uint8_t &data) {
    if(addrInRange(0x0000, 0x1FFF, addr)) {
        if((data & LOW_NIBBLE) == EXT_RAM_ENABLE) ext_ram_enabled = true;
        else ext_ram_enabled = false;
        return;
    } else if(addrInRange(0x2000, 0x3FFF, addr)) {
        int bank_num = data & ROM_BANK_SEL_MASK;
    }
}

void Memory::MBC2_handler(const uint16_t &addr, const uint8_t &data) {

}

void Memory::MBC3_handler(const uint16_t &addr, const uint8_t &data) {
    if(addrInRange(0x0000, 0x1FFF, addr)) {
        if((data & LOW_NIBBLE) == EXT_RAM_ENABLE) ext_ram_enabled = true;
        else ext_ram_enabled = false;
    } else if(addrInRange(0x2000, 0x3FFF, addr)) {
        int bank_num = (data & MBC3_ROM_BANK_MASK) == 0x00 ? 0x01 : (data & MBC3_ROM_BANK_MASK);
        gc_switchable_mem = &game_cart[_16KB * bank_num];
    } else if(addrInRange(0x4000, 0x5FFF, addr)) {
        extern_memory_switchable_offset = (data & LOW_NIBBLE) * _8KB;
    }
}

void Memory::MBC5_handler(const uint16_t &addr, const uint8_t &data) {

}

uint8_t Memory::readByte(const uint16_t &addr) const {
    if(addrInRange(0x0000, 0x3FFF, addr)) return game_cart[addr];
    else if(addrInRange(0x4000, 0x7FFF, addr)) return gc_switchable_mem[addr - 0x4000];
    else if(addrInRange(0xA000, 0xBFFF, addr)) return extern_memory[addr - 0xA000 + extern_memory_switchable_offset];
    else if(addrInRange(0xC000, 0xDFFF, addr)) return internal_memory[addr - 0xC000];
    else if(addrInRange(0xE000, 0xFDFF, addr)) return internal_memory[addr - 0xE000];
    return 0;
}

uint16_t Memory::readWord(const uint16_t &addr) const {
    return 0;
}

int Memory::writeByte(const uint16_t &addr, const uint8_t &data) {
    if(addrInRange(ROM_LOW, ROM_HIGH, addr)) (this->*MemoryController)(addr, data);
    else if(addrInRange(0xA000, 0xBFFF, addr)) extern_memory[addr - 0xA000 + extern_memory_switchable_offset] = data;
    else if(addrInRange(0xC000, 0xDFFF, addr)) internal_memory[addr - 0xC000] = data;
    else if(addrInRange(0xE000, 0xFDFF, addr)) internal_memory[addr - 0xE000] = data;
    else return -1;
    return 0;
}

int Memory::writeWord(const uint16_t &addr, const uint16_t &data) {
    if(addrInRange(ROM_LOW, ROM_HIGH, addr)) return -1;
    return 0;
}

Memory::~Memory() {
	if(game_cart != NULL) munmap(game_cart, fsize);
}