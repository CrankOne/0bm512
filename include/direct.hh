#include "sum.hh"

class DirectSummatorCpp : public iSummator {
private:
    double _state;
public:
    DirectSummatorCpp();
    void add(double value) override;
    void print() const override;
};
