// Copyright 2021 Oskin Georgii
#ifndef MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
#define MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_

#include <string>
#include <vector>

int** getRandomMatrix(int m, int n);
int getLocalMinimum(int* vec, int n);
int getParallelOperations(int** g_matrix, int m, int n);
void initiateThreadAlloc(int m);
int findMinNonParallel(int** mat, int m, int n);

#endif  // MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
