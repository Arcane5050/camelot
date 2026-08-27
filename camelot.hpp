#ifndef CAMELOT_H
#define CAMELOT_H

#define INSTR_SIZE 4
#define INSTR_COUNT 18

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

inline int errors_found = 0;

void error(const string& msg, const string& prefix);
void verbose(const string& msg, const string& prefix);

#endif