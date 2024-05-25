#ifndef SOLUTION_H
#define SOLUTION_H
#include "ComplexNumber.h"
#include <string>
#include <complex>
#include <utility>

using namespace std;


struct Solution {
    string equation;
    pair<ComplexNumber, ComplexNumber> solutions;
    string name;
};

Solution processLine(const string& line);

#endif // SOLUTION_H
