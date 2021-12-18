// Copyright 2021 Moskvin Stanislav
#include "../../../modules/task_1/moskvin_s_integration_by_rectangles/integration_by_rectangles.h"
#include <mpi.h>

double getSequentialIntegration(double a, double b, int n, double(*Func)(double)) {
    const double h = (b - a) / static_cast<double>(n);
    double sum = 0;

    for (int i = 0; i < n; i++)
        sum += Func(a + h * (i + 0.5));
    return (sum * h);
}

double getParallelIntegration(double a, double b, int n, double(*Func)(double)) {
    const double h = (b - a) / static_cast<double>(n);
    double global_sum = 0;

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double local_sum = 0;

    for (int i = rank; i < n; i += size)
        local_sum += Func(a + h * (i + 0.5));

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return (global_sum * h);
}
