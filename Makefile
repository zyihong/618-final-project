CXX=~/gcc-offload/gcc-offload/install/bin/gcc -std=c++11 -fopenmp

all: main

main: main.cc
	$(CXX) main.cc solver.cc -o main -lstdc++

.PHONY: clean

clean:
	rm main *.o

