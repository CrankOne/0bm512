#include <stdio.h>
#include <stdlib.h>

#include "histogram.h"

float generate_uniform_scaled() {
    return rand() / (float) RAND_MAX;
}

float generate_gaussian_scaled() {
    // ..
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

