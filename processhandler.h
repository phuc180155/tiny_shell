#pragma once

#ifndef _PROCESSHANDLER_H_
#define _PROCESSHANDLER_H_

#include <iostream>
#include <Windows.h>
#include <sys/types.h>
using namespace std;

void GetAllProcess();

// Print All Child of Process which has PID
void GetChildProcess(DWORD pid);

// Print All Thread of Process which has PID
void GetThreadList(DWORD ProcessID);

// Suspend Process which has PID
void SuspendProcess(DWORD pid);

// Resume Process which has PID
void ResumeProcess(DWORD pid);

// Find Process ID by its name
void FindProcess(char *process_name);

// Kill Process by ID
void KillProcess(DWORD pid);

// Create a new process
void CreateNewProcess(char **argv);

#endif
