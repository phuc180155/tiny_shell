#pragma once

#ifndef _PROCESSHANDLER_H_
#define _PROCESSHANDLER_H_

#include <iostream>
#include <Windows.h>
#include <sys/types.h>
using namespace std;

void GetAllProcess();					// show all processes

void GetChildProcess(DWORD pid);		// show All Child of Process

void GetThreadList(DWORD ProcessID);	 // Show All Thread of Process 

void CreateNewProcess(char **argv);		// Run a process

void FindProcess(char *process_name);	// Find a Process

void SuspendProcess(DWORD pid);			// Suspend a process

void ResumeProcess(DWORD pid);			// Resume a process

void KillProcess(DWORD pid);			// Terminate a process

#endif
