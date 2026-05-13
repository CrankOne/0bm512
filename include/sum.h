#ifndef H_0BM512_SUM_H
#define H_0BM512_SUM_H 1

struct SummatorInterface {
    void (*add)(double value, void * userdata);
    void * summatorState;
    void (*print)(void *);
};

#endif  /* H_0BM512_SUM_H */
