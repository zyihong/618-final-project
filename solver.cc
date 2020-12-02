// #include <iostream>
// #include <vector>
// #include <string>
// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include <omp.h>
#include "solver.h"

void solver_serial(float* grid, int m, int n) {
	float diff;
	bool converge = false;

	while (!converge) {
		diff = 0.0f;

		for (int i = 1; i < m - 1; i++) {
			for (int j = 1; j < n - 1; j++) {
				float prev = grid[i * n + j];

				*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) / 5.0;

				diff += (grid[i * n + j] - prev);
			}
		}

		if (diff / (m * n) < 1e-9) converge = true;
	}
}

void solver_omp_cpu(float* grid, int m, int n) {
	float diff;
	bool converge = false;

	while (!converge) {
		diff = 0.0f;

		#pragma omp parallel for reduction(+:diff)
		for (int i = 1; i < m - 1; i++) {
			for (int j = 1; j < n - 1; j++) {
				float prev = grid[i * n + j];

				*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) / 5.0;

				diff += (grid[i * n + j] - prev);
			}
		}

		if (diff / (m * n) < 1e-9) converge = true;
	}
}

void solver_omp_gpu(float* grid, int m, int n) {
	float diff;
	bool converge = false;

	// vector<vector<float>> grid2(grid); 

	// int count = 5;

	while (!converge) {
		diff = 0.0f;
		// float A[3] = {1.0,2.0,3.0};
		// float B[3] = {8.0,3.0,4.0};
		// float C[3] = {10.0,20.0,30.0};

		#pragma omp target map(grid[0:m * n])
		#pragma omp teams distribute parallel for reduction(+:diff)
		// for (int i = 0; i < 3; i++) C[i] += A[i] + B[i]; 
		for (int i = 1; i < m - 1; i++) {
			for (int j = 1; j < n - 1; j++) {
				float prev = grid[i * n + j];

				*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) / 5.0;

				diff += (grid[i * n + j] - prev);
			}
		}


		// swap(grid, grid2);

		if (diff / (m * n) < 1e-9) converge = true;
	}
}


void solver_omp_red_black(float* grid, int m, int n) {
	float diff;
	bool converge = false;

	// vector<vector<float>> grid2(grid); 

	// int count = 5;

	while (!converge) {
		diff = 0.0f;
		// float A[3] = {1.0,2.0,3.0};
		// float B[3] = {8.0,3.0,4.0};
		// float C[3] = {10.0,20.0,30.0};

		#pragma omp target map(grid[0:m * n])
		#pragma omp teams distribute parallel for
		// for (int i = 0; i < 3; i++) C[i] += A[i] + B[i]; 
		for (int i = 1; i < m - 1; i++) {
			for (int j = ((i % 2 == 0) ? 2 : 1); j < n - 1; j+=2) {
				float prev = grid[i * n + j];

				*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) / 5.0;

				#pragma omp critical
				{
					diff += (grid[i * n + j] - prev);
				}
			}
		}

		#pragma omp target map(grid[0:m * n])
		#pragma omp teams distribute parallel for
		for (int i = 1; i < m - 1; i++) {
			for (int j = ((i % 2 == 0) ? 1 : 2); j < n - 1; j+=2) {
				float prev = grid[i * n + j];

				*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) / 5.0;

				#pragma omp critical
				{
					diff += (grid[i * n + j] - prev);
				}
			}
		}


		// swap(grid, grid2);

		if (diff / (m * n) < 1e-9) converge = true;
	}
}

