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

vector<string> splitLineBySpaces(const string& line) {
    vector<string> result;
    string current;
    for (const char character : line) {
        if (!isspace(character)) {
            current.append({character,'\0'});
        } else {
            current.clear();
            result.push_back(current);
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
    result.reserve(lines.size());
    for (const string& line : lines) {
        vector<string> current = splitLineBySpaces(line);
        const auto max = static_cast<short>(current.size());
        for (short i = 0; i < 4 - max; i++) {
            current.emplace_back("");
        }
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
void trimNonInt(const string& source, string* dest) {
    string reconstructed;
    for (const char c : source) {
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
    size_t instr_pos = 0;
    for (const vector<string>& instr : IR) {
        char opcode;
        auto index = instruction_table.find(instr[0]);
        if (index != instruction_table.end()) {
            opcode = index->second;
        } else {
            error("Invalid instruction", to_string(instr_pos));
            errors_found++;
        }
        wip_instr.push_back(opcode);
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
                trimNonInt(comp, &comp);
                wip_instr.push_back(static_cast<char>(stoi(comp)));
            } else {
                if (comp.size() == 1) {
                    wip_instr.push_back(comp[0]);
                } else {
                    error("Label/Macro not found: " + comp, to_string(instr_pos));
                    errors_found++;
                }
            }
        }
        wip_instr.shrink_to_fit();
        result.push_back(wip_instr);
        instr_pos++;
    }
}