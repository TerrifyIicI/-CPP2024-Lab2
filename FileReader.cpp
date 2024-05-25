#include "FileReader.h"
#include <iostream>
using namespace std;

class EquationProcessor;

FileReader::FileReader(const char* filename) {
    file = fopen(filename, "rt");
    if (file == nullptr) {
        cerr << "Ошибка файла " << filename << endl;
        exit(EXIT_FAILURE);
    }
}

FileReader::~FileReader() {
    if (file != nullptr) {
        fclose(file);
    }
}

bool FileReader::isEndOfFile() {
    return feof(file);
}

char* FileReader::readLine() {
    static char line[256];
    if (fgets(line, sizeof(line), file) != nullptr) {
        char* newline = strpbrk(line, "\r\n");
        if (newline != nullptr) {
            *newline = '\0';
        }
    }
    return line;
}

Solution FileReader::readAndProcessLine() {
    char* line = readLine();
    return processLine(line);
}
