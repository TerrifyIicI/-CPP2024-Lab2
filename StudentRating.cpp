
#include "StudentRating.h"

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