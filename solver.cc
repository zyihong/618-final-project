// #include <iostream>
// #include <vector>
// #include <string>
// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include <omp.h>
#include "solver.h"

using namespace std;

void solver_serial(vector<vector<float>> grid) {
	float diff;
	bool converge = false;

	int m = grid.size();
	int n = grid[0].size();

	vector<vector<float>> grid2(grid); 

	while (!converge) {
		diff = 0.0f;

		for (int i = 1; i < m - 1; i++) {
			for (int j = 1; j < n - 1; j++) {
				// prev = grid[i][j];

				grid2[i][j] = (grid[i - 1][j] + grid[i][j] + grid[i + 1][j] + grid[i][j - 1] + grid[i][j + 1]) / 5.0;

				diff += (grid2[i][j] - grid[i][j]);
			}
		}

		swap(grid, grid2);

		if (diff / ((m - 2) * (n - 2)) < 1e-9) converge = true;
	}
}


void solver_omp(vector<vector<float>> grid) {
	float diff;
	bool converge = false;

	int m = grid.size();
	int n = grid[0].size();

	vector<vector<float>> grid2(grid); 

	while (!converge) {
		diff = 0.0f;

		#pragma omp parallel for
		for (int i = 1; i < m - 1; i++) {
			for (int j = 1; j < n - 1; j++) {
				// prev = grid[i][j];

				grid2[i][j] = (grid[i - 1][j] + grid[i][j] + grid[i + 1][j] + grid[i][j - 1] + grid[i][j + 1]) / 5.0;

				#pragma omp critical
				{
					diff += (grid2[i][j] - grid[i][j]);
				}
			}
		}

		swap(grid, grid2);

		if (diff / ((m - 2) * (n - 2)) < 1e-9) converge = true;
	}
}

