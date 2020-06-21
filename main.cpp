#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include "processhandler.h"
#include "parse.h"
#include "built_in_cmd.h"
#include "operate.h"
#include <signal.h>

using namespace std;

extern HANDLE h_fore_process;
void loop() {
	LPSTR cur_dir = (char *)calloc(100, sizeof(char));
	char *line_input = (char *)calloc(100, sizeof(char));
	char **line_split;
	cout << "\n\n---------------------------------------------------------------------------------------------------\n";
	cout << "--------------------------------------Here comes the Shell ---------------------------------------\n";
	cout << "---------------------------------------------------------------------------------------------------\n\n";
	while (true) {
		DWORD len_dir = GetCurrentDirectory(100, cur_dir); 	 // Take the length of the currentDirectory string, and the pointer cur_dir points to the CurrentDirectory
		cout << "(AquaShell)" << cur_dir << ">";
		cin.getline(line_input, 100);			 // line_input: Take input from the users
		cin.clear();
		if (strcmp(line_input, "\0") == 0) {				 // enter => continue the loop		
			cout << "\n";
			continue;
		}
		line_split = SplitLine(line_input);	 // Split input received from the keyboard into tokens, separated by " "
		BuiltInCommand(line_split, cur_dir); // Analyse input and choose the command (help, exit, ps...)
	}
	delete line_input;
	delete line_split;
	delete cur_dir;
}
int main(int argc, char **argv) {
	loop();
	return 0;
}
