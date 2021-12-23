// Copyright 2021 Moskvin Stanislav
#ifndef MODULES_TASK_1_MOSKVIN_S_INTEGRATION_BY_RECTANGLES_INTEGRATION_BY_RECTANGLES_H_
#define MODULES_TASK_1_MOSKVIN_S_INTEGRATION_BY_RECTANGLES_INTEGRATION_BY_RECTANGLES_H_

double sequentialIntegration(double a, double b, int n, double(*Func)(double));
double parallelIntegration(double a, double b, int n, double(*Func)(double));

#endif  // MODULES_TASK_1_MOSKVIN_S_INTEGRATION_BY_RECTANGLES_INTEGRATION_BY_RECTANGLES_H_
