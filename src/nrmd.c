/* Implementation for next reaction method with delay stack */

#include <math.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "ssa.h"
#include "pq.h"
#include "heap.h"
#include "dg.h"


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
/* the delay times of all the reactions */
static UINT *STEPS;
/* the simulation time */
static double TIME;
/* the reactant matrix of the system */
static UINT **REACTS;
/* the product matrix of the system */
static UINT **PRODS;
/* the matrix initiations of delayed reactions */
static UINT **INITS;
/* the current state of the system */
static UINT *X;
/* priority queue for the reaction times */
static PQ *Q;
/* priority queue for delayed reactions */
static PQ *STACK;


static void update_reaction(REACTION *r)
{
	double newp = h(REACTS[r->number], X, N) * RATES[r->number];
	double oldp = PROPS[r->number];

	if (newp > 0.0) {
		if (oldp > 0.0) {
			pq_update(Q, r, (oldp / newp) * (r->tau - TIME) + TIME);
		} else {
			r->tau = gsl_ran_exponential(SSARNG, 1 / newp) + TIME;
			pq_insert(Q, r);
		}
	} else if (oldp > 0.0) {
		pq_delete(Q, r);
	}

	PROPS[r->number] = newp;
}


static void step()
{
	REACTION *r = pq_min(Q);
        if (!pq_isempty(STACK) && pq_min(STACK)->tau < r->tau)
                r = pq_min(STACK);
        
        doreaction(REACTS[r->number], PRODS[r->number], X, N);
        TIME = r->tau;

        if (STEPS[r->number] > 1)
                pq_delete(Q, r);
        else {
                PROPS[r->number] = h(REACTS[r->number], X, N) * RATES[r->number];
                if (PROPS[r->number] > 0.0)
                        pq_update(Q, r, gsl_ran_exponential(SSARNG, 1 / PROPS[r->number]) + TIME);
                else
                        pq_delete(Q, r);
        }
	
	LLIST *head = r->affects;
	while (head != NULL) {
		REACTION *update = (REACTION *) head->data;
		update_reaction(update);
		head = head->next;
	}

        head = r->creates;
        while (head != NULL) {
                REACTION *template = (REACTION *) head->data;
                REACTION *newone = reaction_make(template->number);
                newone->tau = gsl_ran_gamma(SSARNG, STEPS[template->number], 1 / RATES[template->number]);
                newone->affects = template->affects;
                pq_insert(STACK, newone);
        }
}


static REACTION **setup_reactions()
{
	REACTION **reactions = malloc(sizeof(REACTION*) * M);
	char **adjmat = adjacencymatrix(REACTS, PRODS, N, M);
	
	UINT i, j;
	for (i = 0; i < M; i++) {
		REACTION *reaction = reaction_make(i);
		reactions[i] = reaction;
	}
	
	for (i = 0; i < M; i++) {
		REACTION *r = reactions[i];
		for (j = 0; j < M; j++) {
			if (i != j && adjmat[i][j] != 0)
				r->affects = llist_push(r->affects, reactions[j]);
                        if (INITS[i][j] != 0)
                                r->creates = llist_push(r->creates, reactions[j]);
                }

        }

        for (i = 0; i < M; i++)
		free(adjmat[i]);
	
	free(adjmat);
	return reactions;
}


void ssa_nrmd(UINT **R, UINT **P, UINT **I, UINT n, UINT m, double *k, UINT *steps, UINT *x, double T)
{
	gsl_rng_env_setup();
	SSARNGT = gsl_rng_default;
	SSARNG = gsl_rng_alloc(SSARNGT);
	gsl_rng_set(SSARNG, time(NULL));
        
        REACTS = R;
        PRODS = P;
        INITS = I;

        N = n;
        M = m;
        RATES = k;
        STEPS = steps;
        X = x;
        TIME = 0.0;
        
        PROPS = malloc(sizeof(double) * M);
        Q = pq_make();
        STACK = pq_make();

	REACTION **reactions = setup_reactions();
	
	UINT i;
	for (i = 0; i < M; i++) {
		REACTION *r = reactions[i];
                if (STEPS[i] > 1)
                        PROPS[i] = h(REACTS[i], X, N) * RATES[i];
                else
                        PROPS[i] = 0.0;

                if (PROPS[i] > 0.0) {
                        r->tau = gsl_ran_exponential(SSARNG, 1 / PROPS[i]);
                        pq_insert(Q, r);
                }
	}

	printstate(TIME, X, N);
	
	while(!pq_isempty(Q) && pq_min(Q)->tau < T) {
                step();
		printstate(TIME, X , N);
        }

	pq_free(Q);
	gsl_rng_free(SSARNG);
}
