#include "camelot.hpp"

#include "parser.hpp"
#include "labels.hpp"
#include "macros.hpp"

int main(int argc, char** argv) {
    const string& filepath = argv[1];
    cout << filepath << "\n";
    ifstream file(filepath);
    if (!file.is_open()) {
        error("File not found", argv[1]);
        return 1;
    }
    vector<string> lines;
    string current_line;
    while (getline(file, current_line)) {
        lines.push_back(current_line);
    }
    lines.shrink_to_fit();
    file.close();
    vector<vector<string>> IR;
    trimIndent_trailingSpaces(&lines);
    removeEmptyLines(&lines);
    parseLabels(&lines);
    splitSpaces(lines, &IR);
    parseMacros(&IR);
    inlineLabels(&IR);
    inlineMacros(&IR);
    vector<vector<char>> bytecode;
    translate(IR, &bytecode);
    if (errors_found > 0) {
        error(to_string(errors_found) + " errors found", "FAIL");
        return 1;
    }
}

void error(const string& msg, const string& prefix) {
    cout << "\033[38;2;255;0;0m" << prefix << ": " << msg << "\033[39m\n";
}
