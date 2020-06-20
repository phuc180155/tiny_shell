#include <iostream>
#include <Windows.h>
#include "built_in_cmd.h"
#include "processhandler.h"

const char *spe_arg[] = {"help", "cd", "exit", "envi", "datetime", "dir", "ps"};

const char *spe_non_arg[] = {"exit"};
const char *spe_one_arg[] = {"help", "datetime", "envi", "ps"};
const char *spe_two_arg[] = {"cd", "dir"};
int (*builtin_one_param)[](**args) = {
	&Help;
	&DateAndTime;
	&Envi;
	&ProcessCommand;
};

int (*builtin_two_param)[](**args, LPSTR cur_dir) = {
	&Cd;
	&Dir;
};

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
}

void BuiltInCommand(char **argv, LPSTR cur_dir){
	int i;
	bool done = false ;
	if (strcmp(argv[0], spe_non_arg[0])==0){
		done = true;
		exit(0);
	}
	for(i=0; i<4;++i){
		if(strcmp(argv[0],spe_one_arg[i])==0){
			done =  true;
			(*builtin_one_param[i])(argv);
		}
	}
	for(i=0; i<2;++i){
		if(strcmp(argv[0],spe_two_arg[i])==0){
			done =  true;
			(*builtin_two_param[i])(argv, cur_dir));
		}
	}
	if(!done)
		cout << "Command not found.\nType 'help' to know more information\n";
}
