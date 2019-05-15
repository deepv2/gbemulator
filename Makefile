all: test

test: cpu.o main.o memory.o
	g++ cpu.o main.o memory.o -o test

cpu.o: cpu.cpp cpu.hpp
	g++ -c -g cpu.cpp

memory.o: memory.hpp memory.cpp
	g++ -c -g memory.cpp

main.o: main.cpp
	g++ -c -g main.cpp

clean:
	rm test cpu.o main.o memory.o
