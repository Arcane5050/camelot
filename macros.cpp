#include "macros.hpp"

static map<string, string> macro_table = {};

void parseMacros(vector<vector<string>>* IR) {
    macro_table["#EOF"] = to_string(EOF_);
    macro_table["#DEOF"] = to_string(EOF_ * 4);
    vector<string> replaced = {
        "rmove", "?", "?", ""
    };
    const size_t max = IR->size();
    for (size_t i = 0; i < max; i++) {
        vector<string>* instr = &IR->at(i);
        if (instr->at(0)[0] == '#') {
            const string& val = instr->at(1);
            const string& reg = instr->at(2);
            validateRegister(reg, i);
            macro_table[instr->at(0)] = reg;
            subVerbose("Found macro '" + instr->at(0) + "'");
            replaced[1] = val;
            replaced[2] = reg;
            *instr = replaced;
        }
    }
    subVerbose("Found " + to_string(macro_table.size() - 2) + " macro(s)");
}

void inlineMacros(vector<vector<string>>* IR) {
    const size_t max = IR->size();
    size_t macros_inlined = 0;
    for (size_t i = 0; i < max; i++) {
        vector<string>* instr = &IR->at(i);
        for (size_t comp_index = 1; comp_index < INSTR_SIZE; comp_index++) {
            string* comp = &instr->at(comp_index);
            if (!comp->empty()) {
                auto macro_table_index = macro_table.find(*comp);
                if (macro_table_index != macro_table.end()) {
                    *comp = "addr" + macro_table_index->second;
                    macros_inlined++;
                } else if (*comp == "#HERE") {
                    *comp = "addr" + to_string(i);
                    macros_inlined++;
                }
            }
        }
    }
    subVerbose("Inlined " + to_string(macros_inlined) + " macro(s)");
}
