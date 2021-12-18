// Copyright 2021 Troegubova Alexandra
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./graham_scan.h"
#include <gtest-mpi-listener.hpp>

TEST(grahan_scan_test, correct_finding_of_a_convex_hull_on_a_set_of_10_points) {
    int size = 10;
    std::vector<std::vector<int>> ps;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int** shell_par;
    if (proc_rank == 0) {
        ps = GetPoints(size, 600, 350, 330);

        std::vector<int*> t_seq1(size);
        for (int i = 0; i < size; i++)
            t_seq1[i] = ps[i].data();
        std::vector<int*> t_par1(size);
        for (int i = 0; i < size; i++)
            t_par1[i] = ps[i].data();

        int size_shell_par;
        int* p_ssp = &size_shell_par;
        // double time_1 = MPI_Wtime();
        shell_par = GrahamParallel(t_par1.data(), size, p_ssp);
        // double time_2 = MPI_Wtime();
        // std::cout << "Time spent on GrahamParallel execution: "<< (time_2 - time_1) <<  "\n";

        int size_shell_seq;
        int* p_sss = &size_shell_seq;
        // time_1 = MPI_Wtime();
        int** shell_seq = GrahamSequential(t_seq1.data(), size, p_sss);
        // time_2 = MPI_Wtime();
        // std::cout << "Time spent on GrahamSequential execution " << (time_2 - time_1) <<  "\n";
        int control = 1;
        if (size_shell_par != size_shell_seq) {
            control = 0;
        } else {
            for (int i = 0; i < size_shell_par; i++) {
                if ((shell_par[i][0] != shell_seq[i][0]) || (shell_par[i][1] != shell_seq[i][1])) {
                    control = 0;
                    break;
                }
            }
        }
        ASSERT_EQ(control, 1);
    } else {
        int* ptr = nullptr;
        int** ptr_p = &ptr;
        int size_shell_par;
        int* p_ssp = &size_shell_par;
        shell_par = GrahamParallel(ptr_p, size, p_ssp);
        delete[] shell_par;
    }   
}

TEST(grahan_scan_test, correct_finding_of_a_convex_hull_on_a_set_of_50_points) {
    int size = 50;
    std::vector<std::vector<int>> ps;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int** shell_par;
    if (proc_rank == 0) {
        ps = GetPoints(size, 600, 350, 330);

        std::vector<int*> t_seq2(size);
        for (int i = 0; i < size; i++)
            t_seq2[i] = ps[i].data();
        std::vector<int*> t_par2(size);
        for (int i = 0; i < size; i++)
            t_par2[i] = ps[i].data();

        int size_shell_par;
        int* p_ssp = &size_shell_par;
        // double time_1 = MPI_Wtime();
        shell_par = GrahamParallel(t_par2.data(), size, p_ssp);
        // double time_2 = MPI_Wtime();
        // std::cout << "Time spent on GrahamParallel execution: " << (time_2 - time_1) << "\n";

        int size_shell_seq;
        int* p_sss = &size_shell_seq;
        // time_1 = MPI_Wtime();
        int** shell_seq = GrahamSequential(t_seq2.data(), size, p_sss);
        // time_2 = MPI_Wtime();
        // std::cout << "Time spent on GrahamSequential execution " << (time_2 - time_1) << "\n";
        int control = 1;
        if (size_shell_par != size_shell_seq) {
            control = 0;
        } else {
            for (int i = 0; i < size_shell_par; i++) {
                if ((shell_par[i][0] != shell_seq[i][0]) || (shell_par[i][1] != shell_seq[i][1])) {
                    control = 0;
                    break;
                }
            }
        }
        ASSERT_EQ(control, 1);
    } else {
        int* ptr = nullptr;
        int** ptr_p = &ptr;
        int size_shell_par;
        int* p_ssp = &size_shell_par;
        shell_par = GrahamParallel(ptr_p, size, p_ssp);
    }
}

TEST(grahan_scan_test, correct_finding_of_a_convex_hull_on_a_set_of_100_points) {
    int size = 100;
    std::vector<std::vector<int>> ps;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int** shell_par;
    if (proc_rank == 0) {
        ps = GetPoints(size, 600, 350, 330);

        std::vector<int*> t_seq3(size);
        for (int i = 0; i < size; i++)
            t_seq3[i] = ps[i].data();
        std::vector<int*> t_par3(size);
        for (int i = 0; i < size; i++)
            t_par3[i] = ps[i].data();

        int size_shell_par;
        int* p_ssp = &size_shell_par;
        // double time_1 = MPI_Wtime();
        shell_par = GrahamParallel(t_par3.data(), size, p_ssp);
        // double time_2 = MPI_Wtime();
        // std::cout << "Time spent on GrahamParallel execution: " << (time_2 - time_1) << "\n";

        int size_shell_seq;
        int* p_sss = &size_shell_seq;
        // time_1 = MPI_Wtime();
        int** shell_seq = GrahamSequential(t_seq3.data(), size, p_sss);
        // time_2 = MPI_Wtime();
        // std::cout << "Time spent on GrahamSequential execution " << (time_2 - time_1) << "\n";
        int control = 1;
        if (size_shell_par != size_shell_seq) {
            control = 0;
        } else {
            for (int i = 0; i < size_shell_par; i++) {
                if ((shell_par[i][0] != shell_seq[i][0]) || (shell_par[i][1] != shell_seq[i][1])) {
                    control = 0;
                    break;
                }
            }
        }
        ASSERT_EQ(control, 1);
    } else {
        int* ptr = nullptr;
        int** ptr_p = &ptr;
        int size_shell_par;
        int* p_ssp = &size_shell_par;
        shell_par = GrahamParallel(ptr_p, size, p_ssp);
    }
}

TEST(grahan_scan_test, correct_finding_of_a_convex_hull_on_a_set_of_500_points) {
    int size = 500;
    std::vector<std::vector<int>> ps;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int** shell_par;
    if (proc_rank == 0) {
        ps = GetPoints(size, 600, 350, 330);

        std::vector<int*> t_seq4(size);
        for (int i = 0; i < size; i++)
            t_seq4[i] = ps[i].data();
        std::vector<int*> t_par4(size);
        for (int i = 0; i < size; i++)
            t_par4[i] = ps[i].data();

        int size_shell_par;
        int* p_ssp = &size_shell_par;
        // double time_1 = MPI_Wtime();
        shell_par = GrahamParallel(t_par4.data(), size, p_ssp);
        // double time_2 = MPI_Wtime();
        // std::cout << "Time spent on GrahamParallel execution: " << (time_2 - time_1) << "\n";

        int size_shell_seq;
        int* p_sss = &size_shell_seq;
        // time_1 = MPI_Wtime();
        int** shell_seq = GrahamSequential(t_seq4.data(), size, p_sss);
        // time_2 = MPI_Wtime();
        // std::cout << "Time spent on GrahamSequential execution " << (time_2 - time_1) << "\n";
        int control = 1;
        if (size_shell_par != size_shell_seq) {
            control = 0;
        } else {
            for (int i = 0; i < size_shell_par; i++) {
                if ((shell_par[i][0] != shell_seq[i][0]) || (shell_par[i][1] != shell_seq[i][1])) {
                    control = 0;
                    break;
                }
            }
        }
        ASSERT_EQ(control, 1);
    } else {
        int* ptr = nullptr;
        int** ptr_p = &ptr;
        int size_shell_par;
        int* p_ssp = &size_shell_par;
        shell_par = GrahamParallel(ptr_p, size, p_ssp);
    }
}

TEST(grahan_scan_test, correct_finding_of_a_convex_hull_on_a_set_of_800_points) {
    int size = 800;
    std::vector<std::vector<int>> ps;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int** shell_par;
    if (proc_rank == 0) {
        ps = GetPoints(size, 600, 350, 330);

        std::vector<int*> t_seq5(size);
        for (int i = 0; i < size; i++)
            t_seq5[i] = ps[i].data();
        std::vector<int*> t_par5(size);
        for (int i = 0; i < size; i++)
            t_par5[i] = ps[i].data();

        int size_shell_par;
        int* p_ssp = &size_shell_par;
        // double time_1 = MPI_Wtime();
        shell_par = GrahamParallel(t_par5.data(), size, p_ssp);
        // double time_2 = MPI_Wtime();
        // std::cout << "Time spent on GrahamParallel execution: " << (time_2 - time_1) << "\n";

        int size_shell_seq;
        int* p_sss = &size_shell_seq;
        // time_1 = MPI_Wtime();
        int** shell_seq = GrahamSequential(t_seq5.data(), size, p_sss);
        // time_2 = MPI_Wtime();
        // std::cout << "Time spent on GrahamSequential execution " << (time_2 - time_1) << "\n";
        int control = 1;
        if (size_shell_par != size_shell_seq) {
            control = 0;
        } else {
            for (int i = 0; i < size_shell_par; i++) {
                if ((shell_par[i][0] != shell_seq[i][0]) || (shell_par[i][1] != shell_seq[i][1])) {
                    control = 0;
                    break;
                }
            }
        }
        ASSERT_EQ(control, 1);
    } else {
        int* ptr = nullptr;
        int** ptr_p = &ptr;
        int size_shell_par;
        int* p_ssp = &size_shell_par;
        shell_par = GrahamParallel(ptr_p, size, p_ssp);
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
