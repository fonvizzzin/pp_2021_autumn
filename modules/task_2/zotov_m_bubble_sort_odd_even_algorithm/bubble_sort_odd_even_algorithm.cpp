// Copyright 2018 Nesterov Alexander
#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/zotov_m_bubble_sort_odd_even_algorithm/bubble_sort_odd_even_algorithm.h"


std::vector<int> BubbleSortSequential(std::vector<int> pData) {
    size_t size = pData.size();
    int upper_bound;
    if (size % 2 == 0) {
        upper_bound = size / 2 - 1;
    }
    else {
        upper_bound = size / 2;
    }

    for (size_t i = 0; i < size; i++) {
        if (i % 2 == 0) {
            for (size_t j = 0; j < size / 2; j++) {
                if (pData[2 * j] > pData[2 * j + 1])

                    std::swap(pData[2 * j], pData[2 * j + 1]);
            }
        }
        else {
            for (size_t j = 0; j < upper_bound; j++) {
                if (pData[2 * j + 1] > pData[2 * j + 2])
                    std::swap(pData[2 * j + 1], pData[2 * j + 2]);
            }
        }
    }

    return pData;
}

std::vector<int> getRandomVector(int size) {
    std::vector<int> tmp;
    for (size_t i = 0; i < size; i++) {
        tmp.push_back(rand());
    }
    return tmp;
}

void mergAndSort(std::vector<int>& firstArr, std::vector<int>& secondArr) {
    std::vector<int> tmp;
    tmp.insert(tmp.end(), firstArr.begin(), firstArr.end());
    tmp.insert(tmp.end(), secondArr.begin(), secondArr.end());
    sort(tmp.begin(), tmp.end());
    for (size_t i = 0; i < tmp.size(); i++) {
        if (i < firstArr.size()) {
            firstArr[i] = tmp[i];
        }
        else {
            secondArr[i - firstArr.size()] = tmp[i];
        }
    }
}


std::vector<int> BubbleSortParallel(std::vector<int> pData) {
    int ProcRank, ProcNum;
    size_t data_size = 0, my_size;
    int remain, sum;
    std::vector<int> my_data, right_neighbor_data;
    double t1, t2;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        data_size = pData.size();
    }

    MPI_Bcast(&data_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    my_size = data_size / ProcNum;
    remain = data_size % ProcNum;

    std::vector <int> p(ProcNum);
    std::vector <int> d;

    t1 = MPI_Wtime();

    if (ProcRank == 0) {
        sum = 0;
        for (size_t i = 0; i < ProcNum; i++) {
            p[i] = my_size;
            if (remain > 0) {
                p[i]++;
                remain--;
            }

            d.push_back(sum);
            sum += p[i];
        }
    }

    MPI_Bcast(p.data(), ProcNum, MPI_INT, 0, MPI_COMM_WORLD);

    my_data.resize(p[ProcRank]);

    if (ProcRank != ProcNum - 1) {
        right_neighbor_data.resize(p[ProcRank + 1]);
    }


    MPI_Scatterv(pData.data(), p.data(), d.data(), MPI_INT, my_data.data(), p[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);

    my_data = BubbleSortSequential(my_data);

    for (int i = 0; i < ProcNum; i++) {
        if (i % 2 == 1) {
            if ((ProcRank % 2 == 1 && ProcRank < ProcNum - 1)) {
                MPI_Recv(right_neighbor_data.data(), p[ProcRank + 1], MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                mergAndSort(my_data, right_neighbor_data);
                MPI_Send(right_neighbor_data.data(), p[ProcRank + 1], MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD);
            }
            else if (ProcRank > 0 - 1 && ProcRank % 2 == 0) {
                MPI_Send(my_data.data(), p[ProcRank], MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
                MPI_Recv(my_data.data(), p[ProcRank], MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            }
        }
        else if (i % 2 == 0) {
            if (ProcRank % 2 == 0 && ProcRank < ProcNum - 1) {
                MPI_Recv(right_neighbor_data.data(), p[ProcRank + 1], MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                mergAndSort(my_data, right_neighbor_data);
                MPI_Send(right_neighbor_data.data(), p[ProcRank + 1], MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD);
            }
            else if (ProcRank > 0 && ProcRank % 2 == 1) {
                MPI_Send(my_data.data(), p[ProcRank], MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
                MPI_Recv(my_data.data(), p[ProcRank], MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Gatherv(my_data.data(), p[ProcRank], MPI_INT, pData.data(), p.data(), d.data(), MPI_INT, 0, MPI_COMM_WORLD);

    t2 = MPI_Wtime();

    return pData;

}