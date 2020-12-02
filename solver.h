#ifndef __SOLVER_H__
#define __SOLVER_H__

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

using namespace std;

void solver_serial(float* grid, int m, int n);
void solver_omp_cpu(float* grid, int m, int n);
void solver_omp_gpu(float* grid, int m, int n);
void solver_omp_red_black(float* grid, int m, int n);

#endif