#ifndef PARSE_H
#define PARSE_H
#include <string>
#include <vector>
#include <map>
using namespace std;

#include "camelot.hpp"

vector<string> splitLineBySpaces(const string& line);
void trimNonInt(const string& source, string* dest);

void trimIndent_trailingSpaces(vector<string>* lines);
void removeEmptyLines(vector<string>* lines);
void splitSpaces(const vector<string>& lines, vector<vector<string>>* dest);
void translate(const vector<vector<string>>& IR, vector<vector<char>>* bytecode);

#endif
