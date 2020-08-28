#ifndef SSA
#define SSA 1

#include <stdint.h>

typedef unsigned int UINT;

/* compute the binomial coefficient (n, r) */
UINT nCr(UINT n, UINT r);
/* compute number of ways reactants can combine, given molecule counts x */
UINT h(UINT *r, UINT *x, UINT n);
/* print the current state (x at time t) of the system with n species */
void printstate(double t, UINT* x, UINT n);
/* update the molecule counts, x, according to the stoichiometry r and p */
void doreaction(UINT *r, UINT *p, UINT *x, UINT n);


/* simulate the system defined by R and P with rates k and initial
state x, until time T, using Gillespie's direct method */
void ssa_dm(UINT **R, UINT **P, UINT n, UINT m, double *k, UINT *x, double T);

/* simulate the system defined by R and P with rates k and initial
state x, until time T, using Gillespie's direct method */
void ssa_dmd(UINT **R, UINT **P, UINT**I, UINT n, UINT m, double *k, UINT *x, double T);

/* simulate the system defined by R and P with rates k and initial
state x, until time T, using Gibson and Bruck's next reaction method */
void ssa_nrm(UINT **R, UINT **P, UINT n, UINT m, double *k, UINT *x, double T);

/* simulate the system defined by R and P with rates k and initial
state x, until time T, using Gibson and Bruck's next reaction method
with the addition of a delayed reaction stack */
void ssa_nrmd(UINT **R, UINT **P, UINT **I, UINT n, UINT m, double *k, UINT *steps, UINT *x, double T);


#endif
