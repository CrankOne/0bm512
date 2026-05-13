#pragma once

#include "pairwise-sum.h"
#include "sum.hh"

class PairwiseSummatorCpp: public iSummator {
private:
    PairwiseSummator _state;
public:
    PairwiseSummatorCpp(size_t n=10);
    void add(double value) override;
    void print() const override;
};

