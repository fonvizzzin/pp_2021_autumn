// Copyright 2021 Zotov Maksim
#ifndef MODULES_TASK_3_ZOTOV_M_SPARSE_MATRIX_MULTIPLICATION_CCS_SPARSE_MATRIX_MULTIPLICATION_CCS_H_
#define MODULES_TASK_3_ZOTOV_M_SPARSE_MATRIX_MULTIPLICATION_CCS_SPARSE_MATRIX_MULTIPLICATION_CCS_H_

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
std::vector<double> multiplication(Matrix A, Matrix B);
Matrix transPosition(Matrix M);
Matrix parallelTransPosition(Matrix M);
std::vector<double> parallelMultiplication(Matrix A, Matrix B);

#endif  // MODULES_TASK_3_ZOTOV_M_SPARSE_MATRIX_MULTIPLICATION_CCS_SPARSE_MATRIX_MULTIPLICATION_CCS_H_
