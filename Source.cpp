#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <complex>
#include <string.h>
#include <regex>
#include <vector>
#include <sstream>
#include "DataStructures.h"
#include "EquationProcessor.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;


struct Solution {
    string equation;
    pair<ComplexNumber, ComplexNumber> solutions; // Пара комплексных чисел для хранения двух корней
    string name;
};
class FileReader {
private:
    FILE* file;
    EquationProcessor equationProcessor;

public:
    FileReader(const char* filename) {
        file = fopen(filename, "rt");
        if (file == nullptr) {
            cerr << "Ошибка файла " << filename << endl;
            exit(EXIT_FAILURE);
        }
    }

    ~FileReader() {
        if (file != nullptr) {
            fclose(file);
        }
    }

    bool isEndOfFile() {
        return feof(file);
    }

    char* readLine() {
        char* line = new char[256];
        if (fgets(line, 256, file) != nullptr) {
            // Удаляем символ перевода строки ('\n') и заменяем его символом окончания строки ('\0')
            int len = strlen(line);
            if (len > 0 && line[len - 1] == '\n') {
                line[len - 1] = '\0';
            }
        }
        return line;
    }

    Solution readAndProcessLine() {
        char* line = readLine();
        return processLine(line);
    }

    static Solution processLine(const string& line) {
        Solution sol;

        // 1. Извлечь уравнение
        size_t eqPos = line.find(' ');
        sol.equation = line.substr(0, eqPos);

        // 2. Извлечь имя студента
        size_t namePos = line.find_last_of(' ');
        sol.name = line.substr(namePos + 1);
        string solutionsStr = line.substr(eqPos + 1, namePos - eqPos - 1);

        // Удаляем пробелы перед именем студента
        solutionsStr = solutionsStr.substr(0, solutionsStr.find_last_not_of(' ') + 1);

        // 3. Обработать решения
        size_t delimPos; // Позиция разделительного пробела

        // Если в строке есть две мнимые части (d), то разделительный пробел идет после первой мнимой части (d)
        if (solutionsStr.find("i ") != string::npos) {
            delimPos = solutionsStr.find("i ") + 1;
            sol.solutions.first = is_valid_number(solutionsStr.substr(0, delimPos));
            sol.solutions.second = is_valid_number(solutionsStr.substr(delimPos));
        }
        // Если в строке нет мнимых частей (d), то разделительный пробел идет после первого числа
        else if (solutionsStr.find(" ") != string::npos) {
            delimPos = solutionsStr.find(" ") + 1;
            sol.solutions.first = is_valid_number(solutionsStr.substr(0, delimPos));
            sol.solutions.second = is_valid_number(solutionsStr.substr(delimPos));
        }
        // Если в строке одна мнимая часть (d), то это одно комплексное число, которое включает в себя и вещественную, и мнимую части
        else if (solutionsStr.find("i") != string::npos) {
            sol.solutions.first = is_valid_number(solutionsStr);
            sol.solutions.second = ComplexNumber(0, 0);
        }
        // Если в строке только одно вещественное число (real), то это одно число
        else {
            sol.solutions.first = is_valid_number(solutionsStr);
            sol.solutions.second = ComplexNumber(0, 0);
        }

        return sol; // Возврат решения
    }
};

class FileWriter {
public:
    FILE* file;

    FileWriter(const char* name) {
        file = fopen(name, "wt");
    }

    ~FileWriter() {
        if (file != nullptr) {
            fclose(file);
        }
    }

    int write(const char* string) {
        if (strlen(string) == 1) {
            return 0;
        }
        fputs(string, file);
        return 1;
    }

    void writeNameAndRootComparison(const string& name, bool comparisonResult) {
        stringstream ss;
        ss << name << " " << comparisonResult << endl;
        fprintf(file, "%s", ss.str().c_str());
    }
};

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    char filename[16];
    cout << "Введите имя входного файла: ";
    cin >> filename;
    FileReader fileReader(filename);

    cout << "Введите имя выходного файла: ";
    cin >> filename;
    FileWriter outputFile(filename);
    EquationProcessor equationProcessor;

    double epsilon = 1e-5; // точность

    while (!fileReader.isEndOfFile()) {
        Solution sol = fileReader.readAndProcessLine();
        bool rootsAreEqual = equationProcessor.compareEquationAndSolutions(sol.equation, sol.solutions, epsilon);
        outputFile.writeNameAndRootComparison(sol.name, rootsAreEqual);
    }

    return 0;
}
