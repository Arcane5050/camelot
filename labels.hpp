#ifndef LABELS_H
#define LABELS_H
#include <string>
#include <vector>
#include <map>
using namespace std;

void parseLabels(const vector<string>* lines);
void inlineLabels(vector<vector<string>>* IR);

#endif