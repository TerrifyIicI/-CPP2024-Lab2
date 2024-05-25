#include "FileWriter.h"
#include <string.h>

FileWriter::FileWriter(const char* name) {
        file = fopen(name, "wt");
}


FileWriter::~FileWriter() {
    if (file != nullptr) {
        fclose(file);
    }
}

int FileWriter::write(const char* string) {
    if (strlen(string) == 1) {
        return 0;
    }
    fputs(string, file);
    return 1;
}

void FileWriter::writeNameAndRootComparison(const string& name, int comparisonResult) {
    stringstream ss;
    ss << name << " " << comparisonResult << endl;
    fprintf(file, "%s", ss.str().c_str());
}
