#define _USE_MATH_DEFINES 1

#include <stdlib.h>
#include <math.h>

#include "randomlib.h"

double rand_0_1() { return (double)rand() / (double)RAND_MAX; }

int rand_in_range(int lower, int upper) { return (int)(lower + (upper - lower) * rand_0_1()); }

static int box_muller_alt = 0;

double rand_normal_boxmuller_cos()
{
	double u1 = rand_0_1();
	double u2 = rand_0_1();

	return sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
}

double rand_normal_boxmuller_sin()
{
	double u1 = rand_0_1();
	double u2 = rand_0_1();

	return sqrt(-2 * log(u1)) * sin(2 * M_PI * u2);
}

int rand_normal_expect_stddev(int expectation, double stddev) { return (int)(rand_normal_boxmuller() * stddev + expectation); }

double rand_normal_boxmuller()
{
	int alt = box_muller_alt;
	box_muller_alt++;
	return (alt % 2) ? rand_normal_boxmuller_cos() : rand_normal_boxmuller_sin();
}

