#include <stdlib.h>
#include <stdio.h>
#include "ssa.h"
#include "dg.h"

char** adjacencymatrix(UINT **R, UINT **P, UINT n, UINT m)
{
	char** adjmat = malloc(sizeof(char *) * m);
	UINT i, j, k;
	for (i = 0; i < m; i++)
		adjmat[i] = malloc(sizeof(char) * m);

	for (i = 0; i < m; i++)
		for (j = 0; j < m; j++)
			adjmat[i][j] = 0;

	for(j = 0; j < m; j++)
		for(i = 0; i < m; i++)
			for (k = 0; k < n; k++)
				adjmat[i][j] |= (R[i][k] || P[i][k]) && R[j][k];
	
	return adjmat;
}
