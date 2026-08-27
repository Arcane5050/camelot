#ifndef MACROS_H
#define MACROS_H
#include <string>
#include <vector>
#include <map>
using namespace std;

#include "camelot.hpp"
#include "parser.hpp"

void parseMacros(vector<vector<string>>* IR);
void inlineMacros(vector<vector<string>>* IR);

#endif