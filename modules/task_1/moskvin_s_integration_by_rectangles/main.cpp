// Copyright 2021 Moskvin Stanislav
#include "../../../modules/task_1/moskvin_s_integration_by_rectangles/integration_by_rectangles.h"
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>

TEST(parallel_operations_mpi, can_integration_correct_using_sequential_method) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        auto Func = [](double x) -> double {
            return 1;
        };

        double error = pow(10.0, -8);
        double seq_res = getSequentialIntegration(0, 2, 10, Func);
        double res = 2;
        ASSERT_NEAR(res, seq_res, error);
    }
}

TEST(parallel_operations_mpi, can_integration_correct_using_parallel_method) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto Func = [](double x) -> double {
        return 1;
    };

    double par_res = getParallelIntegration(0, 2, 10, Func);

    if (rank == 0) {
        double error = pow(10.0, -8);
        double res = 2;
        ASSERT_NEAR(res, par_res, error);
    }
}

TEST(test_different_submethods, integration_sequential_submethods) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        auto Func = [](double x) -> double {
            return x;
        };

        double error = pow(10.0, -2);
        double seq_res = getSequentialIntegration(0, 2, 1000, Func);
        double res = 2;
        ASSERT_NEAR(res, seq_res, error);
    }
}

TEST(test_different_submethods, integration_parallel_submethods) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto Func = [](double x) -> double {
        return x;
    };

    double par_res = getParallelIntegration(0, 2, 1000, Func);

    if (rank == 0) {
        double error = pow(10.0, -2);
        double res = 2;
        ASSERT_NEAR(res, par_res, error);
    }
}

TEST(test_different_submethods, integration_sequential_and_parallel_are_equival_xx) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto Func = [](double x) -> double {
        return x*x;
    };

    double par_res = getParallelIntegration(0, 2, 100, Func);

    if (rank == 0) {
        double error = pow(10.0, -8);
        double seq_res = getSequentialIntegration(0, 2, 100, Func);
        ASSERT_NEAR(seq_res, par_res, error);
    }
}

TEST(test_different_submethods, integration_sequential_and_parallel_are_equival_sinx) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto Func = [](double x) -> double {
        return sin(x);
    };

    double par_res = getParallelIntegration(0, 2, 100, Func);

    if (rank == 0) {
        double error = pow(10.0, -8);
        double seq_res = getSequentialIntegration(0, 2, 100, Func);
        ASSERT_NEAR(seq_res, par_res, error);
    }
}

TEST(test_different_submethods, integration_sequential_and_parallel_are_equival_cosx) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto Func = [](double x) -> double {
        return cos(x);
    };

    double par_res = getParallelIntegration(0, 2, 100, Func);

    if (rank == 0) {
        double error = pow(10.0, -8);
        double seq_res = getSequentialIntegration(0, 2, 100, Func);
        ASSERT_NEAR(seq_res, par_res, error);
    }
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
