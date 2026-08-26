#include "parser.hpp"

static map<string, char> instruction_table = {
    { "mrcopy", 0x00 },
    { "rmcopy", 0x01 },
    { "extern", 0x02 },
    { "rcomp",  0x03 },
    { "cexit",  0x04 },
    { "mprint", 0x05 },
    { "minput", 0x06 },
    { "radd",   0x07 },
    { "rsub",   0x08 },
    { "rless",  0x09 },
    { "rmore",  0x0A },
    { "dmcopy", 0x0B },
    { "mdcopy", 0x0C },
    { "jmp",    0x0D },
    { "cjmp",   0x0E },
    { "mjmp",   0x0F },
    { "rmove",  0x10 },
};

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

void splitSpaces(const vector<string>& lines, vector<vector<string>>* dest) {
    vector<vector<string>> result;
    vector<string> current;
    for (const string& line : lines) {
        string current_comp;
        for (const char character : line) {
            if (!isspace(character)) {
                current_comp.append({character,'\0'});
            } else {
                current_comp.clear();
                current.push_back(current_comp);
            }
        }
        if (!current.empty()) {
            current.push_back(current_comp);
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
static void trimNonInt(string* dest) {
    string reconstructed;
    for (const char c : *dest) {
        if (isalnum(c)) {
            reconstructed.append({c,0});
        }
    }
    dest->swap(reconstructed);
    dest->shrink_to_fit();
}

void translate(const vector<vector<string>>& IR, vector<vector<char>>* bytecode) {
    vector<vector<char>> result;
    vector<char> wip_instr;
    for (const vector<string>& instr : IR) {
        wip_instr.push_back(instruction_table[instr[0]]);
        for (short i = 0; i < 4; i++) {
            string comp = instr[i];
            if (comp.empty()) {
                wip_instr.push_back(0);
            }
            if (
                startsWith(comp, "addr") ||
                startsWith(comp, "reg")  ||
                startsWith(comp, "n")
            ) {
                trimNonInt(&comp);
                wip_instr.push_back(static_cast<char>(stoi(comp)));
            }
        }
        wip_instr.shrink_to_fit();
        result.push_back(wip_instr);
    }
}