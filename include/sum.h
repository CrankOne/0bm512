#ifndef H_0BM512_SUM_H
#define H_0BM512_SUM_H 1

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*AddFunc_t)(double value, void * userdata);
typedef void (*Print_t)(void *);

struct SummatorInterface {
    AddFunc_t add;
    void * summatorState;
    Print_t print;
};

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  /* H_0BM512_SUM_H */
