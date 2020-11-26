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

	char* inputFile = argv[2];
	int numProcessors = atoi(argv[1]);

	omp_set_num_threads(numProcessors);
	omp_set_dynamic(0);

	FILE *fptr;
	fptr = fopen(inputFile, "r");

	int m, n; // m * n grid
	fscanf(fptr, "%d %d", &m, &n);
	printf("We have a grid of %d * %d\n", m, n);

	// vector<vector<float>> grid(m, vector<float>(n, 0.0f));
	float* grid = (float*)malloc(m * n * sizeof(float));

	int i = 0;

	while (!feof(fptr)) {
		// cout << "current i: " << i << endl;
		if (i == (m * n)) break;
		// fscanf(fptr, "%f", &grid[(i / n)][(i % n)]);
		fscanf(fptr, "%f", grid + i);
		i++;
	}

	fclose(fptr);

	// cout << "grid: " << endl;
	// for (int r = 0; r < m; r++) {
	// 	for (int c = 0; c < n; c++) cout << grid[r][c] << " ";
	// 	cout << endl;
	// }

	double start_time = CycleTimer::currentSeconds();

	// solver_serial(grid);
	solver_omp_gpu(grid, m, n);

	double end_time = CycleTimer::currentSeconds();

	printf("The time consumed: %lf\n", end_time - start_time);

	return 0;
}