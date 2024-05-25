#include "Sorting.h"

using namespace std;

class StudentRating;

void Sorting::sortByRatingThenByName(vector<pair<string, StudentRating>>& studentInfosVector) {
    sort(studentInfosVector.begin(), studentInfosVector.end(), [](const pair<string, StudentRating>& a, const pair<string, StudentRating>& b) {
        if (a.second.rating == StudentRating::Rating::Null) {
            return true;
        } else if (b.second.rating == StudentRating::Rating::Null) {
            return false;
        } else if (a.second.rating != b.second.rating) {
            return a.second.rating < b.second.rating;
        } else {
            return a.first < b.first;
        }
    });
}

void Sorting::sortByName(vector<pair<string, StudentRating>>& studentInfosVector) {
    sort(studentInfosVector.begin(), studentInfosVector.end(), [](const pair<string, StudentRating>& a, const pair<string, StudentRating>& b) {
        return a.first < b.first;
    });
}
