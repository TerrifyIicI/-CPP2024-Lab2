#ifndef SOLUTIONVALIDATOR_H
#define SOLUTIONVALIDATOR_H

#include "ComplexNumber.h"
#include <string>
#include <string.h>
#include <cmath>
#include <iostream>
using namespace std;

class EquationProcessor {
public:
    QuadraticEquation parseEquation(const char* line);
    pair<ComplexNumber, ComplexNumber> calculateRoots(QuadraticEquation equation);
    bool compareRoots(const pair<ComplexNumber, ComplexNumber>& roots1, const pair<ComplexNumber, ComplexNumber>& roots2, double epsilon);
    bool compareEquationAndSolutions(const string& equation, const pair<ComplexNumber, ComplexNumber>& solutions, double epsilon);
private:
    void addValue(QuadraticEquation& equation, int sign, double value, int power);
};

#endif // SOLUTIONVALIDATOR_H
