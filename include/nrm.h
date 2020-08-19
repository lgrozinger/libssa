#ifndef NEXTREACTIONM
#define NEXTREACTIONM 1

#include <stdlib.h>
#include "ssa.h"
#include "pq.h"


typedef struct system {
        PQ *pq;
        UINT **R;
        UINT **P;
        UINT **creates;
        UINT **destroys;
        UINT n;
        UINT m;
        UINT *x;
        UINT *steps;
        double *k;
        double *propensities;
} SYSTEM;


void ssa_nrm(UINT **R,
	     UINT **P,
	     UINT n,
	     UINT m,
	     double *k,
	     UINT *x,
	     UINT *steps,
             UINT **creates,
             UINT **destroys,
	     double T);

#endif
