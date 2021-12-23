// Copyright 2018 Nesterov Alexander
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_3/zotov_m_sparse_matrix_multiplication_ccs/sparse_matrix_multiplication_ccs.h"


Matrix getRandomMatrix(int sz) {
    Matrix matrix;
    matrix.size = sz;
    matrix.value.resize(0);
    matrix.row.resize(0);
    matrix.column.resize(0);
    int flag = 0;

    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_real_distribution<> uid(0, 100);

    for (int i = 0; i < sz; i++) {
        matrix.column.push_back(matrix.row.size());
        for (int j = 0; j < sz; j++) {
            int uid_gen = uid(gen);
            if (uid_gen > 70) {
                matrix.value.push_back(uid_gen);
                matrix.row.push_back(j);
                flag = 1;
            }
        }
        if (flag == 0) {
            int uid_gen = uid(gen);
            matrix.value.push_back(uid_gen);
            matrix.row.push_back(i);
        }
        flag = 0;
    }
    matrix.column.push_back(matrix.row.size());

    return matrix;
}

void printMatrix(Matrix& matrix) {
    int count;
    int flag;
    for (size_t i = 0; i < matrix.size; i++) {
        flag = 0;
        std::cout << std::endl;
        for (size_t j = 0; j < matrix.size ; j++) {
            count = 0;
            flag = 0;
            while (count < matrix.column[j + 1] - matrix.column[j]) {
                if (matrix.row[matrix.column[j] + count] == i) {
                    std::cout << "  " << matrix.value[matrix.column[j] + count] << "  ";
                    flag = 1;
                    count++;

                }
                else {
                    count++;
                }
            }
            if (flag == 0) {
                std::cout << "   0   ";
            }
        }
    }
}

Matrix transPosition(Matrix M) {
    std::vector<double> valueT;
    std::vector<int>rowT,columnT;
    int N = M.column.size() - 1;


    Matrix C;
    C.size = M.size;
    C.value.resize(0);
    C.row.resize(0);
    C.column.resize(0);
    int index = 0;
    for (int i = 0; i < N; i++) {
        int j = 0;
        C.column.push_back(C.row.size());
        while (j < M.row.size()) {
            if (i == M.row[j]) {
                index = 0;
                C.value.push_back(M.value[j]);
                while(M.column[index + 1] <= j) {
                    index++;
                }
                C.row.push_back(index);
            }
            j++;
        }

    }
    C.column.push_back(C.row.size());

    return C;

}

void printCoef( Matrix& A) {
    std::cout << std::endl << "value= ";
    for (int i = 0; i < A.value.size(); i++) {
        std::cout << A.value[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl << "row= ";
    for (int i = 0; i < A.row.size(); i++) {
        std::cout << A.row[i] << " ";
    }
    std::cout << std::endl;

    std::cout << std::endl << "column= ";
    for (int i = 0; i < A.column.size(); i++) {
        std::cout << A.column[i] << " ";
    }
    std::cout << std::endl;
}

Matrix multiplication(Matrix A, Matrix B) {
    Matrix C;
    if (A.size == B.size) {
        C.size = A.size;
    }
    A = transPosition(A);
    C.value.resize(0);
    C.row.resize(0);
    C.column.resize(0);

    int countA, countB, as, af, bs, bf, sum = 0;

    for (int i = 0; i < C.size; i++) {
        C.column.push_back(C.row.size());
        for (int j = 0; j < C.size; j++) {
            as = A.column[i], af = A.column[i + 1];
            bs = B.column[j], bf = B.column[j + 1];
            while (as < af && bs < bf) {
                if (A.row[as] < B.row[bs]) {
                    as++;
                }
                else if (A.row[as] > B.row[bs]) {
                    bs++;
                }
                else if (A.row[as] == B.row[bs]) {
                    sum += A.value[as] * B.value[bs];
                    as++;
                    bs++;
                }
            }
            if (sum != 0) {
                C.row.push_back(j);
                C.value.push_back(sum);
                sum = 0;
            }
        }
    }
    C.column.push_back(C.row.size());

    C = transPosition(C);

    return C;
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
}
*/