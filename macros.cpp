#include "macros.hpp"

static map<string, string> macro_table;

void parseMacros(vector<vector<string>>* IR) {
    vector<string> replaced = {
        "rmove", "?", "?", ""
    };
    const size_t max = IR->size();
    for (size_t i = 0; i < max; i++) {
        vector<string>* instr = &IR->at(i);
        vector<string> line = instr[0]; // TODO: fucking weird
        if (line[0][0] == '#') {
            const string& val = line[1];
            const string& reg = line[2];
            macro_table[line[0]] = reg;
            replaced[1] = val;
            replaced[2] = reg;
            *instr = replaced;
        }
    }
}
