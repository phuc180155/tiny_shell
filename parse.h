#ifndef _PARSE_H
#define _PARSE_H

#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <Windows.h>
#include <string>

// Split input:
char **SplitLine(char *buf);

// Combine path to a string:
char *Combine(char **argv, int begin);

#endif
