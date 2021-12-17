// Copyright 2021 Oskin Georgii
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>
#include <vector>

#include "./min_matrix_row.h"

TEST(Parallel_Operations_MPI, getlocalmin_test) {
  int n = 5;
  int vector[5] = {1, 7, 9, 8, 6};
  int res = getLocalMinimum(vector, n);
  ASSERT_EQ(1, res);
}

TEST(Parallel_Operations_MPI, getlocalmin_test_more) {
  int n = 5;
  int vector[5] = {2, 2, 5, 8, 6};
  int res = getLocalMinimum(vector, n);
  ASSERT_EQ(2, res);
}

TEST(Parallel_Operations_MPI, thread_alloc_test) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;
  int m = 121;
  int true_rows = floor(m / size);
  int last = true_rows + (m % size);
  if (rank == 0) {
    initiateThreadAlloc(m);
  }
  if (rank != 0) {
    int numOfRows;
    MPI_Recv(&numOfRows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    if (rank != size - 1) {
      ASSERT_EQ(true_rows, numOfRows);
    } else {
      ASSERT_EQ(last, numOfRows);
    }
  }
}

TEST(Parallel_Operations_MPI, findmin_parall) { 
  int m = 20, n = 20;
  int** g_matrix = new int*[m];
  g_matrix = getRandomMatrix(m, n);
  int true_min = findMinNonParallel(g_matrix, m, n);
  printf("true min %d \n", true_min);
  int para_min = getParallelOperations(g_matrix, m, n);
  ASSERT_EQ(true_min, para_min);

}

  int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
  }
