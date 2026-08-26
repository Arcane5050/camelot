#ifndef MACROS_H
#define MACROS_H
#include <string>
#include <vector>
using namespace std;

void parseMacros(vector<vector<string>>* IR);
void inlineMacros(vector<vector<string>>* IR);

#endif