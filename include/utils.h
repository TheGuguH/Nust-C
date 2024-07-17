#ifndef NUSTC_UTILS_H
#define NUSTC_UTILS_H

#include <string.h>

#define STR_EQUALS(string, value, value_s) (strncmp(string, value, value_s + 1) == 0)

#endif