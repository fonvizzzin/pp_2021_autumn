// Copyright 2021 Troegubova Alexandra
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <stack>
#include "../../../modules/task_3/troegubova_a_graham_scan/graham_scan.h"

int* p0 = new int[2];

std::vector<std::vector<int>> GetPoints(int count_p, int centre_x, int centre_y, int r) {
    std::mt19937 gen(std::random_device().operator()());
    std::uniform_int_distribution<> range(5, r);
    std::vector<std::vector<int>> ps(count_p);

    int count_r = count_p / 63 + 1;
    int remains = 63 - count_p % 63;
    int j = 0;
    for (double i = 0; i < 6.27; i += 0.1) {
        for (int k = 0; k < count_r; k++) {
            if ((remains) && (k == 0)) {
                remains--;
                continue;
            }
            ps[j].resize(2);
            ps[j][0] = centre_x + static_cast<int>((range(gen)) * cos(i));
            ps[j][1] = centre_y + static_cast<int>((range(gen)) * sin(i));
            j++;
            if (j == count_p) {
                break;
            }
        }
        if (j == count_p) {
            break;
        }
    }
    return ps;
}

int Rotate(int* p, int* b, int* c) {
    int res = (b[0] - p[0]) * (c[1] - b[1]) - (b[1] - p[1]) * (c[0] - b[0]);
    if (res == 0)
        return 0;
    return (res > 0) ? 1 : -1;
}

int Distance(int* p1, int* p2) {
    return (p1[0] - p2[0]) * (p1[0] - p2[0]) + (p1[1] - p2[1]) * (p1[1] - p2[1]);
}

int Compare(const void* p_1, const void* p_2) {
    int* p1 = *reinterpret_cast<int**>(const_cast<void*>(p_1));
    int* p2 = *reinterpret_cast<int**>(const_cast<void*>(p_2));
    int val = Rotate(p0, p1, p2);
    if (val == 0)
        return (Distance(p0, p2) >= Distance(p0, p1)) ? 1 : -1;
    return val;
}

int* NextToTop(std::stack<int*> S) {
    int* p = S.top();
    S.pop();
    int* res = S.top();
    S.push(p);
    return res;
}

int** GrahamSequential(int** points, int size, int* size_shell) {
    int x_min = points[0][0], min_p = 0;
    for (int i = 1; i < size; i++) {
        if ((points[i][0] < x_min) || (points[i][0] == x_min && points[i][1] < points[min_p][1]))
            x_min = points[i][0], min_p = i;
    }

    int* tmp = points[0];
    points[0] = points[min_p];
    points[min_p] = tmp;
    p0 = points[0];

    qsort(points + 1, (size - 1), 8, Compare);

    std::stack<int*> S;
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);

    for (int i = 3; i < size; i++) {
        while (Rotate(NextToTop(S), S.top(), points[i]) != -1)
            S.pop();
        S.push(points[i]);
    }
    int stack_size = S.size();
    *size_shell = stack_size;
    int** shell = new int* [stack_size];
    for (int i = 0; i < stack_size; ++i)
        shell[i] = S.top(), S.pop();

    return shell;
}

int** GrahamParallel(int** points, int size, int* size_shell) {
    MPI_Barrier(MPI_COMM_WORLD);
    int proc_count, proc_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    int proc = size / 3 > proc_count ? proc_count : size / 3;
    int whole = size / proc;
    int remains = size % proc;
    int num_elem_block = whole + (proc_rank < remains ? 1 : 0);
    int* rcount = new int[num_elem_block * 2];
    int* sendcounts = nullptr;
    int* displs = nullptr;
    int* ps = nullptr;
    if (proc_rank == 0) {
        ps = new int[size * 2];
        for (int i = 0; i < size; i++) {
            ps[i * 2] = points[i][0];
            ps[i * 2 + 1] = points[i][1];
        }
        sendcounts = new int[proc_count];
        displs = new int[proc_count];
        for (int i = 0; i < proc_count; i++)
            sendcounts[i] = whole * 2;
        for (int i = 0; i < remains; i++)
            sendcounts[i] += 2;
        displs[0] = 0;
        for (int i = 1; i < proc_count; i++)
            displs[i] = displs[i - 1] + sendcounts[i - 1];
    }

    MPI_Scatterv(ps, sendcounts, displs, MPI_INT, rcount, 2 * num_elem_block, MPI_INT, 0, MPI_COMM_WORLD);

    int** points_2 = nullptr;
    if (proc_rank < proc) {
        points_2 = new int* [num_elem_block];
        for (int i = 0; i < num_elem_block; i++) {
            points_2[i] = new int[2];
            points_2[i][0] = rcount[i * 2];
            points_2[i][1] = rcount[i * 2 + 1];
        }
    }
    int shell_local_size = 0;
    int* p_sls = &shell_local_size;
    int** shell_local_2d = nullptr;
    int* shell_global_1d = nullptr;
    int size_global_shell = 0;
    if (proc_rank < proc) {
        shell_local_2d = GrahamSequential(points_2, num_elem_block, p_sls);

        if (proc_rank == 0) {
            size_global_shell = shell_local_size;
            int buf;
            sendcounts[0] = shell_local_size * 2;
            for (int i = 1; i < proc; i++) {
                MPI_Recv(&buf, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sendcounts[i] = buf * 2; size_global_shell += buf, displs[i] = displs[i - 1] + sendcounts[i - 1];
            }
            for (int i = proc; i < proc_count; i++) {
                sendcounts[i] = 0;
                displs[i] = displs[i - 1] + sendcounts[i - 1];
            }
            shell_global_1d = new int[size_global_shell * 2];
        } else {
            MPI_Send(&shell_local_size, 1, MPI_INT, 0, proc_rank, MPI_COMM_WORLD);
        }
    }

    int* shell_local_1d = new int[shell_local_size * 2];
    for (int i = 0; i < shell_local_size; i++) {
        shell_local_1d[i * 2] = shell_local_2d[i][0];
        shell_local_1d[i * 2 + 1] = shell_local_2d[i][1];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gatherv(shell_local_1d, shell_local_size * 2, MPI_INT, shell_global_1d,
        sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    int** res = nullptr;
    if (proc_rank == 0) {
        int** shell_global_2d = new int* [size_global_shell];
        for (int i = 0; i < size_global_shell; i++) {
            shell_global_2d[i] = new int[2];
            shell_global_2d[i][0] = shell_global_1d[i * 2];
            shell_global_2d[i][1] = shell_global_1d[i * 2 + 1];
        }
        if (proc_count) {
            res = GrahamSequential(shell_global_2d, size_global_shell, size_shell);
        } else {
            *size_shell = size_global_shell;
            return shell_global_2d;
        }
    }
    return res;
}






