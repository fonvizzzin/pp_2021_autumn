// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK_1_NESTEROV_A_TEST_TASK_1_OPS_MPI_H_
#define MODULES_TASK_1_NESTEROV_A_TEST_TASK_1_OPS_MPI_H_

#include <vector>
#include <string>
#include <iostream>
#include <random>

struct Matrix {

    int size;
    std::vector<double> value;
    std::vector<int>row, column;



};
Matrix getRandomMatrix(int sz);

void printMatrix(Matrix& matrix);
void printCoef(Matrix& A);
Matrix multiplication(Matrix A, Matrix B);



/*
std::vector<int> getRandomVector(int  sz);
int getParallelOperations(std::vector<int> global_vec,
                          int count_size_vector, const std::string& ops);
int getSequentialOperations(std::vector<int> vec, const std::string& ops);
*/
#endif  // MODULES_TASK_1_NESTEROV_A_TEST_TASK_1_OPS_MPI_H_
