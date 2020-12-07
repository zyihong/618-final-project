// #include <iostream>
// #include <vector>
// #include <string>
// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include <omp.h>
#include "solver.h"

void print_output(string filename, float* grid, int m, int n) {
	ofstream of;
    of.open(filename, ios::trunc);
    of << m << " " << n << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            of << grid[i * n + j];
            if (j != n - 1) of << " ";
        }
        of << endl;
    }

	of.close();
}

void solver_serial(float* grid, int m, int n) {
	float diff;
	bool converge = false;

	while (!converge) {
		diff = 0.0f;

		for (int i = 1; i < m - 1; i++) {
			for (int j = 1; j < n - 1; j++) {
				float prev = grid[i * n + j];

				*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) / 5.0;

				float f = grid[i * n + j] - prev;
				// if (f < 0.0f) printf("%f\n", f);
				diff += (grid[i * n + j] - prev > 0.0) ? (grid[i * n + j] - prev) : (prev - grid[i * n + j]);
			}
		}

		if (diff / (m * n) < 1e-9) converge = true;
	}

	// while (!converge) {
	// 	diff = 0.0f;

	// 	for (int i = 1; i < m - 1; i++) {
	// 		for (int j = ((i % 2 == 0) ? 2 : 1); j < n - 1; j+=2) {
	// 			float prev = grid[i * n + j];

	// 			*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) / 5.0;

	// 			diff += (grid[i * n + j] - prev > 0.0) ? (grid[i * n + j] - prev) : (prev - grid[i * n + j]);
	// 		}
	// 	}

	// 	for (int i = 1; i < m - 1; i++) {
	// 		for (int j = ((i % 2 == 0) ? 1 : 2); j < n - 1; j+=2) {
	// 			float prev = grid[i * n + j];

	// 			*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) / 5.0;

	// 			diff += (grid[i * n + j] - prev > 0.0) ? (grid[i * n + j] - prev) : (prev - grid[i * n + j]);
	// 		}
	// 	}

	// 	if (diff / (m * n) < 1e-9) converge = true;
	// }

	print_output("output_serial_no.txt", grid, m, n);
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

				diff += (grid[i * n + j] - prev > 0.0) ? (grid[i * n + j] - prev) : (prev - grid[i * n + j]);
			}
		}

		if (diff / (m * n) < 1e-9) converge = true;
	}

	print_output("output_cpu.txt", grid, m, n);
}

void solver_omp_gpu(float* grid, int m, int n) {
	// float diff = 0.0f;
	bool converge = false;
	int count = 0;

	// #pragma omp target data map(grid[0:m * n])
	while (!converge) {
		float diff = 0.0f;

		if (count % 1000 == 0){
		// if (true){
			// #pragma omp target map(tofrom:grid[0:m * n]) map(tofrom:diff)
			// #pragma omp teams distribute reduction(+:diff)
			#pragma omp target map(tofrom:grid[0:m * n]) map(tofrom:diff)
			#pragma omp teams distribute parallel for reduction(+:diff) collapse(2)
			for (int i = 1; i < m - 1; i++) {
				// #pragma parallel for reduction(+:diff)
				for (int j = 1; j < n - 1; j++) {
					float prev = grid[i * n + j];

					*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) * 0.2f;

					diff += (grid[i * n + j] - prev > 0.0) ? (grid[i * n + j] - prev) : (prev - grid[i * n + j]);
				}
			}
		} else {
			#pragma omp target map(tofrom:grid[0:m * n])
			#pragma omp teams distribute parallel for collapse(2)
			for (int i = 1; i < m - 1; i++) {
				// #pragma parallel for reduction(+:diff)
				for (int j = 1; j < n - 1; j++) {
					float prev = grid[i * n + j];

					*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) / 5.0;

					// diff += (grid[i * n + j] - prev);
				}
			}
		}

		
		if (count % 1000 == 0 && diff / (m * n) < 1e-9) converge = true;
		count++;
	}

	print_output("output_gpu.txt", grid, m, n);
}

void solver_omp_blocking(float *grid, int m, int n) {
	int grid_height = m / BLOCK_HEIGHT + 1;
	int grid_width = n / BLOCK_WIDTH + 1;
	float diff = 0.0f;
	bool converge = false;

	#pragma omp target data map(grid[0:grid_height * grid_width * BLOCK_WIDTH * BLOCK_HEIGHT])
	while (!converge) {

		#pragma omp target
		#pragma omp teams distribute parallel for reduction(+:diff)
		for (int i = 0; i < grid_height; i++) {
			// #pragma parallel for reduction(+:diff)
			for (int j = 0; j < grid_width; j++) {
				for (int k = 0; k < BLOCK_HEIGHT; k++) {
					for (int l = 0; l < BLOCK_WIDTH; l++) {
						int x = j * BLOCK_WIDTH + l;
						int y = i * BLOCK_HEIGHT + k;
						if (k > 0 && l > 0 && k < BLOCK_HEIGHT - 1 && l < BLOCK_WIDTH - 1 && x > 0 && y > 0 && x < n - 1 && y < m - 1) {
							float prev = grid[x * n + y];

							*(grid + x * n + y) = (grid[(x - 1) * n + y] + grid[x * n + y] + grid[(x + 1) * n + y] + grid[x * n + y - 1] + grid[x * n + y + 1]) / 5.0;

							diff += (grid[x * n + y] - prev > 0.0) ? (grid[x * n + y] - prev) : (prev - grid[x * n + y]);
						}
					}
				}
			}
		}

		if (diff / (m * n) < 1e-9) converge = true;
	}
}

void solver_omp_red_black(float* grid, int m, int n) {
	float diff;
	bool converge = false;

	while (!converge) {
		diff = 0.0f;

		#pragma omp target map(grid[0:m * n])
		#pragma omp teams distribute parallel for
		for (int i = 1; i < m - 1; i++) {
			for (int j = ((i % 2 == 0) ? 2 : 1); j < n - 1; j+=2) {
				float prev = grid[i * n + j];

				*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) / 5.0;

				#pragma omp critical
				{
					diff += (grid[i * n + j] - prev > 0.0) ? (grid[i * n + j] - prev) : (prev - grid[i * n + j]);
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
					diff += (grid[i * n + j] - prev > 0.0) ? (grid[i * n + j] - prev) : (prev - grid[i * n + j]);
				}
			}
		}

		if (diff / (m * n) < 1e-9) converge = true;
	}

	print_output("output_red_black.txt", grid, m, n);
}

void solver_omp_gpu_test(float* grid, int m, int n) {
	// float diff = 0.0f;
	bool converge = false;
	// int count = 0;

	// #pragma omp target data map(grid[0:m * n])
	// while (!converge) {
	for (int c = 0; c < 1000; c++) {
		// float diff = 0.0f;
		
		// #pragma omp target map(tofrom:grid[0:m * n]) map(tofrom:diff)
		// #pragma omp teams distribute reduction(+:diff)
		#pragma omp target map(tofrom:grid[0:m * n]) 
		#pragma omp teams distribute parallel for collapse(2)
		for (int i = 1; i < m - 1; i++) {
			// #pragma parallel for reduction(+:diff)
			for (int j = 1; j < n - 1; j++) {
				float prev = grid[i * n + j];

				*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) * 0.2f;

				// diff += (grid[i * n + j] - prev > 0.0) ? (grid[i * n + j] - prev) : (prev - grid[i * n + j]);
			}
		}
		
	}

	// print_output("output_gpu_test.txt", grid, m, n);
}

void solver_omp_cpu_test(float* grid, int m, int n) {
	// float diff;
	// bool converge = false;

	for (int c = 0; c < 1000; c++) {

		#pragma omp parallel for
		for (int i = 1; i < m - 1; i++) {
			for (int j = 1; j < n - 1; j++) {
				float prev = grid[i * n + j];

				*(grid + i * n + j) = (grid[(i - 1) * n + j] + grid[i * n + j] + grid[(i + 1) * n + j] + grid[i * n + j - 1] + grid[i * n + j + 1]) / 5.0;

				// diff += (grid[i * n + j] - prev > 0.0) ? (grid[i * n + j] - prev) : (prev - grid[i * n + j]);
			}
		}

		// if (diff / (m * n) < 1e-9) converge = true;
	}

	// print_output("output_cpu.txt", grid, m, n);
}

void solver_omp_blocking_test(float *grid, int m, int n) {
	int grid_height = m / BLOCK_HEIGHT + 1;
	int grid_width = n / BLOCK_WIDTH + 1;
	float diff = 0.0f;
	bool converge = false;

	while (!converge) {

		#pragma omp teams distribute parallel for reduction(+:diff)
		for (int i = 0; i < grid_height; i++) {
			// #pragma parallel for reduction(+:diff)
			for (int j = 0; j < grid_width; j++) {
				for (int k = 0; k < BLOCK_HEIGHT; k++) {
					for (int l = 0; l < BLOCK_WIDTH; l++) {
						int x = j * BLOCK_WIDTH + l;
						int y = i * BLOCK_HEIGHT + k;
						if (k > 0 && l > 0 && k < BLOCK_HEIGHT - 1 && l < BLOCK_WIDTH - 1 && x > 0 && y > 0 && x < n - 1 && y < m - 1) {
							float prev = grid[x * n + y];

							*(grid + x * n + y) = (grid[(x - 1) * n + y] + grid[x * n + y] + grid[(x + 1) * n + y] + grid[x * n + y - 1] + grid[x * n + y + 1]) / 5.0;

							diff += (grid[x * n + y] - prev > 0.0) ? (grid[x * n + y] - prev) : (prev - grid[x * n + y]);
						}
					}
				}
			}
		}

		if (diff / (m * n) < 1e-9) converge = true;
	}

	// print_output("output_block_test.txt", grid, m, n);
}