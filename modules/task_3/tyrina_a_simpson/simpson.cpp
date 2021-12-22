// Copyright 2021 Tyrina Anastasia
#include "../../../modules/task_3/tyrina_a_simpson/simpson.h"

#include <mpi.h>

#include <cmath>

double getSequentialSimpson(function<double(vector<double>)> func,
                            vector<pair<double, double>> a_b, int n) {
  int integral_size = a_b.size(); // размерность интеграла
  vector<double> h(integral_size); // длина одного отрезка (разбиения)
  int num = 1; // число отрезков

  for (int i = 0; i < integral_size; ++i) {
    h[i] = (a_b[i].second - a_b[i].first) / n; // заполняем массив длин отрезков для каждого интеграла
    num *= n; // число всех отрезков
  }

  double sum = 0.0;
  for (int i = 0; i < num; ++i) { // в цикле до n^размерность
    vector<vector<double>> x(integral_size); // нам надо посчитать параметры по формуле Симпсона для каждого интеграла
    int tmp = i; // для этого делаем переменную равно текущему i
    for (int j = 0; j < integral_size; ++j) { // 6 параметров по формуле для каждого интеграла
      x[j].push_back(a_b[j].first + tmp % n * h[j]); // берем процент n чтобы не выходило за n 0 1 2 0 1 2 0 1 2 ...
      for (int k = 0; k < 4; k++) {
        x[j].push_back(a_b[j].first + tmp % n * h[j] + h[j] / 2);
      }
      x[j].push_back(a_b[j].first + tmp % n * h[j] + h[j]);
      tmp /= n; // для каждого интеграла в конце делим на n 0 0 0 1 1 1 2 2 2
    }
    vector<double> comb;
    for (int i = 0; i < pow(6, integral_size); ++i) { // здесь нам нужно все 6^размерность параметров перебрать друг с другом
      int tmp = i;
      for (int j = 0; j < integral_size; ++j) {
        comb.push_back(x[j][tmp % 6]); // заполняем каждый (x, y, z,..)
        tmp /= 6;
      }
      sum += func(comb); // вычисляем знач. функции от каждой пары (тройки...) и суммируем
      comb.clear();
    }
    x.clear();
  }
  for (int i = 0; i < integral_size; ++i) {
    sum *= h[i] / 6.0; // в конце умножаем на длину отрезка деленную на 6 (по формуле)
  }
  return sum;
}

double getParallelSimpson(function<double(vector<double>)> func,
                          vector<pair<double, double>> a_b, int n) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int integral_size = a_b.size();
  std::vector<double> h(integral_size);

  int global_num;
  if (rank == 0) { // в нулевом процессе
    global_num = 1;
    for (int i = 0; i < integral_size; ++i) { 
      h[i] = (a_b[i].second - a_b[i].first) / n; // считаем длину
      global_num *= n; // считаем общее число отрезков
    }
  }
  MPI_Bcast(&global_num, 1, MPI_INT, 0, MPI_COMM_WORLD); // рассылаем во все остальные
  MPI_Bcast(h.data(), integral_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  int rem = global_num % size; // находим остаток от деления отрезков на процессы
  int delta, start;
  if (rank == 0) {
    delta = global_num / size + rem; // в нулевом процессе в размер его части записываем остаток
    start = 0; // нулевой начинает сначала
  } else {
    delta = global_num / size; // в остальных без остатка
    start = rem + delta * rank; // точка старта для каждого процесса
  }

  double local_sum = 0.0;
  for (int i = start; i < delta + start; ++i) {
    vector<vector<double>> x(integral_size);
    int tmp = i;
    for (int j = 0; j < integral_size; ++j) {
      x[j].push_back(a_b[j].first + tmp % n * h[j]);
      for (int k = 0; k < 4; k++) {
        x[j].push_back(a_b[j].first + tmp % n * h[j] + h[j] / 2);
      }
      x[j].push_back(a_b[j].first + tmp % n * h[j] + h[j]);
      tmp /= n;
    }

    vector<double> comb;
    for (int i = 0; i < pow(6, integral_size); ++i) {
      int temp = i;
      for (int j = 0; j < integral_size; ++j) {
        comb.push_back(x[j][temp % 6]);
        temp /= 6;
      }
      local_sum += func(comb); // в каждом процессе получаем локальную сумму
      comb.clear();
    }
    x.clear();
  }

  double global_sum = 0.0;
  MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, // собираем все локальные суммы в глобальную в нулевом
             MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < integral_size; ++i) {
      global_sum *= h[i] / 6.0; // в самом конце умножаем на h
    }
  }
  return global_sum;
}
