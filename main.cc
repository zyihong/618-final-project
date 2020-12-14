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
	int grid_height = m / BLOCK_HEIGHT + 1;
	int grid_width = n / BLOCK_WIDTH + 1;

	float* grid = (float*)malloc(m * n * sizeof(float));
	// float* grid = (float*)malloc(grid_height * BLOCK_HEIGHT * grid_width * BLOCK_WIDTH * sizeof(float));

	int i = 0;

	while (!feof(fptr)) {
		// cout << "current i: " << i << endl;
		if (i == (m * n)) break;
		// fscanf(fptr, "%f", &grid[(i / n)][(i % n)]);
		int pos = i;
		// int grid_x = (i % n) / BLOCK_WIDTH;
		// int grid_y = (i / n) / BLOCK_HEIGHT;
		// int block_x = (i % n) % BLOCK_WIDTH;
		// int block_y = (i / n) % BLOCK_HEIGHT;
		// pos = (grid_y * grid_width + grid_x) * BLOCK_HEIGHT * BLOCK_WIDTH + block_y * BLOCK_WIDTH + block_x;
		fscanf(fptr, "%f", grid + pos);
		i++;
	}

	fclose(fptr);

	// cout << "grid: " << endl;
	// for (int r = 0; r < m; r++) {
	// 	for (int c = 0; c < n; c++) cout << grid[r][c] << " ";
	// 	cout << endl;
	// }

	double start_time = CycleTimer::currentSeconds();

	// solver_serial(grid, m, n);
	// solver_omp_gpu(grid, m, n);
	// solver_omp_red_black(grid, m, n);
	solver_omp_cpu(grid, m, n);
	// solver_omp_gpu_test(grid, m, n);
	// solver_omp_cpu_test(grid, m, n);
	// solver_omp_blocking(grid, m, n);
	// solver_omp_gpu_test_slide(grid, m, n);

	double end_time = CycleTimer::currentSeconds();

	printf("The time consumed: %lf\n", end_time - start_time);

	free(grid);

	return 0;
}