#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <vector>
#include "solver.h"

#include "CycleTimer.h"

using namespace std;

int main(int argc, char* argv[]) {
	printf("There are %d devices\n", omp_get_num_devices());

	char* inputFile = argv[3];
	int numProcessors = atoi(argv[2]);

	omp_set_num_threads(numProcessors);
	omp_set_dynamic(0);

	FILE *fptr;
	fptr = fopen(inputFile, "r");

	int m, n; // m * n grid
	fscanf(fptr, "%d %d", &m, &n);
	printf("We have a grid of %d * %d\n", m, n);

	vector<vector<float>> grid(m + 2, vector<float>(n + 2, 0.0f));

	int i = 0;

	while (!feof(fptr)) {
		if (i == (m * n)) break;
		fscanf(fptr, "%d", grid[(i / n) + 1][(i % n) + 1]);
		i++;
	}

	fclose(fptr);

	double start_time = CycleTimer::currentSeconds();

	// solver_serial(grid);

	double end_time = CycleTimer::currentSeconds();

	printf("The time consumed: %lf\n", end_time - start_time);

	return 0;
}