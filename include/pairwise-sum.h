#ifndef H_0BM512_PAIRWISE_SUM_H
#define H_0BM512_PAIRWISE_SUM_H 1

#include <stdlib.h>

//typedef double Real_t;

#define Real_t double

#ifdef __cplusplus
extern "C" {
#endif

struct PairwiseSummator {
    Real_t * values;  // ptr to array
    size_t n;  // overall number of elements in the array
    size_t current;  // index of element to be added (one after the last)
};

void pairwise_summator_init(struct PairwiseSummator * s, size_t n);

void pairwise_summator_destroy(struct PairwiseSummator * s);

void pairwise_summator_add(struct PairwiseSummator * s, Real_t v);

Real_t pairwise_summator_get(const struct PairwiseSummator * s);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif /* H_0BM512_PAIRWISE_SUM_H */

