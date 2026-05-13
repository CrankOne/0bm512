#include "sum.h"
#include "pairwise-sum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#if 0

#include "histogram.h"

float generate_uniform_scaled() {
    return rand() / (float) RAND_MAX;
}

float generate_gaussian_scaled() {
    // ...
}

int main() {
    struct Histogram1D a;

    hist1d_init(&a, 40, -15, 20);

    hist1d_reset(&a);

    hist1d_generate_and_fill(&a, 5000, generate_uniform_scaled);

    //for(int i = 0; i < 5000; ++i) {
    //    // get random value in definition range
    //    float v = a.min + (rand() / (float) RAND_MAX) * (a.max - a.min);
    //    hist1d_fill(v, &a);
    //}

    hist1d_print(&a);

    return 0;
}

#else

#include "pairwise-sum.h"

int draw_random_numbers(struct SummatorInterface * sum) {
    for(size_t i = 0; i < 1000; ++i) {
        double v = (1.*rand())/RAND_MAX;
        sum->add(v, sum->summatorState);
    }
    return 0;
}

static void _pairwise_sum_add(double value, void * userdata) {
    struct PairwiseSummator * S = (struct PairwiseSummator *) userdata;
    pairwise_summator_add(S, value);
}

static void _pairwise_sum_print(void * userdata) {
    struct PairwiseSummator * S = (struct PairwiseSummator *) userdata;
    printf("%e\n", pairwise_summator_get(S));
}


static void _direct_sum_add(double value, void * userdata) {
    *((double *) userdata) += value;
}

static void _direct_sum_print(void * userdata) {
    printf("%e\n", *((double *) userdata));
}

int main(int argc, char *argv[]) {
    if(argc != 2)
        return 1;

    struct SummatorInterface iface;

    if(0 == strcmp(argv[1], "pairwise")) {
        puts("pairwise sum in use");
        iface.summatorState = malloc(sizeof(struct PairwiseSummator));
        pairwise_summator_init((struct PairwiseSummator*) iface.summatorState, 10);
        iface.add = _pairwise_sum_add;
        iface.print = _pairwise_sum_print;
    } else if(0 == strcmp(argv[1], "direct")) {
        puts("direct sum in use");
        iface.summatorState = malloc(sizeof(double));
        *((double*) iface.summatorState) = 0;
        iface.add = _direct_sum_add;
        iface.print = _direct_sum_print;
    } else {
        return 1;
    }
    
    draw_random_numbers(&iface);

    iface.print(iface.summatorState);

    return 0;
}
#endif

