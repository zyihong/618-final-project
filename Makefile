CXX=~/gcc-offload/gcc-offload/install/bin/g++ -std=c++11 -fopenmp -foffload=-lm -fno-fast-math -fno-associative-math -march=native

all: main grid_gen

main: main.cc
	$(CXX) main.cc solver.cc -o main

grid_gen: grid_gen.cc
	$(CXX) grid_gen.cc -o grid_gen

.PHONY: clean

clean:
	rm main grid_gen *.o

