#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <cstdio>
#include <string>
#include <sstream>

using namespace std;

class FileWriter {
public:
    FILE* file;

    FileWriter(const char* name);
    ~FileWriter();

    int write(const char* string);
    void writeNameAndRootComparison(const string& name, int comparisonResult);
};

#endif // FILEWRITER_H
