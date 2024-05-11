#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <string>
#include <complex>
#include <string.h>
#include <regex>
#include <vector>
#include <sstream>

using namespace std;

struct QuadraticEquation {
    double a;
    double b;
    double c;
};

struct ComplexNumber {
    double real;
    double imag;

    ComplexNumber(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    ComplexNumber operator+(const ComplexNumber& other) const {
        return ComplexNumber(real + other.real, imag + other.imag);
    }

    friend ostream& operator<<(ostream& os, const ComplexNumber& cn) {
        os << cn.real;
        if (cn.imag != 0) {
            os << (cn.imag > 0 ? " + " : " - ") << abs(cn.imag) << "i";
        }
        return os;
    }
};

struct Solution {
    const char* equation;
    pair<ComplexNumber, ComplexNumber> solutions; // Пара комплексных чисел для хранения двух корней
    string name;
};

// Helper function to trim whitespace from both ends of a string
static inline string trim(const std::string &s) {
    auto wsfront = find_if_not(s.begin(), s.end(), [](int c) { return isspace(c); });
    auto wsback = find_if_not(s.rbegin(), s.rend(), [](int c) { return isspace(c); }).base();
    return (wsback <= wsfront ? string() : string(wsfront, wsback));
}

class EquationProcessor {
public:
    QuadraticEquation parseEquation(const char* line);
    pair<ComplexNumber, ComplexNumber> calculateRoots(QuadraticEquation equation);
    bool compareRoots(const pair<ComplexNumber, ComplexNumber>& roots1, const pair<ComplexNumber, ComplexNumber>& roots2, double epsilon);
    bool compareEquationAndSolutions(const char* equation, const pair<ComplexNumber, ComplexNumber>& solutions, double epsilon);
    Solution processLine(const string& line);
private:
    void addValue(QuadraticEquation& equation, int sign, double value, int power);
};

QuadraticEquation EquationProcessor::parseEquation(const char* line) {
    QuadraticEquation equation = { 0, 0, 0 }; // инициализация структуры QuadraticEquation нулями
    bool isRightSide = false; // флаг, указывающий, находимся ли мы на правой стороне уравнения (после знака '=')
    int sign = 1; // текущий знак (по умолчанию положительный)
    double value = 0; // текущее значение (коэффициент при переменной)
    int i = 0;

    while (i < strlen(line)) {
        if (line[i] >= '0' && line[i] <= '9') { // если текущий имвол - цифра
            value = value * 10 + (line[i] - '0'); // добавляем цифру к текущему значеню
        }
        else if (line[i] == 'x') { // если текущий символ - 'x' (переменная)
            i++;
            if (value == 0) {
                value = 1;
            }
            if (line[i] == '^') { // если после 'x' следует '^' (степень)
                if ((line[i + 1] > '2') || (i < strlen(line) - 2 && line[i + 2] >= '0' && line[i + 2] <= '9')) {
                    cout << "Это ловушка, уравнение не квадратное" << endl; // если степень больше 2, то уравнение не квадратное
                    equation = { 0, 0, 0 };
                    return equation;
                }
                if (line[i + 1] == '2') { // если степень равна 2
                    i++;
                    addValue(equation, sign, value, 2); // добавляем значение к коэффициенту при x^2
                }
                else if (line[i + 1] == '0') { // если степень равна 0
                    i++;
                    addValue(equation, sign, value, 0); // обавляем значение к свободному лену
                }
                else { // если степень равна 1
                    addValue(equation, sign, value, 1); // добавляем значение к коэффициенту при x
                }
                value = 0;
            }
            else { // если после 'x' нет '^', то степень равна 1
                i--;
                if (value == 0) {
                    value = 1;
                }
                addValue(equation, sign, value, 1); // обавляем значение к коэффициенту при x
            }
            value = 0;
        }
        else if (line[i] == '+' || line[i] == '-') { // если текущий символ - '+' или '-'
            if (value != 0) {
                addValue(equation, sign, value, 0); // добавляем значение к свободному члену
                value = 0;
            }
            sign = (line[i] == '+') ? 1 : -1; // меняем знак на противоположный
            if (isRightSide) {
                sign *= -1;
            }
        }
        else if (line[i] == '=') { // если текущий символ - '='
            if (value != 0) {
                addValue(equation, sign, value, 0); // добавляем значение к свободному члену
                value = 0;
            }
            isRightSide = true; // переходим н правую сторону уравнения
            sign = -1; // меняем знак на противоположный
        }
        i++;
    }

    if (value != 0) {
        addValue(equation, sign, value, 0); // добавляем значение к свободному члену
    }
    if (equation.a == 0) {
        cout << "Это ловушка, уравнение не квадратное" << endl; // если коэффициент при x^2 равен 0, то уравнение не квадратное
        equation = { 0, 0, 0 };
        return equation;
    }
    return equation;
}
void EquationProcessor::addValue(QuadraticEquation& equation, int sign, double value, int power) {
    if (power == 2) {
        equation.a += sign * value;
    }
    else if (power == 1) {
        equation.b += sign * value;
    }
    else {
        equation.c += sign * value;
    }
}
// calculateRoots вычисляет корни квадратного уравнения и возвращает их в виде пары комплексных чисел
pair<ComplexNumber, ComplexNumber> EquationProcessor::calculateRoots(QuadraticEquation equation) {
    // вычисляем дискриминант
    double discriminant = equation.b * equation.b - 4 * equation.a * equation.c;

    // проверяем, не является ли уравнение ривиальным
    if (equation.a == 0 && equation.b == 0 && equation.c == 0) {
        return make_pair(ComplexNumber(0.0, 0.0), ComplexNumber(0.0, 0.0));
    }
    // если дискриминант больше 0, то корни вещественные и различные
    else if (discriminant > 0) {
        double root1_real = (-equation.b + sqrt(discriminant)) / (2 * equation.a);
        double root2_real = (-equation.b - sqrt(discriminant)) / (2 * equation.a);
        return make_pair(ComplexNumber(root1_real, 0.0), ComplexNumber(root2_real, 0.0));
    }
    // если дискриминант равен 0, то корень ещественный и один
    else if (discriminant == 0) {
        double root_real = -equation.b / (2 * equation.a);
        // возвращаем пару одинаковых комплексных чисел
        return make_pair(ComplexNumber(root_real, 0.0), ComplexNumber(0.0, 0.0));
    }
    // если дискриминант меньше 0, то корни комплексные и сопряженные
    else {
        double realPart = -equation.b / (2 * equation.a);
        double imagPart = sqrt(-discriminant) / (2 * equation.a);
        // возвращаем пару комплексных чисел
        return make_pair(ComplexNumber(realPart, imagPart), ComplexNumber(realPart, -imagPart));
    }
}
bool EquationProcessor::compareRoots(const pair<ComplexNumber, ComplexNumber>& roots1, const pair<ComplexNumber, ComplexNumber>& roots2, double epsilon) {
    bool isEqual1 = (std::abs(roots1.first.real - roots2.first.real) < epsilon) && (std::abs(roots1.first.imag - roots2.first.imag) < epsilon) &&
                    (std::abs(roots1.second.real - roots2.second.real) < epsilon) && (std::abs(roots1.second.imag - roots2.second.imag) < epsilon);

    bool isEqual2 = (std::abs(roots1.first.real - roots2.second.real) < epsilon) && (std::abs(roots1.first.imag - roots2.second.imag) < epsilon) &&
                    (std::abs(roots1.second.real - roots2.first.real) < epsilon) && (std::abs(roots1.second.imag - roots2.first.imag) < epsilon);

    return isEqual1 || isEqual2;
}

bool EquationProcessor::compareEquationAndSolutions(const char* equation, const pair<ComplexNumber, ComplexNumber>& solutions, double epsilon) {
    QuadraticEquation parsedEquation = parseEquation(equation);
    pair<ComplexNumber, ComplexNumber> calculatedRoots = calculateRoots(parsedEquation);
    return compareRoots(calculatedRoots, solutions, epsilon);
}
Solution EquationProcessor::processLine(const string& line) {
    Solution sol;
    istringstream iss(line);
    vector<string> tokens;
    string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    // Имя студента всегда последнее
    sol.name = tokens.back();
    tokens.pop_back(); // Удаляем имя из списка токенов

    // Уравнение всегда первое
    sol.equation = tokens.front().c_str();

    // Обработка комплексных чисел
    size_t i = 1;
    while (i < tokens.size()) {
        double real = 0.0;
        double imag = 0.0;
        bool hasImaginary = false;

        // Считываем реальную часть
        real = stod(tokens[i++]);

        // Проверяем наличие следующего токена и его содержимое
        if (i < tokens.size() && (tokens[i] == "+" || tokens[i] == "-")) {
            string sign = tokens[i++];
            if (i < tokens.size() && tokens[i].find('i') != string::npos) {
                hasImaginary = true;
                string imagToken = tokens[i++];
                imagToken.pop_back(); // Удаляем 'i'
                imag = stod(imagToken);
                if (sign == "-") {
                    imag = -imag;
                }
            }
        }

        // Создаем комплексное число
        ComplexNumber root(real, hasImaginary ? imag : 0.0);
        if (sol.solutions.first.real == 0 && sol.solutions.first.imag == 0) {
            sol.solutions.first = root;
        } else {
            sol.solutions.second = root;
        }
    }

    return sol;
}

class FileReader {
private:
    FILE* file;
    EquationProcessor equationProcessor;

public:
    FileReader(const char* filename) {
        file = fopen(filename, "rt");
        if (file == nullptr) {
            std::cerr << "Ошибка фйла " << filename << std::endl;
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
        fgets(line, 256, file);
        return line;
    }

    Solution readAndProcessLine() {
        char* line = readLine();
        string str(line);
        delete[] line;
        return equationProcessor.processLine(str);
    }
};

class FileWriter {
public:
    FILE* file;
    FileWriter(const char* name) {
        file = fopen(name, "wt");
    }
    int write(const char* string) {
        if (strlen(string) == 1)
            return 0;
        fputs(string, file);
        return 1;
    }
    ~FileWriter() {
        if (file != nullptr) {
            fclose(file);
        }
    }

    void writeNameAndRootComparison(const string& name, bool comparisonResult) {
        stringstream ss;
        ss << name << " " << comparisonResult << endl;
        fprintf(file, "%s", ss.str().c_str());
    }
};

int main() {
    char filename[16];
    cout << "Введите имя входного файла: ";
    cin >> filename;
    FileReader fileReader(filename);

    cout << "Введите имя выходного файла: ";
    cin >> filename;
    FileWriter outputFile(filename);
    EquationProcessor equationProcessor;

    double epsilon = 1e-5; //точность

    while (!fileReader.isEndOfFile()) {
        Solution sol = fileReader.readAndProcessLine();
        bool rootsAreEqual = equationProcessor.compareEquationAndSolutions(sol.equation, sol.solutions, epsilon);
        outputFile.writeNameAndRootComparison(sol.name, rootsAreEqual);
    }

    return 0;
}

