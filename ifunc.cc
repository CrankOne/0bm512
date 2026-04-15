#include <cmath>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "histogram.h"

float ifunc(float u, float lambda) {
    return - log(1 - u);
}

int main(int argc, char * argv[]) {
    Histogram1D h;
    hist1d_init(&h, 40, 0, 15);
    hist1d_reset(&h);

    // 8.956e-11

    for(int i = 0; i < 1e2; ++i) {
        float u = ((float) rand())/RAND_MAX;
        float v = ifunc(u, 1);
        hist1d_fill(v, &h);
        printf("%e\n", v);
    }
    hist1d_print(&h);
    hist1d_free(&h);
    return 0;
}


