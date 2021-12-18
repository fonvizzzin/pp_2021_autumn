// Copyright 2021 Troegubova Alexandra
#ifndef MODULES_TASK_3_TROEGUBOVA_A_GRAHAM_SCAN_GRAHAM_SCAN_H_
#define MODULES_TASK_3_TROEGUBOVA_A_GRAHAM_SCAN_GRAHAM_SCAN_H_

#include <vector>
#include<stack>

std::vector<std::vector<int>> GetPoints(int count_p, int centre_x, int centre_y, int r);
int Rotate(int* p, int* b, int* c);
int Distance(int* p1, int* p2);
int Compare(const void* p_1, const void* p_2);
int* NextToTop(std::stack<int*> S);
int** GrahamSequential(int** points, int size, int* size_shell);
int** GrahamParallel(int** points, int size, int* size_shell);

#endif  // MODULES_TASK_3_TROEGUBOVA_A_GRAHAM_SCAN_GRAHAM_SCAN_H_





