// Copyright 2018 Nesterov Alexander
#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <random>
#include <algorithm>
#include <cmath>
#include "../../../modules/task_1/zotov_m_integration_Monte_Carlo_method/integration_Monte_Carlo_method.h"


double f1(double x)
{
    return pow(x, 2);
}
double f2(double x)
{
    return x/2 + 1;
}
double f3(double x) {
    return cos(x);
}
double f4(double x) {
    return sin(x) * sin(x);
}
double f5(double x) {
    return pow(x, 2)/3 + 1;
}

double integralMonteCarlo(double a, double b, double h, int n, double(*f)(double))
{
    srand(time(NULL));
    int inCount = 0;
    double res;
    double x;
    double y;

    //double x,y;
    for (int i = 0; i < n; i++) {
        x = rand() / static_cast<double>(RAND_MAX) * (b - a) + a;
        y = rand() / static_cast<double>(RAND_MAX) * h;
        if (y < f(x)) {
            inCount++;
        }
    }
    res = (b - a) * h * static_cast<double>(inCount) / n;
    return res;
}

double integralParallel(double a, double b, double h, int n, double(*f)(double))
{
    srand(time(NULL));
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int inCount = 0, count = 0;
    double res;
    double x;
    double y;

    for (int i = 0; i < n; i += ProcNum) {
        x = rand() / static_cast<double>(RAND_MAX) * (b - a) + a;
        y = rand() / static_cast<double>(RAND_MAX) * h;
        if (y < f(x)) {
            inCount++;
        }

    }
    //cout <<"procrank ="<< ProcRank << endl;
    MPI_Reduce(&inCount, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    //std::cout << "myProcrank= " << ProcRank << " myCount= " << count << endl;
    return res = (b - a) * h * static_cast<double>(count) / n;

}
