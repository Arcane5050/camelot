#include "macros.hpp"

static map<string, string> macro_table;

void parseMacros(vector<vector<string>>* IR) {
    vector<string> replaced = {
        "rmove", "?", "?", ""
    };
    const size_t max = IR->size();
    for (size_t i = 0; i < max; i++) {
        vector<string>* instr = &IR->at(i);
        if (instr->at(0)[0] == '#') {
            const string& val = instr->at(1);
            const string& reg = instr->at(2);
            macro_table[instr->at(0)] = reg;
            replaced[1] = val;
            replaced[2] = reg;
            *instr = replaced;
        }
    }
}

void inlineMacros(vector<vector<string>>* IR) {
    const size_t max = IR->size();
    for (size_t i = 0; i < max; i++) {
        vector<string>* instr = &IR->at(i);
        const size_t instr_size = instr->size();
        for (size_t comp_index = 0; comp_index < instr_size; comp_index++) {
            string* comp = &instr->at(comp_index);
            auto macro_table_index = macro_table.find(*comp);
            if (macro_table_index != macro_table.end()) {
                *comp = "addr" + macro_table_index->second;
            }
        }
    }
}
