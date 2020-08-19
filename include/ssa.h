#ifndef SSA
#define SSA 1

#include <stdint.h>

typedef unsigned int UINT;

UINT ssa_nCr(UINT, UINT);

void ssa_printstate(double, UINT*, UINT);

/* mutate `x`, the vector of molecule counts, according to `v`, the
stoichiometry vector of the reaction to be performed. */
void ssa_doreaction(UINT *r, UINT *p, UINT *x, UINT n);

/* compute the number of ways the reactants in `r` can combine, based
on their counts in `x` */
UINT ssa_h(UINT *r, UINT *x, UINT n);

#endif
