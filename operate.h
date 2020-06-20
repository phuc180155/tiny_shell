#pragma once

#ifndef _OPERATE_H_
#define _OPERATE_H_

#include <iostream>
#include <Windows.h>
#include <sys/types.h>
using namespace std;

void BuiltInCommand(char **argv, LPSTR cur_dir);

void ProcessCommand(char **argv);

#endif
