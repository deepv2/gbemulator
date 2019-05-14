all: test

test: cpu.o main.o
	g++ cpu.o main.o -o test

cpu.o: cpu.cpp cpu.hpp
	g++ -c -g cpu.cpp

main.o: main.cpp
	g++ -c -g main.cpp

clean:
	rm test cpu.o main.o
