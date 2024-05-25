#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "ComplexNumber.h"
#include "EquationProcessor.h"
#include "StudentRating.h"
#include "GenerateHTML.h"
#include "Solution.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "Sorting.h"
#include <map>
using namespace std;


string getHTMLFilename(const string& filename, char separator = '.') {
    size_t pos = filename.find_last_of(separator);
    if (pos != string::npos) {
        return filename.substr(0, pos) + ".html";
    }
    return filename + ".html";
}

int main() {
    double epsilon = 1e-5; // точность
    map<string, StudentRating> studentInfos;
    vector<pair<string, StudentRating>> studentInfosVector;
    string filename;
    string choice;

    cout << "Enter input filename (default=ex.txt): ";
    getline(cin, filename);
    if (filename.empty()) {
        filename = "ex.txt"; //default
    }
    FileReader fileReader(filename.c_str());

    while (!fileReader.isEndOfFile()) {
        Solution sol = fileReader.readAndProcessLine();
        processSolution(sol, epsilon, studentInfos);
    }
    studentInfosVector.assign(studentInfos.begin(), studentInfos.end());

    cout << "Choose a sorting method:" << endl;
    cout << "   1) Sort by name" << endl;
    cout << "   2) Sort by name and rating" << endl;
    cout << "Enter a number (default=1): ";
    getline(cin, choice);
    if (choice.empty()) {
        choice = "1"; //default
    }

    if (choice == "1") {
        Sorting::sortByName(studentInfosVector);
    }
    else if (choice == "2"){
        Sorting::sortByRatingThenByName(studentInfosVector);
    }

    cout << "Enter output filename (default=output.txt): ";
    getline(cin, filename);
    if (filename.empty()) {
        filename = "output.txt"; //default
    }
    FileWriter outputFile(filename.c_str());

    for (const auto& studentInfo : studentInfosVector) {
        outputFile.writeNameAndRootComparison(studentInfo.first, studentInfo.second.correctSolutions);
    }

    generateHTML(studentInfosVector, getHTMLFilename(filename));

    return 0;
}