#include "camelot.hpp"
#include <sstream>
#include <cstring>

#include "parser.hpp"
#include "labels.hpp"
#include "macros.hpp"

static bool do_verbose = false;

int main(int argc, char** argv) {
    if (argc == 1) {
        cout << "Usage: camelot <files*> [--verbose]\n";
        return 0;
    }
    vector<string> lines;
    for (size_t i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--verbose") == 0) {
            do_verbose = true;
        } else {
            ifstream file(argv[i]);
            if (!file.is_open()) {
                error("File failed to open", argv[i]);
                errors_found++;
            } else {
                string current_line;
                while (getline(file, current_line)) {
                    lines.push_back(current_line);
                }
            }
            file.close();
        }
    }
    if (lines.empty()) {
        error("All files failed to open", FAIL_PREFIX);
        return 1;
    }
    lines.shrink_to_fit();
    EOF_ = lines.size();
    vector<vector<string>> IR;
    trimIndent_trailingSpaces(&lines);
    verbose("Removed indent and trailing spaces", FORMAT_PREFIX);
    parseLabels(&lines);
    verbose("Parsed all labels", LABELS_PREFIX);
    removeEmptyLines(&lines);
    EOF_ = lines.size();
    verbose("Removed all empty lines", FORMAT_PREFIX);
    if (lines.empty()) {
        error("No instructions found", FAIL_PREFIX);
        return 1;
    }
    splitSpaces(lines, &IR);
    verbose("Split all lines by spaces", LABELS_PREFIX);
    parseMacros(&IR);
    verbose("Parsed all macros", MACROS_PREFIX);
    inlineLabels(&IR);
    verbose("Inlined all labels", LABELS_PREFIX);
    inlineMacros(&IR);
    verbose("Inlined all macros", MACROS_PREFIX);
    vector<vector<char>> bytecode;
    translate(IR, &bytecode);
    verbose("Translated all instructions and arguments to 8-bit integers", PARSER_PREFIX);
    if (errors_found > 0) {
        error(to_string(errors_found) + " error(s) found", FAIL_PREFIX);
        return 1;
    }
    verbose("Completed", "");
    cout << "[\n";
    for (const vector<char>& instr : bytecode) {
        cout << "  [ ";
        for (const char& c : instr) {
            cout << static_cast<int>(c) << ", ";
        }
        cout << "],\n";
    }
    cout << "]\n";
}

void error(const string& msg, const string& prefix) {
    cout << "\033[38;2;255;0;0m[@] " << prefix << ": " << msg << "\033[39m\n";
}

void verbose(const string& msg, const string& prefix) {
    if (do_verbose) {
        string color = "\033[38;2;0;255;0m[!] ";
        if (errors_found > 0) {
            color = "\033[38;2;255;255;0m[!] ";
        }
        cout << color << prefix << ": " << msg << "\033[39m\n";
    }
}

void validateRegister(const string &reg, const size_t location) {
    if (stoi(trimNonInt(reg)) > reg_count) {
        error("Register index cannot be above " + to_string(reg_count), to_string(location));
        errors_found++;
    }
}
