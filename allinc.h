#ifndef ALLINC
#define ALLINC

#include <stdio.h>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <math.h>

#define POP_SIZE 200
#define L_TOURN_N 3 			// do not touch plz
#define MUTATE_POINTS 7			// should not exceed (LENGTH_N/2)-1
#define MUTATE_INDS 77
#define LENGTH_N 30
#define CROSSOVER_TYPE 0		// 0 - uniform template based, 1 - one-point, 2 - fat-kid-cut crossover
#define TERM_CRITERIA 1			// 0 - fitness evaluations limit, 1 - generations limit
#define GENERATIONS 25000
#define FIT_EVAL 20000000

int ev;

struct Individual{
	int sequence[LENGTH_N];
	int fitness;

};

struct Population{
	Individual pop[POP_SIZE];
};

#endif
