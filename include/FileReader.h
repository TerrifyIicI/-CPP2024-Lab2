#ifndef FILEREADER_H
#define FILEREADER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Solution.h"
#include "EquationProcessor.h"

class EquationProcessor;

class FileReader {
private:
    FILE* file;
    EquationProcessor equationProcessor;

public:
    FileReader(const char* filename);
    ~FileReader();

    bool isEndOfFile();
    char* readLine();
    Solution readAndProcessLine();
};

#endif // FILEREADER_H
