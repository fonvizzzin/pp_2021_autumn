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
    int row_size, value_size;
    int flag = 0;
    int ProcRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
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


Matrix parallelTransPosition(Matrix M) {
    std::vector<double> valueT;
    std::vector<int>rowT, columnT;
    int size = M.size;
    std::vector<double> local_data(size * size,0);
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
    MPI_Bcast(global_data.data(), size * size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
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
            //as++;
        }
    }
    MPI_Reduce(valueT.data(), C.value.data(), valueT.size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(rowT.data(), C.row.data(), rowT.size(), MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return C;



}

Matrix parallelMultiplication(Matrix A, Matrix B) {
    Matrix C;
    if (A.size == B.size) {
        C.size = A.size;
    }
    A = parallelTransPosition(A);

    MPI_Bcast(A.value.data(), 6, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(A.row.data(), 6, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(A.column.data(), 5, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&A.size, 1, MPI_INT, 0, MPI_COMM_WORLD);


    C.value.resize(0);
    C.row.resize(0);
    C.column.resize(A.column.size());
    int count = 0;

    std::vector<double> valueT;
    std::vector<int>rowT, columnT;

    int size = C.size;
    std::vector<double> local_data(size * size);
    std::vector<double> global_data(size * size);

    int ProcRank, ProcNum;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    int as, af, bs, bf,my_count = 0, sum = 0;

    for (int i = ProcRank; i < size; i += ProcNum) {
        for (int j = 0; j < size; j++) {
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
                local_data[i * size + j] = sum;
                my_count++;
                sum = 0;
            }
        }
    }

    MPI_Reduce(local_data.data(), global_data.data(), size * size, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&my_count, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Bcast(&count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    C.value.resize(count);
    C.row.resize(count);
    valueT.resize(count);
    rowT.resize(count);


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

    int k, AS, BS;
    for (int i = ProcRank; i < size; i += ProcNum) {
        AS = C.column[i];
        BS = C.column[i + 1];
        k = 0;
        while (BS - AS > 0) {
            while (k < size) {
                if (local_data[i * size + k] != 0) {
                    valueT[AS] = local_data[i * size + k];
                    rowT[AS] = k;
                    AS++;
                }

                k++;
            }
            
        }
    }
    MPI_Reduce(valueT.data(), C.value.data(), valueT.size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(rowT.data(), C.row.data(), rowT.size(), MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

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

    int as, af, bs, bf, sum = 0;

    for (int i = 0; i < C.size; i++) {
        C.column.push_back(C.row.size());
        for (int j = 0; j < C.size; j++) {
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
                C.row.push_back(j);
                C.value.push_back(sum);
                sum = 0;
            }
        }
    }
    C.column.push_back(C.row.size());

    //C = transPosition(C);

    return C;
}
