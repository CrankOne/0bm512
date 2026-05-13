#include "direct.hh"

#include <iostream>

DirectSummatorCpp::DirectSummatorCpp() {
    _state = 0.;
}

void
DirectSummatorCpp::add(double value) {
    _state += value;
}

void
DirectSummatorCpp::print() const {
    std::cout << _state << std::endl;
}

