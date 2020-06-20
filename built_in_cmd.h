#pragma once
#ifndef _BUILT_IN_CMD_H
#define _BUILT_IN_CMD_H

#include <iostream>
#include <Windows.h>
#include <ctime>
#include "parse.h"

using namespace std;

void Help(char **argv);

void Cd(char **argv, LPSTR cur_dir);

void DateAndTime(char **argv);

void Dir(char **argv, LPSTR cur_dir);

void GetAllEnvironmentVariables();

void GetAnEnvironmentVariable(char **argv);

void SetEnvironmentVariable(char **argv);

void Envi(char **argv);


#endif
