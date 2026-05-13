#include "pairwise-sum.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEBUG_OUTPUT

void pairwise_summator_init(struct PairwiseSummator * s, size_t n) {
    s->values = (Real_t *) malloc(sizeof(Real_t)*n);
    s->n = n;
    s->current = 0;
};

void pairwise_summator_destroy(struct PairwiseSummator * s) {
    free(s->values);
}

void pairwise_summator_add(struct PairwiseSummator * s, Real_t v) {
    if(s->current >= s->n) {
        Real_t * newValues = (Real_t*) malloc((2*s->n)*sizeof(Real_t));
        memcpy(newValues, s->values, s->n*sizeof(Real_t));
        s->n *= 2;
        free(s->values);
        s->values = newValues;
    }
    s->values[s->current] = v;
    ++(s->current);
}

Real_t pairwise_summator_get(const struct PairwiseSummator * s) {
    if(0 == s->current) return 0.;
    unsigned long n = s->n;
    while(n>1) {
        int half = n/2;
        for(unsigned long i = 0; i < half; ++i) {
            s->values[i] += s->values[n - i - 1];
        }
        n -= half;
    }
    return s->values[0];
}

