#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define DEBUG_OUTPUT

//typedef double Real_t;

#define Real_t double

struct PairwiseSummator {
    Real_t * values;  // ptr to array
    size_t n;  // overall number of elements in the array
    size_t current;  // index of element to be added (one after the last)
};

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
        #if 0
        Real_t * newValues = (Real_t*) malloc((s->n + 1)*sizeof(Real_t));
        memcpy(newValues, s->values, s->n*sizeof(Real_t));
        s->n += 1;
        #else
        Real_t * newValues = (Real_t*) malloc((2*s->n)*sizeof(Real_t));
        memcpy(newValues, s->values, s->n*sizeof(Real_t));
        s->n *= 2;
        #endif
        free(s->values);
        s->values = newValues;
    }
    s->values[s->current] = v;
    ++(s->current);
}

Real_t pairwise_summator_get(struct PairwiseSummator * s) {
    unsigned int nIters = ceil(log2(s->n));
    unsigned long n = s->current;
    for(unsigned int nIt = 0; nIt < nIters; ++nIt) {
        printf("it#%u n=%lu\n", nIt, n);
        n = floor(n/2.);
        for(unsigned long i = 0; i < n; ++i) {
            s->values[i] += s->values[n - i - 1];
        }
    }
    return s->values[0];
}

int main(int argc, char *argv[]) {
    struct PairwiseSummator S;
    pairwise_summator_init(&S, 10);
    size_t N = 131;

    Real_t s = 0.0;
    for(long unsigned int i = 0; i < N; ++i) {
        Real_t v = 1./N;
        pairwise_summator_add(&S, v);
        //Real_t u = rand()/(Real_t) RAND_MAX;
        //s += u;
        s += v;
    }
    printf("  Direct sum: %e\n", s);
    printf("Pairwise sum: %e\n", pairwise_summator_get(&S));

    return 0;
}
