#ifndef _ANFIS_H
#define _ANFIS_H

#include <math.h>

static double fisTriangleMf(double x, double *param);
static double fisGaussianMf(double x, double *param);
double evalfis(double dtdata);

#endif
