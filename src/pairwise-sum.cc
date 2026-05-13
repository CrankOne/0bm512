#include "pairwise-sum.hh"
#include "pairwise-sum.h"
#include <iostream>

PairwiseSummatorCpp::PairwiseSummatorCpp(size_t n) {
    pairwise_summator_init(&_state, n);
}

void
PairwiseSummatorCpp::add(double value) {
    pairwise_summator_add(&_state, value);
}

void
PairwiseSummatorCpp::print() const {
    std::cout << pairwise_summator_get(&_state) << std::endl;
}
