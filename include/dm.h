#ifndef DIRECTM
#define DIRECTM 1

#include <stdlib.h>

#include "ssa.h"

#ifndef RANDDBL
#define RANDDBL (rand() / (double) RAND_MAX)
#endif


void ssa_dm(UINT **R,
	    UINT **P,
	    UINT n,
	    UINT m,
	    double *k,
	    UINT *x,
	    double T);

#endif
