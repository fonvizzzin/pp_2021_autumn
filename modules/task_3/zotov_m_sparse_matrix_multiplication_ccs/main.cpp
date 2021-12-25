// Copyright 2018 Nesterov Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./sparse_matrix_multiplication_ccs.h"
#include <gtest-mpi-listener.hpp>


TEST(Parallel_Operations_MPI, Test_Sum) {
    Matrix A;
    Matrix B;
    Matrix C;
    Matrix D;
    Matrix G1;
    Matrix G2;
    int ProcRank, ProcNum;
    double t1, t2;


    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    //A = getRandomMatrix(4);
    //B = getRandomMatrix(4);

    A.value = { 1,3,4,8 };
    A.row = { 0,1,1,3 };
    A.column = { 0,1,1,2,4 };
    A.size = 4;

    B.value = { 2,4,6,3,2,5 };
    B.row = { 0,3,2,0,3,1 };
    B.column = { 0,2,3,5,6 };
    B.size = 4;
    C = getRandomMatrix(100);
    D = getRandomMatrix(100);
    
    if (ProcRank == 0) {
        t1 = MPI_Wtime();
    }
    
    if (ProcRank == 0) {
        G1 = multiplication(C,D);
    }
    

    if (ProcRank == 0) {
        t2 = MPI_Wtime();
    }
    if (ProcRank == 0) {
        //printMatrix(G1);
        //printCoef(G1);
    }

    if (ProcRank == 0) {
        std::cout << "//////////////////////////////////" << std::endl;
        //printMatrix(G2);
        //printCoef(G2);
        std::cout << std::endl << t2 - t1 << std::endl;
    }
    if (ProcRank == 0) {
        t1 = MPI_Wtime();
    }

    G2 = parallelMultiplication(C,D);

    if (ProcRank == 0) {
        t2 = MPI_Wtime();
        std::cout << std::endl << t2 - t1 << std::endl;
    }
    if (ProcRank == 0) {
        //printMatrix(G2);
        //printCoef(G2);
    }
    
    ASSERT_EQ(1,1);
}


/*

TEST(Parallel_Operations_MPI, Test_Sum) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_sum = getParallelOperations(global_vec, count_size_vector, "+");

    if (rank == 0) {
        int reference_sum = getSequentialOperations(global_vec, "+");
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Diff) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_diff = getParallelOperations(global_vec, count_size_vector, "-");

    if (rank == 0) {
        int reference_diff = getSequentialOperations(global_vec, "-");
        ASSERT_EQ(reference_diff, global_diff);
    }
}

TEST(Parallel_Operations_MPI, Test_Diff_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_diff = getParallelOperations(global_vec, count_size_vector, "-");

    if (rank == 0) {
        int reference_diff = getSequentialOperations(global_vec, "-");
        ASSERT_EQ(reference_diff, global_diff);
    }
}

TEST(Parallel_Operations_MPI, Test_Max) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_max;
    global_max = getParallelOperations(global_vec, count_size_vector, "max");

    if (rank == 0) {
        int reference_max = getSequentialOperations(global_vec, "max");
        ASSERT_EQ(reference_max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_Max_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_max;
    global_max = getParallelOperations(global_vec, count_size_vector, "max");

    if (rank == 0) {
        int reference_max = getSequentialOperations(global_vec, "max");
        ASSERT_EQ(reference_max, global_max);
    }
}

*/

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
