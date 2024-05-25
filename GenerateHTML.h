#ifndef GENERATEHTML_H
#define GENERATEHTML_H

#include <string>
#include <vector>
#include <map>
#include "StudentRating.h"

void generateHTML(const vector<pair<string, StudentRating>>& studentInfosVector, const string& filename);

#endif // GENERATEHTML_H
