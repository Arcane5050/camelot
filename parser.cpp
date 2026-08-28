#include "parser.hpp"

static const string instruction_table[INSTR_COUNT] = {
    "mrcopy",
    "rmcopy",
    "extern",
    "rcomp",
    "cexit",
    "mprint",
    "minput",
    "radd",
    "rsub",
    "rless",
    "rmore",
    "dmcopy",
    "mdcopy",
    "jmp",
    "cjmp",
    "rjmp",
    "rmove",
};

void removeEmptyLines(vector<string>* lines) {
    vector<string> reconstructed;
    for (const string& line : *lines) {
        if (!line.empty()) {
            reconstructed.push_back(line);
        }
    }
    lines->swap(reconstructed);
    lines->shrink_to_fit();
}

void trimIndent_trailingSpaces(vector<string>* lines) {
    for (size_t i = 0; i < EOF_; i++) {
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

vector<string> splitLineBySpaces(const string& line) {
    vector<string> result;
    string current;
    for (const char character : line) {
        if (!isspace(character)) {
            current.push_back(character);
        } else {
            result.push_back(current);
            current.clear();
        }
    }
    if (!current.empty()) {
        result.push_back(current);
    }
    result.shrink_to_fit();
    return result;
}

void splitSpaces(const vector<string>& lines, vector<vector<string>>* dest) {
    vector<vector<string>> result;
    result.reserve(EOF_);
    for (const string& line : lines) {
        vector<string> current = splitLineBySpaces(line);
        const auto max = static_cast<short>(current.size());
        for (short i = 0; i < INSTR_SIZE - max; i++) {
            current.emplace_back("");
        }
        result.push_back(current);
    }
    dest->swap(result);
    dest->shrink_to_fit();
}

static bool startsWith(const string& src, const string& search) {
    if (src.substr(0, search.size()) == search) {
        return true;
    }
    return false;
}

string trimNonInt(const string& source) {
    string reconstructed;
    for (const char c : source) {
        if (isdigit(c)) {
            reconstructed.push_back(c);
        }
    }
    return reconstructed;
}

void translate(const vector<vector<string>>& IR, vector<byte_t>* bytecode) {
    vector<byte_t> result;
    size_t instr_pos = 0;
    for (const vector<string>& instr : IR) {
        byte_t opcode = 255;
        const string& instr_name = instr[0];
        for (short i = 0; i < INSTR_COUNT; i++) {
            const string& current = instruction_table[i];
            if (instr_name == current) {
                opcode = static_cast<char>(i);
                break;
            }
        }
        if (opcode == 255) {
            error("Invalid instruction", to_string(instr_pos));
            errors_found++;
        }
        result.push_back(opcode);
        for (short i = 1; i < INSTR_SIZE; i++) {
            string comp = instr[i];
            if (comp.empty()) {
                result.push_back(0);
            } else if (
                startsWith(comp, "addr") ||
                startsWith(comp, "n")
            ) {
                comp = trimNonInt(comp);
                result.push_back(static_cast<byte_t>(stoi(comp)));
            } else if (startsWith(comp, "reg")) {
                validateRegister(comp, instr_pos);
                comp = trimNonInt(comp);
                result.push_back(static_cast<byte_t>(stoi(comp)));
            } else {
                if (comp.size() == 1) {
                    result.push_back(static_cast<byte_t>(comp[0]));
                } else {
                    error("Label/Macro not found: " + comp, to_string(instr_pos));
                    errors_found++;
                }
            }
        }
        instr_pos++;
    }
    bytecode->swap(result);
    bytecode->shrink_to_fit();
}