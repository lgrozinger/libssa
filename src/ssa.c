#include <stdio.h>
#include <stdint.h>

#include "ssa.h"


UINT nCr(UINT n, UINT r)
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

UINT h(UINT *r, UINT *x, UINT n)
{
        UINT result = 1;
        UINT i;
        for (i = 0; i < n; i++)
                result *= nCr(x[i], r[i]);
        return result;
}


void printstate(double t, UINT *x, UINT n)
{
	printf("%f", t);
	UINT i;
	for(i = 0; i < n; i++)
		printf(" %d", x[i]);
	printf("\n");
}


void doreaction(UINT *r, UINT *p, UINT *x, UINT n)
{
	UINT i;
	for (i = 0; i < n; i++)
		x[i] = x[i] + p[i] - r[i];
}

