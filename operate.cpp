#include <iostream>
#include <Windows.h>
#include "built_in_cmd.h"
#include "processhandler.h"

const char *spe_arg[] = {"help", "cd", "exit", "envi", "datetime", "dir", "ps"};

void ProcessCommand(char **argv) {
	if (argv[1] == NULL) {
		cout << "Required arguments.\n";
		return;
	}
	if (strcmp(argv[1], "-all") == 0) {
		GetAllProcess();
		return;
	}
	else if (strcmp(argv[1], "-thread") == 0) {
		if (argv[2] == NULL)	{
			cout << "Required arguments.\n";
			return;
		}
		DWORD pid = atoi(argv[2]);		// Convert string -> int
		GetThreadList(pid);
	}
	else if (strcmp(argv[1], "-find") == 0) {
		if (argv[2] == NULL) {
			cout << "Required arguments.\n";
			return;
		}
		FindProcess(argv[2]);
	}
	else if (strcmp(argv[1], "-child") == 0)	{
		if (argv[2] == NULL)		{
			cout << "Required arguments.\n";
			return;
		}
		DWORD pid = atoi(argv[2]);
		GetChildProcess(pid);
	}
	else if (strcmp(argv[1], "-suspend") == 0) {
		if (argv[2] == NULL) {
			cout << "Required arguments.\n";
			return;
		}
		DWORD pid = atoi(argv[2]);
		SuspendProcess(pid);
	}
	else if (strcmp(argv[1], "-resume") == 0) {
		if (argv[2] == NULL) {
			cout << "Required arguments.\n";
			return;
		}
		DWORD pid = atoi(argv[2]);
		ResumeProcess(pid);
	}
	else if (strcmp(argv[1], "-fore") == 0 || strcmp(argv[1], "-back") == 0) {
		if (argv[2] == NULL) {
			cout << "Required arguments.\n";
			return;
		}
		CreateNewProcess(argv);
	}
	else if (strcmp(argv[1], "-kill") == 0)	{
		if (argv[2] == NULL) {
			cout << "Required arguments.\n";
			return;
		}
		DWORD pid = atoi(argv[2]);
		KillProcess(pid);
	}
	else 
		cout << "Command " << argv[0] << " does not have option " << argv[1] << "\n";
	return;
}

void BuiltInCommand(char **argv, LPSTR cur_dir) {
	if (strcmp(argv[0], spe_arg[0]) == 0) 
		Help(argv);
	else if (strcmp(argv[0], spe_arg[1]) == 0) 
		Cd(argv, cur_dir);
	else if (strcmp(argv[0], spe_arg[2]) == 0) 
		exit(0);
	else if (strcmp(argv[0], spe_arg[3]) == 0) 
		Envi(argv);
	else if (strcmp(argv[0], spe_arg[4]) == 0)
		DateAndTime(argv);
	else if (strcmp(argv[0], spe_arg[5]) == 0)
		Dir(argv, cur_dir);
	else if (strcmp(argv[0], spe_arg[6]) == 0)
		ProcessCommand(argv);
	else
		cout << "Command not found.\nType 'help' to know more information\n";
	return;
}
