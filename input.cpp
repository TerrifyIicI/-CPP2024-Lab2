#include <iostream>
#include <string>
#include <regex>

using namespace std;

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

enum class StateNum {
    START,
    MINUS,
    INTEGER,
    DOT,
    FRACTION,
    IMAGINARY,
    SPACE,
    SIGN,
    ERROR
};

#include <sstream>
#include <sstream>

ComplexNumber is_valid_number(const string& number) {
    double real = 0.0;
    double image = 0.0;

    StateNum state = StateNum::START;
    int dot_count = 0;
    int imaginary_flag = 0;
    bool sign_found = false;
    bool minus_found = false;
    string current_token = "";
    int i = 0;

    while (i < number.length()) {
        char c = number[i];

        if (isdigit(c) || (dot_count <= 1 && c == '.')) {
            current_token += c;
            state = StateNum::INTEGER;
            if (dot_count == 1) {
                state = StateNum::FRACTION;
            }
        } else if (c == '-') {
            if (state == StateNum::START) {
               current_token += c;
            } else if (state == StateNum::INTEGER) {
                minus_found = true;
                sign_found = true;
                state = StateNum::SIGN;
            }
        } else if (c == '+') {
            if (state == StateNum::INTEGER) {
                minus_found = false;
                sign_found = true;
                state = StateNum::SIGN;
            }
        } else if (c == 'i') {
            if (state == StateNum::INTEGER || state == StateNum::FRACTION) {
                imaginary_flag++;
                state = StateNum::IMAGINARY;
            }
        } else if (c == ' ') {
            // Просто пропустим пробелы
        } else {
            state = StateNum::ERROR;
        }

        if (state == StateNum::ERROR) {
            return ComplexNumber(0, 0);
        }

        if (state == StateNum::IMAGINARY && !current_token.empty()) {
            stringstream ss(current_token);
            double token_value;
            ss >> token_value;

            if (minus_found) {
                image = -token_value;
                minus_found = false;
            } else {
                image = token_value;
            }

            current_token = "";
        }

        if (state == StateNum::SIGN) {
            stringstream ss(current_token);
            double token_value;
            ss >> token_value;

            if (sign_found && imaginary_flag == 0) {
                real = token_value;
            } else {
                image = token_value;
            }
            current_token = ""; // Обнуляем токен, так как теперь ожидается мнимая часть
        }

        i++;
    }

    if (!current_token.empty()) {
        stringstream ss(current_token);
        double token_value;
        ss >> token_value;

        if (imaginary_flag == 1) {
            image = token_value;
        } else {
            real = token_value;
        }
    }

    if (sign_found && !imaginary_flag) {
        // Сгенерировать ошибку, если поднят флаг знака, но не поднят флаг image и достигнут конец строки
        return ComplexNumber(0, 0);
    }

    return ComplexNumber(real, image);
}

int main() {
    std::string numbers[] = {
        "-2.000000",
        "0.250000+1.198958i",
        "0.250000-1.198958i",
        "2.645751",
        "-2.645751",
        "0.666667 - 1.374369i",
        "0.666667 + 1.374369i",
        "0",
        "1.000000 + 1.732051i",
        "1.000000- 6.732051i",
        "0.00000",
        "1.4574784",
        "1.50000",
        "577",
        "2.000000",
        "-3.000000",
        "2.000000",
        "2i",
        "-0.500000",
        "1.000000",
        "3.000000",
        "0",
        "0.333333",
        "-2.000000",
        "5.000000",
        "-5.000000",
        "0",
        "3.000000",
        "0.500000",
        "3.316625",
        "-3.316625",
        "0",
        "1.732051",
        "-1.732051",
        "0",
        "1.000000",
        "-0.500000",
        "3.000000",
        "-0.500000 +0.866025i",
        "-0.50"
    };
    ComplexNumber complex;
    for (const auto& number : numbers) {
        complex = is_valid_number(number);
        cout << " " << complex.real << " " << complex.imag*1000 << std::endl;
    }

    return 0;
}
