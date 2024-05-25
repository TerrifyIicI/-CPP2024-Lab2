#ifndef SORTING_H
#define SORTING_H

#include <vector>
#include <string>
#include <algorithm>
#include "StudentRating.h"

using namespace std;

class StudentRating;

class Sorting {
public:
    static void sortByRatingThenByName(vector<pair<string, StudentRating>>& studentInfosVector);
    static void sortByName(vector<pair<string, StudentRating>>& studentInfosVector);
};

#endif // SORTING_H
