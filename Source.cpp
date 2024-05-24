#define _CRT_SECURE_NO_WARNINGS
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
#include <map>
using namespace std;

struct Solution {
    string equation;
    pair<ComplexNumber, ComplexNumber> solutions; // Пара комплексных чисел для хранения двух корней
    string name;
};


Solution processLine(const string& line) {
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
    // Если в строке одна мнимая часть (d), то это одно комплексное число, которое включает в себя и вещественную, и мнимую части
    else if (solutionsStr.find("i") != string::npos) {
        sol.solutions.first = is_valid_number(solutionsStr);
        sol.solutions.second = ComplexNumber(0, 0);
    }
    // Если в строке нет мнимых частей (d), то разделительный пробел идет после первого числа
    else if (solutionsStr.find(" ") != string::npos) {
        delimPos = solutionsStr.find(" ") + 1;
        sol.solutions.first = is_valid_number(solutionsStr.substr(0, delimPos));
        sol.solutions.second = is_valid_number(solutionsStr.substr(delimPos));
    }
    // Если в строке только одно вещественное число (real), то это одно число
    else {
        sol.solutions.first = is_valid_number(solutionsStr);
        sol.solutions.second = ComplexNumber(0, 0);
    }

    return sol; // Возврат решения
}
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
        static char line[256]; // Используем статический массив для уменьшения выделений памяти
        if (fgets(line, sizeof(line), file) != nullptr) {
            // Находим первый символ перевода строки ('\r' или '\n') и заменяем его на нулевой символ ('\0')
            char* newline = strpbrk(line, "\r\n");
            if (newline != nullptr) {
                *newline = '\0';
            }
        }
        return line;
    }
    
    Solution readAndProcessLine() {
        char* line = readLine();
        return processLine(line);
    }
};
struct StudentRating {
    int correctSolutions;
    enum class Rating { Good, Average, Poor, Null }; // Перемещаем перечисление внутрь структуры
    Rating rating;
};

void processSolution(Solution sol, double epsilon, map<string, StudentRating>& studentInfos) {
    auto it = studentInfos.find(sol.name);
    if (it == studentInfos.end()) {
        // Если студента нет в списке, добавляем его с начальными значениями
        studentInfos[sol.name] = StudentRating{0, StudentRating::Rating::Null};
    }

    StudentRating& studentInfo = studentInfos[sol.name];

    // Проверяем, является ли решение нулевым
    bool isZeroSolution = EquationProcessor().compareRoots(pair<ComplexNumber, ComplexNumber>({0,0}, {0,0}), sol.solutions, epsilon);

    // Проверяем, правильное ли решение
    bool rootsAreEqual = EquationProcessor().compareEquationAndSolutions(sol.equation, sol.solutions, epsilon);

    // Обновляем количество правильно решенных задач
    if (rootsAreEqual) {
        studentInfo.correctSolutions++;
    }

    // Инициализация рейтинга или обновление существующего
    if (studentInfo.rating == StudentRating::Rating::Null) {
        // Если решение нулевое и неправильное
        if (isZeroSolution && !rootsAreEqual) {
            studentInfo.rating = StudentRating::Rating::Poor;
        }
        // Если решение не нулевое и неправильное
        else if (!isZeroSolution && !rootsAreEqual) {
            studentInfo.rating = StudentRating::Rating::Average;
        }
        // Если решение не нулевое и правильное
        else if (!isZeroSolution && rootsAreEqual) {
            studentInfo.rating = StudentRating::Rating::Good;
        }
        // Если решение нулевое и правильное
        else if (isZeroSolution && rootsAreEqual) {
            studentInfo.rating = StudentRating::Rating::Null;
        }

    } else if (studentInfo.rating == StudentRating::Rating::Good) {
        // Если текущий ответ правильный, ничего не делаем
        if (!rootsAreEqual) {
            // Если текущий ответ неправильный, переводим в средний
            studentInfo.rating = StudentRating::Rating::Average;
        }
    } else if (studentInfo.rating == StudentRating::Rating::Poor) {
        // Если текущий ответ неправильный, ничего не делаем
        if (!isZeroSolution) {
            // Если текущий ответ не нулевой, переводим в средний
            studentInfo.rating = StudentRating::Rating::Average;
        }
    } else if (studentInfo.rating == StudentRating::Rating::Average) {
        // Если рейтинг уже средний, никаких изменений не производим
    }
}
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

    void writeNameAndRootComparison(const string& name, int comparisonResult) {
        stringstream ss;
        ss << name << " " << comparisonResult << endl;
        fprintf(file, "%s", ss.str().c_str());
    }
};

class Sorting {
    public:
    static void sortByRatingThenByName(vector<pair<string, StudentRating>>& studentInfosVector) {
        sort(studentInfosVector.begin(), studentInfosVector.end(), [](const pair<string, StudentRating>& a, const pair<string, StudentRating>& b) {
            if (a.second.rating == StudentRating::Rating::Null) {
                return true; //если студент везде написал 0 и везде угадал, то требуются доп. вопросы, поэтому он будет в начале списка
            } else if (b.second.rating == StudentRating::Rating::Null) {
                return false;
            } else if (a.second.rating != b.second.rating) {
                return a.second.rating < b.second.rating;
            } else {
                return a.first < b.first;
            }
        });
    }

    static void sortByName(vector<pair<string, StudentRating>>& studentInfosVector) {
        sort(studentInfosVector.begin(), studentInfosVector.end(), [](const pair<string, StudentRating>& a, const pair<string, StudentRating>& b) {
            return a.first < b.first;
        });
    }
};


int main() {
    setlocale(LC_ALL, "ru-RU.UTF-8");
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

    return 0;

}