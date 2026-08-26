#include "labels.hpp"

static map<string, size_t> label_table;

void parseLabels(const vector<string>* lines) {
    const size_t max = lines->size();
    for (size_t i = 0; i < max; i++) {
        const string& line = lines->at(i);
        if (line[line.size() - 1] == ':') {
            label_table[line.substr(0, line.size() - 1)] = i;
        }
    }
}

void inlineLabels(vector<vector<string>>* IR) {
    const size_t max = IR->size();
    for (size_t i = 0; i < max; i++) {
        vector<string>* instr = &IR->at(i);
        const size_t instr_size = instr->size();
        for (size_t comp_index = 0; comp_index < instr_size; comp_index++) {
            string* comp = &instr->at(comp_index);
            auto label_table_index = label_table.find(*comp);
            if (label_table_index != label_table.end()) {
                *comp = "addr" + to_string(label_table_index->second);
            }
        }
    }
}
