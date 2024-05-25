#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H
#include <cmath>
#include <sstream>
#include <complex>
#include <string>
using namespace std;

struct QuadraticEquation {
    double a;
    double b;
    double c;
};

struct ComplexNumber {
    double real;
    double imag;

    ComplexNumber(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    ComplexNumber operator+(const ComplexNumber& other) const;
};

ostream& operator<<(ostream& os, const ComplexNumber& cn);

ComplexNumber is_valid_number(const string& number);

struct Solution {
    string equation;
    pair<ComplexNumber, ComplexNumber> solutions; // Пара комплексных чисел для хранения двух корней
    string name;
};


#endif // DATASTRUCTURES_H
