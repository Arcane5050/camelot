#include "camelot.hpp"

#include <sstream>
#include <cstring>
using namespace std;

#include "parser.hpp"
#include "labels.hpp"
#include "macros.hpp"

#define BOLD    "\033[1m"
#define REGULAR "\033[39;0m"
#define GREEN   "\033[92m"
#define CYAN    "\033[96m"
#define YELLOW  "\033[93m"
#define RED     "\033[91m"

static bool do_verbose = false;
static vector<string> errors;

int main(int argc, char** argv) {
    ofstream file;
    if (argc == 1) {
        cout << ("--= CAMELOT =--\n"
                 "The Arthur ASM assembler\n"
                 "\n"
                 "Usage: camelot <files[]> -o <outfile> [flags]\n"
                 "\n"
                 "FLAGS:\n"
                 "  --verbose, -v : Turn on verbose mode\n");
        return 0;
    }
    vector<string> lines;
    string out_file = "out.abf";
    for (size_t i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            do_verbose = true;
        } else if (strcmp(argv[i], "--out") == 0 || strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc) {
                error("Expected output file path");
                return 1;
            }
            out_file = argv[++i];
        } else {
            ifstream current_file(argv[i]);
            if (!current_file.is_open()) {
                error("File '" + string(argv[i]) + "' failed to open");
                errors_found++;
            } else {
                string current_line;
                while (getline(current_file, current_line)) {
                    lines.push_back(current_line);
                }
            }
            current_file.close();
        }
    }
    if (lines.empty()) {
        error("All files failed to open");
        return 1;
    }
    verbose("Formatting");
    subVerbose("Removed blank lines");
    lines.shrink_to_fit();
    EOF_ = lines.size();
    vector<vector<string>> IR;
    trimIndent_trailingSpaces(&lines);
    subVerbose("Removed indent");
    verbose("Labels");
    parseLabels(&lines);
    verbose("Formatting");
    removeEmptyLines(&lines);
    subVerbose("Removed empty lines");
    EOF_ = lines.size();
    if (lines.empty()) {
        error("No instructions found");
        return 1;
    }
    verbose("Parsing");
    splitSpaces(lines, &IR);
    subVerbose("Parsed text");
    verbose("Macros");
    parseMacros(&IR);
    verbose("Inlining");
    inlineLabels(&IR);
    subVerbose("Inlined labels");
    inlineMacros(&IR);
    subVerbose("Inlined macros");
    verbose("Bytecode");
    vector<byte_t> bytecode;
    translate(IR, &bytecode);
    if (errors_found > 0) {
        cout << BOLD ":: " RED "Error(s) (" << to_string(errors_found) << ")\n" REGULAR;
        for (const string& error : errors) {
            cout << error << "\n";
        }
        return 1;
    }
    file.open(out_file, ios::binary);
    for (byte_t byte : bytecode) {
        file << byte;
    }
    file.close();

    verbose("Completed");
}

void error(const string& msg, const long long position) {
    errors.emplace_back(BOLD "  :: " REGULAR RED + msg + REGULAR);
    if (position != -1) {
        errors.emplace_back(BOLD RED "    ==> " REGULAR CYAN "At position " + to_string(position));
    }
}

void verbose(const string& msg) {
    if (do_verbose) {
        string color = BOLD CYAN;
        if (errors_found > 0) {
            color = REGULAR YELLOW;
        }
        cout << BOLD ":: " << color << msg << REGULAR "\n";
    }
}
void subVerbose(const string& msg) {
    if (do_verbose) {
        string color = BOLD GREEN;
        if (errors_found > 0) {
            color = BOLD YELLOW;
        }
        cout << color << "  ==> " REGULAR CYAN << msg << REGULAR "\n";
    }
}

void validateRegister(const string &reg, const size_t location) {
    if (stoi(trimNonInt(reg)) > reg_count) {
        error("Register index cannot be above " + to_string(reg_count), static_cast<long long>(location));
        errors_found++;
    }
}
