#ifndef STUDENT_RATING_H
#define STUDENT_RATING_H

#include <map>
#include <string>
#include "Solution.h"
#include "EquationProcessor.h"

struct StudentRating {
    int correctSolutions;
    enum class Rating { Good, Average, Poor, Null };
    Rating rating;
};

void processSolution(Solution sol, double epsilon, std::map<std::string, StudentRating>& studentInfos);

#endif // STUDENT_RATING_H
