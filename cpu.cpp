#include "cpu.hpp"
#define PC_START 0x100

CPU::CPU()
{
	a = 0;
	b = 0;
	c = 0;
	d = 0;
	e = 0;
	f = 0;
	h = 0;
	l = 0;
	sp = 0; //change later
	pc = (uint16_t)PC_START;
	cycles = 0;
	flags.zero = 0;
	flags.sub = 0;
	flags.half_carry = 0;
	flags.carry = 0;
	flags.padding = 0;
	internal_memory.fill(0);
}

CPU::CPU(const char *str) : CPU() {
	loadCart(str);
}
CPU::CPU(const std::string &str) : CPU() {
	loadCart(str.c_str());
}

/* int loadCart(const char *str)
 * Creates a file mapping for the game cart and maps
 * the file to a virtual address
 * Returns: -1 on error
 */
int CPU::loadCart(const char *str) {
	#ifdef LINUX
	// open file to check if actual file
	int fd = open(str, O_RDONLY | O_EXCL, "rb");
	if(fd == -1) return -1;

	// calculate file size with istream
	std::ifstream file(str, std::ios::binary);
	int fsize_start = file.tellg();
	file.seekg(0, file.end);
	fsize = file.tellg();
	fsize = fsize - fsize_start;


	// use mmap to map game cart address to CPU object
	game_cart = (uint8_t*)mmap(NULL, fsize, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if(game_cart == MAP_FAILED) return 1;
	return 0;

	// TO-DO: Windows File-mapping implementation
	#else
	return -1;
	#endif
}


int CPU::decodeOpcode() {
	uint8_t opcode, imm8;
	opcode = game_cart[pc++];
	switch(opcode) {
		// 8-bit LD Ops
		case 0x06: // LD B,n
			imm8 = game_cart[pc++];
			b = (int8_t)imm8;
			cycles += 8;
			break;
		case 0x0E: // LD C,n
			imm8 = game_cart[pc++];
			c = (int8_t)imm8;
			cycles += 8;
			break;
		case 0x16: // LD D,n
			imm8 = game_cart[pc++];
			d = (int8_t)imm8;
			cycles += 8;
			break;
		case 0x1E: // LD E,n
			imm8 = game_cart[pc++];
			e = (int8_t)imm8;
			cycles += 8;
			break;
		case 0x26: // LD H,n
			imm8 = game_cart[pc++];
			h = (int8_t)imm8;
			cycles += 8;
			break;
		case 0x2E: // LD L,n
			imm8 = game_cart[pc++];
			l = (int8_t)imm8;
			cycles += 8;
        	break;
		case 0x78: // LD A,B
			a = b;
			cycles += 4;
			break;
		case 0x79: // LD A,C
			a = c;
			cycles += 4;
			break;
		case 0x7A: // LD A,D
			a = d;
			cycles += 4;
			break;
		case 0x7B: // LD A,E
			a = e;
			cycles += 4;
			break;
		case 0x7C: // LD A,H
			a = h;
			cycles += 4;
			break;
		case 0x7D: // LD A,L
			a = l;
			cycles += 4;
			break;
		case 0x7E: // LD A,(HL)
			cycles += 8;
			break;
		case 0x7F: // LD A,A
			cycles += 4;
			break;
		case 0x40: // LD B,B
			cycles += 4;
			break;
		case 0x41: // LD B,C
			b = c;
			cycles += 4;
			break;
		case 0x42: // LD B,D
			b = d;
			cycles += 4;
			break;
		case 0x43: // LD B,E
			b = e;
			cycles += 4;
			break;
		case 0x44: // LD B,H
			b = h;
			cycles += 4;
			break;
		case 0x45: // LD B,L
			b = l;
			cycles += 4;
			break;
		case 0x46: // LD B,(HL)
			cycles += 8;
			break;
		case 0x47: // LD B,A
			b = a;
			cycles += 4;
			break;
		case 0x48: // LD C,B
			c = b;
			cycles += 4;
			break;
		case 0x49: // LD C,C
			cycles += 4;
			break;
		case 0x4A: // LD C,D
			c = d;
			cycles += 4;
			break;
		case 0x4B: // LD C,E
			c = e;
			cycles += 4;
			break;
		case 0x4C: // LD C,H
			c = h;
			cycles += 4;
			break;
		case 0x4D: // LD C,L
			c = l;
			cycles += 4;
			break;
		case 0x4E: // LD C,(HL)
			cycles += 8;
			break;
		case 0x4F: // LD C,A
			c = a;
			cycles += 4;
			break;
		case 0x50: // LD D,B
			d = b;
			cycles += 4;
			break;
		case 0x51: // LD D,C
			d = c;
			cycles += 4;
			break;
		case 0x52: // LD D,D
			cycles += 4;
			break;
		case 0x53: // LD D,E
			d = e;
			cycles += 4;
			break;
		case 0x54: // LD D,H
			d = h;
			cycles += 4;
			break;
		case 0x55: // LD D,L
			d = l;
			cycles += 4;
			break;
		case 0x56: // LD D,(HL)
			cycles += 8;
			break;
		case 0x57: // LD D,A
			d = a;
			cycles += 4;
			break;
		case 0x58: // LD E,B
			e = b;
			cycles += 4;
			break;
		case 0x59: // LD E,C
			e = c;
			cycles += 4;
			break;
		case 0x5A: // LD E,D
			e = d;
			cycles += 4;
			break;
		case 0x5B: // LD E,E
			cycles += 4;
			break;
		case 0x5C: // LD E,H
			e = h;
			cycles += 4;
			break;
		case 0x5D: // LD E,L
			e = l;
			cycles += 4;
			break;
		case 0x5E: // LD E,(HL)
			cycles += 8;
			break;
		case 0x5F: // LD E,A
			e = a;
			cycles += 4;
			break;
		case 0x60: // LD H,B
			h = b;
			cycles += 4;
			break;
		case 0x61: // LD H,C
			h = c;
			cycles += 4;
			break;
		case 0x62: // LD H,D
			h = d;
			cycles += 4;
			break;
		case 0x63: // LD H,E
			h = e;
			cycles += 4;
			break;
		case 0x64: // LD H,H
			cycles += 4;
			break;
		case 0x65: // LD H,L
			h = l;
			cycles += 4;
			break;
		case 0x66: // LD H,(HL)
			cycles += 8;
			break;
		case 0x67: // LD H,A
			h = a;
			cycles += 4;
			break;
		case 0x68: // LD L,B
			l = b;
			cycles += 4;
			break;
		case 0x69: // LD L,C
			l = c;
			cycles += 4;
			break;
		case 0x6A: // LD L,D
			l = d;
			cycles += 4;
			break;
		case 0x6B: // LD L,E
			l = e;
			cycles += 4;
			break;
		case 0x6C: // LD L,H
			l = h;
			cycles += 4;
			break;
		case 0x6D: // LD L,L
			cycles += 4;
			break;
		case 0x6E: // LD L,(HL)
			cycles += 8;
			break;
		case 0x6F: // LD L,A
			l = a;
			cycles += 4;
			break;
		case 0x70: // LD (HL), B
			cycles += 8;
			break;
		case 0x71: // LD (HL), C
			cycles += 8;
			break;
		case 0x72: // LD (HL), D
			cycles += 8;
			break;
		case 0x73: // LD (HL), E
			cycles += 8;
			break;
		case 0x74: // LD (HL), H
			cycles += 8;
			break;
		case 0x75: // LD (HL), L
			cycles += 8;
			break;
		case 0x36: // LD (HL), n
			cycles += 12;
			break;
		case 0x0A: // LD A,(BC)
			cycles += 8;
			break;
		case 0x1A: // LD A,(DE)
			cycles += 8;
			break;
		case 0xFA: // LD A,(nn)
			cycles += 16;
			break;
		case 0x3E: // LD A, #
			cycles += 8;
			break;
		case 0x02: // LD (BC),A
			cycles += 8;
			break;
		case 0x12: // LD (DE),A
			cycles += 8;
			break;
		case 0x77: // LD (HL),A
			cycles += 8;
			break;
		case 0xEA: // LD (nn),A
			cycles += 16;
			break;
		case 0xF2: // LD A,($FF00+C)
			cycles += 8;
			break;
		case 0xE2: // LD ($FF00+C),A
			cycles += 8;
			break;
		case 0x3A: // LD A,(HL) and DEC HL
			cycles += 8;
			break;
		case 0x32: // LD (HL),A and DEC HL
			cycles += 8;
			break;
		case 0x2A: // LD A,(HL) and INC HL
			cycles += 8;
			break;
		case 0x22: // LD (HL),A and INC HL
			cycles += 8;
			break;
		case 0xE0: // LD ($FF00+n),A
			cycles += 12;
			break;
		case 0xF0: // LD A,($FF00+n)
			cycles += 12;
			break;
		// 16-bit LD Ops
		case 0x01: // LD BC,nn
			cycles += 12;
			break;
		case 0x11: // LD DE,nn
			cycles += 12;
			break;
		case 0x21: // LD HL,nn
			cycles += 12;
			break;
		case 0x31: // LD SP,nn
			cycles += 12;
			break;
		case 0xF9: // LD SP,HL
			cycles += 8;
			break;
		case 0xF8: // LDHL SP,n; flags
			cycles += 12;
			break;
		case 0x08: // LD (nn),SP
			cycles += 20;
			break;
		case 0xF5: // PUSH AF
			cycles += 16;
			break;
		case 0xC5: // PUSH BC
			cycles += 16;
			break;
		case 0xD5: // PUSH DE
			cycles += 16;
			break;
		case 0xE5: // PUSH HL
			cycles += 16;
			break;
		case 0xF1: // POP AF
			cycles += 12;
			break;
		case 0xC1: // POP BC
			cycles += 12;
			break;
		case 0xD1: // POP DE
			cycles += 12;
			break;
		case 0xE1: // POP HL
			cycles += 12;
			break;
		// 8-bit ALU
		case 0x87: // ADD A,A
			cycles += 4;
			break;
		case 0x80: // ADD A,B
			cycles += 4;
			break;
		case 0x81: // ADD A,C
			cycles += 4;
			break;
		case 0x82: // ADD A,D
			cycles += 4;
			break;
		case 0x83: // ADD A,E
			cycles += 4;
			break;
		case 0x84: // ADD A,H
			cycles += 4;
			break;
		case 0x85: // ADD A,L
			cycles += 4;
			break;
		case 0x86: // ADD A,(HL)
			cycles += 8;
			break;
		case 0xC6: // ADD A,#
			cycles += 8;
			break;
		case 0x8F: // ADC A,A
			cycles += 4;
			break;
		case 0x88: // ADC A,B
			cycles += 4;
			break;
		case 0x89: // ADC A,C
			cycles += 4;
			break;
		case 0x8A: // ADC A,D
			cycles += 4;
			break;
		case 0x8B: // ADC A,E
			cycles += 4;
			break;
		case 0x8C: // ADC A,H
			cycles += 4;
			break;
		case 0x8D: // ADC A,L
			cycles += 4;
			break;
		case 0x8E: // ADC A,(HL)
			cycles += 8;
			break;
		case 0xCE: // ADC A,#
			cycles += 8;
			break;
		case 0x97: // SUB A
			cycles += 4;
			break;
		case 0x90: // SUB B
			cycles += 4;
			break;
		case 0x91: // SUB C
			cycles += 4;
			break;
		case 0x92: // SUB D
			cycles += 4;
			break;
		case 0x93: // SUB E
			cycles += 4;
			break;
		case 0x94: // SUB H
			cycles += 4;
			break;
		case 0x95: // SUB L
			cycles += 4;
			break;
		case 0x96: // SUB (HL)
			cycles += 8;
			break;
		case 0xD6: // SUB #
			cycles += 8;
			break;
		case 0x9F: // SBC A,A
			cycles += 4;
			break;
		case 0x98: // SBC A,B
			cycles += 4;
			break;
		case 0x99: // SBC A,C
			cycles += 4;
			break;
		case 0x9A: // SBC A,D
			cycles += 4;
			break;
		case 0x9B: // SBC A,E
			cycles += 4;
			break;
		case 0x9C: // SBC A,H
			cycles += 4;
			break;
		case 0x9D: // SBC A,L
			cycles += 4;
			break;
		case 0x9E: // SBC A,(HL)
			cycles += 8;
			break;
		// case 0x9F: // SBC A,# unknown opcode?
		case 0xA7: // AND A
			cycles += 4;
			break;
		case 0xA0: // AND B
			cycles += 4;
			break;
		case 0xA1: // AND C
			cycles += 4;
			break;
		case 0xA2: // AND D
			cycles += 4;
			break;
		case 0xA3: // AND E
			cycles += 4;
			break;
		case 0xA4: // AND H
			cycles += 4;
			break;
		case 0xA5: // AND L
			cycles += 4;
			break;
		case 0xA6: // AND (HL)
			cycles += 8;
			break;
		case 0xE6: // AND #
			cycles += 8;
			break;
		case 0xB7: // OR A
			cycles += 4;
			break;
		case 0xB0: // OR B
			cycles += 4;
			break;
		case 0xB1: // OR C
			cycles += 4;
			break;
		case 0xB2: // OR D
			cycles += 4;
			break;
		case 0xB3: // OR E
			cycles += 4;
			break;
		case 0xB4: // OR H
			cycles += 4;
			break;
		case 0xB5: // OR L
			cycles += 4;
			break;
		case 0xB6: // OR (HL)
			cycles += 8;
			break;
		case 0xF6: // OR #
			cycles += 8;
			break;
		case 0xAF: // XOR A
			cycles += 4;
			break;
		case 0xA8: // XOR B
			cycles += 4;
			break;
		case 0xA9: // XOR C
			cycles += 4;
			break;
		case 0xAA: // XOR D
			cycles += 4;
			break;
		case 0xAB: // XOR E
			cycles += 4;
			break;
		case 0xAC: // XOR H
			cycles += 4;
			break;
		case 0xAD: // XOR L
			cycles += 4;
			break;
		case 0xAE: // XOR (HL)
			cycles += 8;
			break;
		case 0xEE: // XOR #
			cycles += 8;
			break;
		case 0xBF: // CP A
			cycles += 4;
			break;
		case 0xB8: // CP B
			cycles += 4;
			break;
		case 0xB9: // CP C
			cycles += 4;
			break;
		case 0xBA: // CP D
			cycles += 4;
			break;
		case 0xBB: // CP E
			cycles += 4;
			break;
		case 0xBC: // CP H
			cycles += 4;
			break;
		case 0xBD: // CP L
			cycles += 4;
			break;
		case 0xBE: // CP (HL)
			cycles += 8;
			break;
		case 0xFE: // CP #
			cycles += 8;
			break;
		case 0x3C: // INC A
			cycles += 4;
			break;
		case 0x04: // INC B
			cycles += 4;
			break;
		case 0x0C: // INC C
			cycles += 4;
			break;
		case 0x14: // INC D
			cycles += 4;
			break;
		case 0x1C: // INC E
			cycles += 4;
			break;
		case 0x24: // INC H
			cycles += 4;
			break;
		case 0x2C: // INC L
			cycles += 4;
			break;
		case 0x34: // INC (HL)
			cycles += 12;
			break;
		case 0x3D: // DEC A
			cycles += 4;
			break;
		case 0x05: // DEC B
			cycles += 4;
			break;
		case 0x0D: // DEC C
			cycles += 4;
			break;
		case 0x15: // DEC D
			cycles += 4;
			break;
		case 0x1D: // DEC E
			cycles += 4;
			break;
		case 0x25: // DEC H
			cycles += 4;
			break;
		case 0x2D: // DEC L
			cycles += 4;
			break;
		case 0x35: // DEC (HL)
			cycles += 12;
			break;
		// 16-bit ALU Ops
		case 0x09: // ADD HL,BC
			cycles += 8;
			break;
		case 0x19: // ADD HL,DE
			cycles += 8;
			break;
		case 0x29: // ADD HL,HL
			cycles += 8;
			break;
		case 0x39: // ADD HL,SP
			cycles += 8;
			break;
		case 0xE8: // ADD SP,#
			cycles += 16;
			break;
		case 0x03: // INC BC
			cycles += 8;
			break;
		case 0x13: // INC DE
			cycles += 8;
			break;
		case 0x23: // INC HL
			cycles += 8;
			break;
		case 0x33: // INC SP
			cycles += 8;
			break;
		case 0x0B: // DEC BC
			cycles += 8;
			break;
		case 0x1B: // DEC DE
			cycles += 8;
			break;
		case 0x2B: // DEC HL
			cycles += 8;
			break;
		case 0x3B: // DEC SP
			cycles += 8;
			break;
		// Misc
		case 0xCB:
			imm8 = game_cart[pc++];
			switch(imm8) {
				case 0x37: // SWAP A
					cycles += 8;
					break;
				case 0x30: // SWAP B
					cycles += 8;
					break;
				case 0x31: // SWAP C
					cycles += 8;
					break;
				case 0x32: // SWAP D
					cycles += 8;
					break;
				case 0x33: // SWAP E
					cycles += 8;
					break;
				case 0x34: // SWAP H
					cycles += 8;
					break;
				case 0x35: // SWAP L
					cycles += 8;
					break;
				case 0x36: // SWAP (HL)
					cycles += 16;
					break;
			}
			break;
		case 0x27: // DAA
			cycles += 4;
			break;
		case 0x2F: // CPL [NOT A]
			cycles += 4;
			break;
		case 0x3F: // CCF
			cycles += 4;
			break;
		case 0x37: // SCF
			cycles += 4;
			break;
		case 0x00: // NOP
			cycles += 4;
			break;
		case 0x76: // HALT
			cycles += 4;
			break;
		case 0x10: // STOP
			imm8 = game_cart[pc++];
			if(imm8 == 0x00) {
				cycles += 4;
				break;
			}
			break;
		case 0xF3: // DI
			cycles += 4;
			break;
		case 0xFB: // EI
			cycles += 4;
			break;
		// Rotating and Shifting

	}
}

CPU::~CPU()
{
	munmap(game_cart, fsize);
}
