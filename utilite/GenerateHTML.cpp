#include "GenerateHTML.h"
#include <fstream>
#include <iostream>

void generateHTML(const vector<pair<string, StudentRating>>& studentInfosVector, const string& filename) {
    ofstream htmlFile(filename);

    if (!htmlFile.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    htmlFile << "<!DOCTYPE html>\n";
    htmlFile << "<html>\n";
    htmlFile << "<head>\n";
    htmlFile << "<title>Student Ratings</title>\n";
    htmlFile << "<style>\n";
    htmlFile << "table, th, td { border: 1px solid black; border-collapse: collapse; }\n";
    htmlFile << "th { background-color: #ccc; }\n";
    htmlFile << ".good { background-color: #90ee90; }\n";
    htmlFile << ".average { background-color: #ffff8c; }\n";
    htmlFile << ".poor { background-color: #ffb6c1; }\n";
    htmlFile << ".null { background-color: #c9c9c9; }\n";
    htmlFile << "div.color-key { display: inline-block; vertical-align: top; margin-left: 20px; width: 380px; }\n";
    htmlFile << "table { float: left; }\n";
    htmlFile << "</style>\n";
    htmlFile << "</head>\n";
    htmlFile << "<body>\n";
    htmlFile << "<table>\n";
    htmlFile << "<tr><th>Name</th><th>Correct Solutions</th></tr>\n";

    for (const auto& studentInfo : studentInfosVector) {
        const string& name = studentInfo.first;
        const StudentRating& rating = studentInfo.second;
        const char* ratingClass;

        switch (rating.rating) {
            case StudentRating::Rating::Good:
                ratingClass = "good";
                break;
            case StudentRating::Rating::Average:
                ratingClass = "average";
                break;
            case StudentRating::Rating::Poor:
                ratingClass = "poor";
                break;
            case StudentRating::Rating::Null:
            default:
                ratingClass = "null";
                break;
        }

        htmlFile << "<tr><td class=\"" << ratingClass << "\">" << name << "</td><td class=\"" << ratingClass << "\">" << rating.correctSolutions << "</td></tr>\n";
    }

    htmlFile << "</table>\n";
    htmlFile << "<div class=\"color-key\">\n";
    htmlFile << "<p>Ключ цветов:</p>\n";
    htmlFile << "<ul>\n";
    htmlFile << "<li><span class=\"good\">●</span> Хорошо: всегда решают задачи правильно</li>\n";
    htmlFile << "<li><span class=\"average\">●</span> Среднее: есть шанс решить задачу правильно, но могут также ошибаться</li>\n";
    htmlFile << "<li><span class=\"poor\">●</span> Плохо: всегда пишут, что корень один и это 0</li>\n";
    htmlFile << "<li><span class=\"null\">●</span> Null: требуется дополнительный вопрос</li>\n";
    htmlFile << "</ul>\n";
    htmlFile << "<p>Color key:</p>\n";
    htmlFile << "<ul>\n";
    htmlFile << "<li><span class=\"good\">●</span> Good: always solve tasks correctly</li>\n";
    htmlFile << "<li><span class=\"average\">●</span> Average: have a chance to solve tasks correctly, but can also make mistakes</li>\n";
    htmlFile << "<li><span class=\"poor\">●</span> Poor: always write that the root is one and it is 0</li>\n";
    htmlFile << "<li><span class=\"null\">●</span> Null: requires an additional question</li>\n";
    htmlFile << "</ul>\n";  
    htmlFile << "</div>\n";
    htmlFile << "</body>\n";
    htmlFile << "</html>\n";

    htmlFile.close();
}
