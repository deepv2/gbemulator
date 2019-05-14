#include "cpu.hpp"

int main(int argc, char **argv) {
	if(argc != 2) {
		std::cout << "Format: ./test [Game Cart file path]" << std::endl;
		return 1;
	}
	CPU gb(argv[1]);
	while(1) gb.decodeOpcode();
}
