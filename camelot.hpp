#ifndef CAMELOT_H
#define CAMELOT_H

#define INSTR_SIZE 4
#define INSTR_COUNT 18

#define FAIL_PREFIX "FAIL"
#define FORMAT_PREFIX "Formatting"
#define LABELS_PREFIX "Labels"
#define MACROS_PREFIX "Macros"
#define PARSER_PREFIX "Parse"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

inline int errors_found = 0;
inline size_t EOF_;
inline constexpr int reg_count = 32;

void error(const string& msg, const string& prefix);
void verbose(const string& msg, const string& prefix);

void validateRegister(const string& reg, size_t location);

#endif