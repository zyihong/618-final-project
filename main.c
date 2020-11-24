#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char* argv[]) {
	printf("There are %d devices\n", omp_get_num_devices());
	return 0;
}