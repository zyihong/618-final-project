all: main

main: main.c
	# gcc -std=c99 -fopenmp -target=nvptx-none main.c -o main
	gcc -std=c99 -fopenmp -foffload="-O3 -v" main.c -o main
