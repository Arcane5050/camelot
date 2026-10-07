#include "labels.hpp"

static map<string, string> label_table = {};

void parseLabels(vector<string>* lines) {
    size_t instr_index = 0;
    for (size_t i = 0; i < EOF_; i++) {
        const string& line = lines->at(i);
        if (!line.empty()) {
            if (line[line.size() - 1] == ':') {
                const string id = line.substr(0, line.size() - 1);
                label_table[id] = to_string(instr_index);
                lines->at(i) = "";
                subVerbose("Found label '" + id + "'");
            }
            instr_index++;
        }
    }
    subVerbose("Found " + to_string(label_table.size()) + " label(s)");
}

void inlineLabels(vector<vector<string>>* IR) {
    size_t labels_inlined = 0;
    for (size_t i = 0; i < EOF_; i++) {
        vector<string>* instr = &IR->at(i);
        for (size_t comp_index = 1; comp_index < INSTR_SIZE; comp_index++) {
            string* comp = &instr->at(comp_index);
            if (!comp->empty()) {
                auto label_table_index = label_table.find(*comp);
                if (label_table_index != label_table.end()) {
                    *comp = "addr" + label_table_index->second;
                    labels_inlined++;
                }
            }
        }
    }
    subVerbose("Inlined " + to_string(labels_inlined) + " label(s)");
}
