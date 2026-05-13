#include "histogram.h"

#include <stdlib.h>
#include <stdio.h>

void hist1d_init(struct Histogram1D * hist, int nBins, float min, float max) {
    hist->nBins = nBins;
    hist->counters = (int*) malloc(hist->nBins*sizeof(int));
    hist->min = min;
    hist->max = max;
}

void hist1d_reset( struct Histogram1D * hist ) {
    hist->min = hist->max = 0;
    for(int i = 0; i < hist->nBins; ++i) {
        hist->counters[i] = 0;
    }
}

void hist1d_fill(float v, struct Histogram1D * hist ) {
    if(v < hist->min) {
        ++(hist->underflow);
        return;
    }
    if(v > hist->max) {
        ++(hist->overflow);
        return;
    }
    int nBin = (v - hist->min)*hist->nBins/(hist->max - hist->min);
    ++(hist->counters[nBin]);
}

void hist1d_generate_and_fill(struct Histogram1D * hist, int nEvents
        , float (*func)() ) {
    for(int i = 0; i < nEvents; ++i) {
        // get random value in definition range
        float v = hist->min + func()*(hist->max - hist->min);// ...
        hist1d_fill(v, hist);
    }
}

void hist1d_print( struct Histogram1D * hist ) {
    printf("-1 %f %f %d\n", -INFINITY, hist->min, hist->underflow);
    for(int i = 0; i < hist->nBins; ++i) {
        float step = (hist->max - hist->min)/hist->nBins;
        printf("%d %f %f %d\n", i, i*step, (i+1)*step, hist->counters[i]);
    }
    printf("%d %f %f %d\n", hist->nBins, hist->max, INFINITY, hist->overflow);
}

void hist1d_free(struct Histogram1D * hist) {
    free(hist->counters);
}
