// Copyright 2018 Nesterov Alexander
#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <random>
#include <algorithm>
#include <cmath>
#include "../../../modules/task_1/zotov_m_integration_Monte_Carlo_method/integration_Monte_Carlo_method.h"


double f(double x)
{
    return pow(x, 2);
}
double integralMonteCarlo(double a, double b, double c, double d, int n)
{
    srand(time(NULL));
    int inCount = 0;
    double res = 0.0;

    //double x,y;
    for (int i = 0; i < n; i++) {
        double x = rand() / static_cast<double>(RAND_MAX) * (b - a) + a;
        double y = rand() / static_cast<double>(RAND_MAX) * (d - c) + c;
        if (y < f(x)) {
            inCount++;
        }
    }
    res = (b - a) * (d - c) * double(inCount) / double(n);
    //cout << "xMax = " << xMax << " yMax = " << yMax << endl;
    return res;
}

double integralParallel(double a, double b, double c, double d, int n)
{
    srand(time(NULL));
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int inCount = 0, count = 0;
    double res = 0.;

    for (int i = 0; i < n; i += ProcNum) {
        double x = rand() / static_cast<double>(RAND_MAX) * (b - a) + a;
        double y = rand() / static_cast<double>(RAND_MAX) * (d - c) + c;
        if (y < f(x)) {
            inCount++;
        }

    }
    //cout <<"procrank ="<< ProcRank << endl;
    MPI_Reduce(&inCount, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    //std::cout << "myProcrank= " << ProcRank << " myCount= " << count << endl;
    return res = (b - a) * (d - c) * double(count) / n;

}

/*
std::vector<int> getRandomVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(sz);
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

int getSequentialOperations(std::vector<int> vec, const std::string& ops) {
    const int  sz = vec.size();
    int reduction_elem = 0;
    if (ops == "+") {
        for (int  i = 0; i < sz; i++) {
            reduction_elem += vec[i];
        }
    } else if (ops == "-") {
        for (int  i = 0; i < sz; i++) {
            reduction_elem -= vec[i];
        }
    } else if (ops == "max") {
        reduction_elem = vec[0];
        for (int  i = 1; i < sz; i++) {
            reduction_elem = std::max(reduction_elem, vec[i]);
        }
    }
    return reduction_elem;
}

int getParallelOperations(std::vector<int> global_vec,
                          int count_size_vector, const std::string& ops) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = count_size_vector / size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(global_vec.data() + proc * delta, delta,
                        MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec(delta);
    if (rank == 0) {
        local_vec = std::vector<int>(global_vec.begin(),
                                     global_vec.begin() + delta);
    } else {
        MPI_Status status;
        MPI_Recv(local_vec.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int global_sum = 0;
    int local_sum = getSequentialOperations(local_vec, ops);
    MPI_Op op_code = MPI_OP_NULL;
    if (ops == "+" || ops == "-") { op_code = MPI_SUM; }
    if (ops == "max") { op_code = MPI_MAX; }
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, op_code, 0, MPI_COMM_WORLD);
    return global_sum;
}*/
