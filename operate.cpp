#include <iostream>
#include <Windows.h>
#include "built_in_cmd.h"
#include "processhandler.h"

const char *pro_one_arg[] = {"-all"};
const char *pro_pid_arg[] = {"-child",  "-thread", "-suspend", "-resume",  "-kill"};
const char *pro_name_arg[] = {"-find"};
const char *pro_run_arg[] = {"-fore", "-back"};
void (*pro_one_func[]) () = {GetAllProcess};
void (*pro_pid_func[]) (DWORD) = {GetChildProcess, GetThreadList, SuspendProcess, ResumeProcess, KillProcess};
void (*pro_name_func[]) (char *) = {FindProcess};
void (*pro_run_func[]) (char **) = {CreateNewProcess};

void ProcessCommand(char **argv) {
	if (argv[1] == NULL) {
		cout << "Required arguments.\n";
		return;
	}
	int i;
	bool done = false ;
	if (strcmp(argv[1], pro_one_arg[0])==0){
		(*pro_one_func[0])();
		return;
	}
	
	for(i=0; i<5;++i){
		if(strcmp(argv[1],pro_pid_arg[i])==0){
			if (argv[2] == NULL)	{
				cout << "Required arguments.\n";
				return;
			}
			done =  true;
			DWORD pid = atoi(argv[2]);		// Convert string -> int
			(*pro_pid_func[i])(pid);
		}
	}
	if (strcmp(argv[1], pro_name_arg[0])==0) {		
		if (argv[2] == NULL) {
			cout << "Required arguments.\n";
			return;
		}
		done = true;
		(*pro_name_func[0])(argv[2]);
	}
	
	for(i=0; i<2;++i){
		if(strcmp(argv[1],pro_run_arg[i])==0){
			if (argv[2] == NULL)	{
				cout << "Required arguments.\n";
				return;
			}
			done =  true;
			(*pro_run_func[0])(argv);
		}
	}
	if(done == false)
		cout << "Command " << argv[0] << " does not have option " << argv[1] << "\n";
}
	
const char *spe_non_arg[] = {"exit"};
const char *spe_one_arg[] = {"help", "datetime", "envi", "ps"};
const char *spe_two_arg[] = {"cd", "dir"};	
void (*builtin_one_param[]) (char **) = {Help, DateAndTime, Envi, ProcessCommand};
void (*builtin_two_param[]) (char **, LPSTR) = {Cd, Dir};
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
			(*builtin_two_param[i])(argv, cur_dir);
		}
	}
	if(!done)
		cout << "Command not found.\nType 'help' to know more information\n";
}
