// Copyright 2021 Zotov Maksim
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>
#include "../../../modules/task_3/zotov_m_sparse_matrix_multiplication_ccs/sparse_matrix_multiplication_ccs.h"

Matrix getRandomMatrix(int sz) {
    Matrix matrix;
    matrix.size = sz;
    matrix.value.resize(0);
    matrix.row.resize(0);
    matrix.column.resize(0);
    int row_size = 0, value_size = 0 ;
    int ProcRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        std::random_device dev;
        std::mt19937 gen(dev());
        std::uniform_real_distribution<> uid(0, 50);

        for (int i = 0; i < sz; i++) {
            matrix.column.push_back(matrix.row.size());
            for (int j = 0; j < sz; j++) {
                int uid_gen = uid(gen);
                if (uid_gen <= 20) {
                    matrix.value.push_back(uid_gen);
                    matrix.row.push_back(j);
                }
            }
        }
        matrix.column.push_back(matrix.row.size());
        row_size = matrix.row.size();
        value_size = matrix.value.size();
    }

    MPI_Bcast(&row_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&value_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (ProcRank > 0) {
        matrix.size = sz;
        matrix.value.resize(value_size);
        matrix.row.resize(row_size);
        matrix.column.resize(sz + 1);
    }
    MPI_Bcast(matrix.value.data(), value_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(matrix.row.data(), row_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(matrix.column.data(), sz + 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&matrix.size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    return matrix;
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

Matrix parallelTransPosition(Matrix M) {
    std::vector<double> valueT;
    std::vector<int>rowT;
    int size = M.size;
    std::vector<double> local_data(size * size);
    std::vector<double> global_data(size * size);
    int ProcRank, ProcNum;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    Matrix C;
    C.size = M.size;
    C.value.resize(M.value.size());
    C.row.resize(M.row.size());
    C.column.resize(M.column.size());
    rowT.resize(C.row.size());
    valueT.resize(C.value.size());

    int index = 0;
    for (int i = ProcRank; i < size; i += ProcNum) {
        int j = 0;
        while (j < M.row.size()) {
            if (i == M.row[j]) {
                index = 0;
                while (M.column[index + 1] <= j) {
                    index++;
                }
                local_data[i * size + index] = M.value[j];
            }
            j++;
        }

    }

    MPI_Reduce(local_data.data(), global_data.data(), size * size, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
 
    if (ProcRank == 0) {
        int sum = 0;
        for (int i = 0; i < size; i++) {
            C.column[i] = sum;
            for (int j = 0; j < size; j++) {
                if (global_data[i * size + j] != 0) {
                    sum++;
                }
            }
        }
        C.column[size] = sum;
    }

    MPI_Bcast(C.column.data(), C.column.size(), MPI_INT, 0, MPI_COMM_WORLD);
    int k, as, bs;
    for (int i = ProcRank; i < size; i += ProcNum) {
        as = C.column[i];
        bs = C.column[i + 1];
        k = 0;
        while (bs - as > 0) {
            while ( k < size) {
                if (local_data[i * size + k] != 0) {
                    valueT[as] = local_data[i * size + k];
                    rowT[as] = k;
                    as++;
                }
                
                k++;
            }
        }
    }
    MPI_Allreduce(valueT.data(), C.value.data(), valueT.size(), MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(rowT.data(), C.row.data(), rowT.size(), MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    return C;
}

std::vector<double> parallelMultiplication(Matrix A, Matrix B) {
    Matrix C;
    C.size = A.size;
    C.column.resize(A.column.size());

    std::vector<double> valueT;
    std::vector<int>rowT;

    int size = C.size;
    int ProcRank, ProcNum;

    int as, af, bs, bf,count = 0, my_count = 0, sum = 0;

    std::vector<double> local_data(size * size);
    std::vector<double> global_data(size * size);

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    A = parallelTransPosition(A);

    for (int i = ProcRank; i < size; i += ProcNum) {
        for (int j = 0; j < size; j++) {
            as = A.column[j], af = A.column[j + 1];
            bs = B.column[i], bf = B.column[i + 1];
            while (as < af && bs < bf) {
                if (A.row[as] < B.row[bs]) {
                    as++;
                } else if (A.row[as] > B.row[bs]) {
                    bs++;
                } else if (A.row[as] == B.row[bs]) {
                    sum += A.value[as] * B.value[bs];
                    as++;
                    bs++;
                }
            }
            if (sum != 0) {
                local_data[i * size + j] = sum;
                my_count++;
                sum = 0;
            }
        }
    }
    MPI_Reduce(local_data.data(), global_data.data(), size * size, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Allreduce(&my_count, &count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    return (global_data);
}

std::vector<double> multiplication(Matrix A, Matrix B) {
    Matrix C;
    C.size = A.size;
    A = transPosition(A);
    C.value.resize(0);
    C.row.resize(0);
    C.column.resize(0);

    int as, af, bs, bf, sum = 0;
    std::vector<double> data(C.size * C.size);

    for (int i = 0; i < A.size; i ++) {
        for (int j = 0; j < A.size; j++) {
            as = A.column[j], af = A.column[j + 1];
            bs = B.column[i], bf = B.column[i + 1];
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
                data[i * A.size + j] = sum;

                sum = 0;
            }
        }
    }
    return data;
}
