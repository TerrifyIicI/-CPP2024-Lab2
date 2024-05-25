#include "Solution.h"
#include <string>
#include <sstream>
#include <cmath>

using namespace std;


Solution processLine(const string& line) {
    Solution sol;

    size_t eqPos = line.find(' ');
    sol.equation = line.substr(0, eqPos);

    size_t namePos = line.find_last_of(' ');
    sol.name = line.substr(namePos + 1);
    string solutionsStr = line.substr(eqPos + 1, namePos - eqPos - 1);

    solutionsStr = solutionsStr.substr(0, solutionsStr.find_last_not_of(' ') + 1);

    size_t delimPos;

    if (solutionsStr.find("i ") != string::npos) {
        delimPos = solutionsStr.find("i ") + 1;
        sol.solutions.first = is_valid_number(solutionsStr.substr(0, delimPos));
        sol.solutions.second = is_valid_number(solutionsStr.substr(delimPos));
    } else if (solutionsStr.find("i") != string::npos) {
        sol.solutions.first = is_valid_number(solutionsStr);
        sol.solutions.second = ComplexNumber(0, 0);
    } else if (solutionsStr.find(" ") != string::npos) {
        delimPos = solutionsStr.find(" ") + 1;
        sol.solutions.first = is_valid_number(solutionsStr.substr(0, delimPos));
        sol.solutions.second = is_valid_number(solutionsStr.substr(delimPos));
    } else {
        sol.solutions.first = is_valid_number(solutionsStr);
        sol.solutions.second = ComplexNumber(0, 0);
    }

    return sol;
}
