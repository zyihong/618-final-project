#include<omp.h>
#include<stdio.h>
#include<random>
#include "CycleTimer.h"

#define N 10000000

void set_value(float *a, float *b) {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 10);
    for (int i = 0; i < N; i++) {
        a[i] = dist(e2);
        b[i] = dist(e2);
    }
}

int main() {
    float *a, *b, *c;
    a = (float *)malloc(N * sizeof(float));
    b = (float *)malloc(N * sizeof(float));
    c = (float *)malloc(N * sizeof(float));
    set_value(a, b);
    // initialize a, b and c ….
    double start_time = CycleTimer::currentSeconds();

    #pragma omp target map(a[0:N], b[0:N], c[0:N])
    #pragma teams distribute parallel for
    for(int i = 0; i < N; i++)
        c[i] = a[i] + b[i];

    double end_time = CycleTimer::currentSeconds();

	printf("The time consumed: %lf\n", end_time - start_time);
    for (int i = 0; i < 10000; i += 1000)
        printf("a = %f b = %f c = %f\n", a[i], b[i], c[i]);
    // Test results, report results …
}