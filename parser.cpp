#include "parser.hpp"

void removeEmptyLines(vector<string>* lines) {
    vector<string> reconstructed;
    for (const string& line : *lines) {
        if (!line.empty()) {
            reconstructed.push_back(line);
        }
    }
    reconstructed.shrink_to_fit();
    lines->swap(reconstructed);
}

void trimIndent_trailingSpaces(vector<string>* lines) {
    const size_t size = lines->size();
    for (size_t i = 0; i < size; i++) {
        const string& current_line = lines->at(i);
        const size_t max = current_line.size();
        size_t front = 0;
        while (isspace(current_line[front])) {
            front++;
            if (front > max) {
                break;
            }
        }
        size_t back = max;
        while (isspace(current_line[back])) {
            back--;
            if (back < 0) {
                break;
            }
        }
        const string trimmed = current_line.substr(front, back - front);
        lines->at(i) = trimmed;
    }
}
