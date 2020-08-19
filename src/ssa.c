#include <stdio.h>
#include <stdint.h>

#include "ssa.h"


UINT ssa_nCr(UINT n, UINT r)
{
        if (r > n)
                return 0;
        
        r = r < n - r ? r : n - r;
        UINT i;
        UINT result = 1;
        for (i = 1; i <= r; i++)
                result = result / i * (n + 1 - i) + result % i * (n + 1 - i) / i;

        return result;
}


void ssa_printstate(double t, UINT *x, UINT n)
{
	printf("%f", t);
	UINT i;
	for(i = 0; i < n; i++)
		printf(" %d", x[i]);
	printf("\n");
}

/* mutate `x`, the vector of molecule counts, according to `v`, the
stoichiometry vector of the reaction to be performed. */
void ssa_doreaction(UINT *r, UINT *p, UINT *x, UINT n)
{
	UINT i;
	for (i = 0; i < n; i++)
		x[i] = x[i] + p[i] - r[i];
}


/* compute the number of ways the reactants in `r` can combine, based
on their counts in `x` */
UINT ssa_h(UINT *r, UINT *x, UINT n)
{
	UINT c = 1;
	UINT i, j;
	for(i = 0; i < n; i++)
		for(j = 0; j < r[i]; j++)
			c *= x[i] - j;
	return c;
}

