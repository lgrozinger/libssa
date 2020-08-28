/* Implementation of Gillespie's direct method */

#include <math.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "ssa.h"


static const gsl_rng_type *SSARNGT;
static gsl_rng *SSARNG;

/* the number of reactions in the system */
static UINT M;
/* the number of species in the system */
static UINT N;
/* the current propensities of the reactions */
static double *PROPS;
/* the rate constants of the reactions */
static double *RATES;
/* the simulation time */
static double TIME;
/* the reactant matrix of the system */
static UINT **REACTS;
/* the product matrix of the system */
static UINT **PRODS;
/* the current state of the system */
static UINT *X;


static UINT nextreaction()
{
	double r1 = gsl_ran_flat(SSARNG, 0.0, 1.0) * PROPS[M - 1];
	UINT i = 0;
	for(i = 0; i < M; i++)
		if(r1 <= PROPS[i])
			return i;
	return -1;
}


static void step()
{
	PROPS[0] = h(REACTS[0], X, N) * RATES[0];
	UINT i;
	for (i = 1; i < M; i++)
		PROPS[i] = h(REACTS[i], X, N) * RATES[i] + PROPS[i - 1];


	if (PROPS[M - 1] > 0.0) {
		TIME += gsl_ran_exponential(SSARNG, 1 / PROPS[M - 1]);
		UINT next = nextreaction();
		doreaction(REACTS[next], PRODS[next], X, N);
	}
}


void ssa_dm(UINT **R, UINT **P, UINT n, UINT m, double *k, UINT *x, double T)
{
	gsl_rng_env_setup();
	SSARNGT = gsl_rng_default;
	SSARNG = gsl_rng_alloc(SSARNGT);
	gsl_rng_set(SSARNG, time(NULL));

        REACTS = R;
        PRODS = P;

        N = n;
        M = m;
        RATES = k;
        X = x;
        TIME = 0.0;
        
        PROPS = malloc(sizeof(double) * M);
        printstate(TIME, X, N);
	
	do {
		step();
		printstate(TIME, X, N);
	} while(TIME < T && PROPS[M - 1] > 0.0);

	free(PROPS);
        gsl_rng_free(SSARNG);
}
