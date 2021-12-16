// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK_1_NESTEROV_A_TEST_TASK_1_OPS_MPI_H_
#define MODULES_TASK_1_NESTEROV_A_TEST_TASK_1_OPS_MPI_H_

#include <vector>
#include <string>
/*
std::vector<int> getRandomVector(int  sz);
int getParallelOperations(std::vector<int> global_vec,
                          int count_size_vector, const std::string& ops);
int getSequentialOperations(std::vector<int> vec, const std::string& ops);
*/

double integralMonteCarlo(double a, double b, double h, int n, double(*f)(double));

double integralParallel(double a, double b, double h, int n, double(*f)(double));

double f1(double x);

double f2(double x);

double f3(double x);

double f4(double x);

double f5(double x);

#endif  // MODULES_TASK_1_NESTEROV_A_TEST_TASK_1_OPS_MPI_H_
