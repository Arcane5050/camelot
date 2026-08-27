#include "labels.hpp"

static map<string, string> label_table = {};

void parseLabels(vector<string>* lines) {
    const size_t max = lines->size();
    for (size_t i = 0; i < max; i++) {
        const string& line = lines->at(i);
        if (!line.empty()) {
            if (line[line.size() - 1] == ':') {
                label_table[line.substr(0, line.size() - 1)] = to_string(i);
                lines->at(i) = "";
            }
        }
    }
}

void inlineLabels(vector<vector<string>>* IR) {
    const size_t max = IR->size();
    for (size_t i = 0; i < max; i++) {
        vector<string>* instr = &IR->at(i);
        for (size_t comp_index = 1; comp_index < INSTR_SIZE; comp_index++) {
            string* comp = &instr->at(comp_index);
            if (!comp->empty()) {
                auto label_table_index = label_table.find(*comp);
                if (label_table_index != label_table.end()) {
                    *comp = "addr" + label_table_index->second;
                }
            }
        }
    }
}
