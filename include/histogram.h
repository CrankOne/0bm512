#ifndef H_0BM512_H
#define H_0BM512_H 1

#ifdef __cplusplus
extern "C" {
#endif

struct Histogram1D {
    int nBins;
    int * counters;
    int underflow, overflow;
    float min;
    float max;
};

void hist1d_init(struct Histogram1D * hist, int nBins, float min, float max);

void hist1d_reset( struct Histogram1D * hist );

void hist1d_fill(float v, struct Histogram1D * hist );

void hist1d_generate_and_fill(struct Histogram1D * hist, int nEvents
        , float (*func)() );

void hist1d_print( struct Histogram1D * hist );

void hist1d_free(struct Histogram1D * hist);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  /* H_0BM512_H */
