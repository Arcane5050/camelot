#ifndef CAMELOT_H
#define CAMELOT_H
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

inline int errors_found = 0;

void error(const string& msg, const string& prefix);

#endif