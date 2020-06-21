#include <iostream>
#include <Windows.h>
#include "parse.h"
#include <ctime>
#include "processhandler.h"
#include <iomanip>

using namespace std;

void Help(char **argv) {
	if (argv[2] != NULL) {
		cout << "Provides help information for TinyShell command" << endl;
		cout << "Syntax :\n";
		cout << "       "
			 << "help -[command]" << endl;
		return;
	}
	if (argv[1] == NULL) {
		cout << "Type \"help -[command]\" to understand the command obviously.\n";
		cout << "Suppoted commands:\n cd\n datetime\n dir\n ps\n envi\n exit\n";
	}
	else if (!strcmp(argv[1], "-cd")) {
		cout << "Change the current directory. You must write the new directory after this command.\nEXAMPLE: \"cd C:/\"\n";
	}
	else if (!strcmp(argv[1], "-datetime"))	{
		cout << "Show the current time.\n";
	}
	else if (!strcmp(argv[1], "-ps")) {
		cout << "Supported options:\n";
		cout << " " << setw(15) << left << "all"
			 << "Print all the running processes.\n";
		cout << " " << setw(15) << left << "back"
			 << "Running a process in background mode.\n";
		cout << " " << setw(15) << left << "fore"
			 << "Running a process in foreground mode.\n";
		cout << " " << setw(15) << left << "suspend"
			 << "Suspend a process by its pid.\n";
		cout << " " << setw(15) << left << "resume"
			 << "Resume a process by its pid.\n";
		cout << " " << setw(15) << left << "kill"
			 << "Terminate a process by its pid.\n";
		cout << " " << setw(15) << left << "child"
			<< "Show all the childrent process of a process by its pid\n";
		cout << " " << setw(15) << left << "thread"
			<< "Show all the threads of a process by its pid\n";
		cout << " " << setw(15) << left << "find"
			<< "Find a process by its pid\n";
		cout << "Systex parameter: " << endl;
		cout << "                "
			 << "-[option]" << endl;
	}
	else if (!strcmp(argv[1], "-dir"))	{
		cout << "Show list of files or folders in current folder.\n";
		cout << "EXAMPLES: \"dir\"\n";
	}
	else if (!strcmp(argv[1], "-envi"))	{
		cout << "Supported options:\n";
		cout << " " << setw(15) << left << "all"
			 << "Show all the environment variables.\n";
		cout << " " << setw(15) << left << "get"
			 << "Get value of a specific environment variable.\n";
		cout << " " << setw(15) << left << "set"
			 << "Set value of a specific environment variable.\n";
	}
	else if (!strcmp(argv[1], "-exit"))	
		cout << "Exit shell.\n";
	else	{
		cout << "Command not found!\nType 'help' to know more details.\n";
		return;
	}
}

void Cd(char **argv, LPSTR cur_dir) {
	if (argv[1] == NULL) {
		cout << "Current Directory: " << cur_dir << "\n";
		return;
	}
	char *path = CombineLine(argv, 1);
	if (SetCurrentDirectory(path) == 0) 	
		cout << "The system cannot find the specified path.\n";
}

void DateAndTime(char **argv) {
	if (argv[1] != NULL) {
		cout << "Command \"datetime\" does not support any option !\n";
		return;
	}
	time_t now = time(0);		// current date/time based on current system
 	char* dt = ctime(&now);		// convert now to string form
 	cout << "Current date and time is: " << dt << "\n";
	return;
}

void Dir(char **argv, LPSTR cur_dir)
{
	if (argv[1] != NULL) 	{
		cout << "Command \"dir\" does not support any option\n";
		return;
	}
	WIN32_FIND_DATA data;	// Describes a file found by FindNextFile() or FindFirstFile
	FILETIME time_lastaccess;	// A structure describes time last access 1 file
	SYSTEMTIME convert_time;
	cur_dir = strcat(cur_dir, "//*");
	HANDLE handle = FindFirstFile(cur_dir, &data);
	cout << setw(15) << left << "DATE" << setw(15) << left << "TIME" << setw(15) << left << "TYPE" << setw(15) << left << "NAME" << "\n";
	cout<<"=========================================================================\n";
	while (handle != INVALID_HANDLE_VALUE)	{
			time_lastaccess = data.ftLastAccessTime;
			FileTimeToSystemTime(&time_lastaccess, &convert_time);
			// Convert date, time to string
			string date = to_string(convert_time.wDay) + '/' + to_string(convert_time.wMonth) + '/' + to_string(convert_time.wYear);
			string time = to_string(convert_time.wHour) + ':' + to_string(convert_time.wMinute) + ':' + to_string(convert_time.wSecond);
			if (data.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
				cout << setw(15) << left << date << setw(15) << left << time << setw(15) << left << "FILE" << setw(15) << left << data.cFileName << "\n";
			if (data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				cout << setw(15) << left << date << setw(15) << left << time << setw(15) << left << "FOLDER" << setw(15) << left << data.cFileName << "\n";
			if (FindNextFile(handle, &data) == FALSE) break;
	}
	FindClose(handle);
	return;
}

void GetAllEnvironmentVariables(){
	char *str = (char *) GetEnvironmentStrings();
	if (str == NULL) {
		cout << "Error when get environment variables.\n";
		return;
	}
	
	for (int i = 0;;i++) {
		if (str[i] != '\0') cout << str[i];
		else {
			cout <<"\n";
			if (str[i+1] == '\0') break;
		}		
	}
}

// Get a specific environment variable
void GetAnEnvironmentVariable(char **argv) {
	if (argv[2] == NULL) {
		cout << "Required arguments\n";
		return;
	}
	if (argv[3] != NULL) {
		cout << " Command \"enva -get\"  doesn't have any arguments\n";
		return;
	}
	char *value = (char *)calloc(10000, sizeof(char));
	DWORD byte = 10000 * sizeof(char);
	if (GetEnvironmentVariableA(argv[2], value, byte) == 0)
		cout << "Can't find variable "<< argv[2] << "\n";
	else
		cout << argv[2] << "=" << value << "\n";
	delete value;
	return;
}

// Set a specific environment variable
void SetEnvironmentVariable(char **argv) {	
	if (argv[3] == NULL) {
		cout << "Required arguments\n";
		return;
	}
	char *new_value = CombineLine(argv, 3);
	// get evironment var
	char *value = (char *)calloc(10000, sizeof(char));
	DWORD byte = 10000 * sizeof(char);
	// if variable doesn't exist, make a new variable. Else then add to existance value
	if (GetEnvironmentVariableA(argv[2], value, byte) == 0)	{
		if (!SetEnvironmentVariableA(argv[2], new_value))	{
			cout <<"Set environment variable ERROR\n";
			return;
		}
	}
	else {
		char *p = (char *)calloc (10000,sizeof(char));
		value = strcat(p, new_value);
		if (!SetEnvironmentVariableA(argv[2], value)) {
			cout << "Set environment variable error\n";
			return;
		}
	}
	delete value;
	return;
}

void Envi(char **argv) {
	if (argv[1] == NULL) {
		printf("Required arguments.\n");
		return;
	}
	if (strcmp(argv[1], "-all") == 0) {
		if (argv[2] != NULL) {
			cout << "Redundant arguments\n";
			return;
		}
		else {
			GetAllEnvironmentVariables();
			return;
		}
	}
	else if (strcmp(argv[1], "-get") == 0) {
		GetAnEnvironmentVariable(argv);
		return;
	}
	else if (strcmp(argv[1], "-set") == 0) {
		SetEnvironmentVariable(argv);
		return;
	}
	else {
		cout << "Option " << argv[1] << " not available in command\"envi\"\n";
		return;
	}
}
