#include "DataStructures.h"

// Перегрузка оператора вывода в поток
ostream& operator<<(ostream& os, const ComplexNumber& cn) {
    os << cn.real;
    if (cn.imag != 0) {
        os << (cn.imag > 0 ? " + " : " - ") << abs(cn.imag) << "i";
    }
    return os;
}

// Перечисление StateNum
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

// Функция is_valid_number
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
