#ifndef __SOLVER_H__
#define __SOLVER_H__

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#include<fstream>

using namespace std;

#define BLOCK_WIDTH 16
#define BLOCK_HEIGHT 16

void print_output(string filename, float* grid, int m, int n);

void solver_serial(float* grid, int m, int n);
void solver_omp_cpu(float* grid, int m, int n);
void solver_omp_gpu(float* grid, int m, int n);
void solver_omp_blocking(float* grid, int m, int n);
void solver_omp_red_black(float* grid, int m, int n);
void solver_omp_gpu_test(float* grid, int m, int n);
void solver_omp_cpu_test(float* grid, int m, int n);
void solver_omp_blocking_test(float *grid, int m, int n);

#endif