#include "EquationProcessor.h"
#include "DataStructures.h"


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
    // если дискриминант равен 0, то корень еущественный и один
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

bool EquationProcessor::compareEquationAndSolutions(const string& equation, const pair<ComplexNumber, ComplexNumber>& solutions, double epsilon) {
    QuadraticEquation parsedEquation = parseEquation(equation.c_str());
    pair<ComplexNumber, ComplexNumber> calculatedRoots = calculateRoots(parsedEquation);
    return compareRoots(calculatedRoots, solutions, epsilon);
}