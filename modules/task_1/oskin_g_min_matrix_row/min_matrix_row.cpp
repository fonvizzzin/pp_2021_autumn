// Copyright 2021 Oskin Georgii
#include "../../../modules/task_1/oskin_g_min_matrix_row_mpi/min_matrix_row.h"

#include <mpi.h>

#include <algorithm>
#include <random>
#include <string>

int** getRandomMatrix(int m, int n) {
  std::random_device dev;
  std::mt19937 gen(dev());
  int** matrix = new int*[m];
  for (int i = 0; i < m; i++) {
    matrix[i] = new int[n];
  }
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i][j] = gen() % 1000;
    }
  }
  return matrix;
}

int findMinNonParallel(int** mat, int m, int n) {
  int tmp_min = mat[0][0];
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (tmp_min > mat[i][j]) tmp_min = mat[i][j];
    }
  }
  return tmp_min;
}

int getLocalMinimum(int* vec, int n) {
  int min = vec[0];
  for (int i = 0; i < n; i++) {
    if (vec[i] < min) min = vec[i];
  }
  return min;
}

void initiateThreadAlloc(int m) {
  int size, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int base_vec = floor(m / size);
  int delta = m % size;
  int last = base_vec + delta;
  for (int i = 1; i < size; i++) {
    if (i != (size - 1))
      MPI_Send(&base_vec, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    else {
      MPI_Send(&last, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  }
}

int getParallelOperations(int** g_matrix, int m, int n) {
  int size, rank, res;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int vec_size = floor(m / size);
  int adder = m % size;
  int tmp_min = 0;
  int* numOfRows = new int[size];
  int* starters = new int[m];
  if (rank == 0) {
    initiateThreadAlloc(m);

    MPI_Recv(&numOfRows[rank], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
             &status);

    for (int i = 0; i < size; i++) {
      starters[i] = i * vec_size;
    }
    starters[rank - 1] += adder;
  }
  int* tmp_ar = new int[numOfRows[rank]];
  MPI_Scatterv(g_matrix, numOfRows, starters, MPI_INT, tmp_ar, numOfRows[rank],
               MPI_INT, 0, MPI_COMM_WORLD);

  tmp_min = getLocalMinimum(tmp_ar, numOfRows[rank]);

  MPI_Reduce(&tmp_min, &res, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

  return res;
}
