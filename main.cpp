#include <iomanip>
#include "cpu.hpp"
#include "memory.hpp"

int main(int argc, char **argv) {
	if(argc != 2) {
		std::cout << "Format: ./test [Game Cart file path]" << std::endl;
		return 1;
	}
	try {
		Memory m(argv[1]);
		CPU cpu(m);
	}
	catch(int e) {
		std::cout << "Error: " << e << std::endl;
	}
}
