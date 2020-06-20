#define WINVER 0x0500
#include <iostream>
#include <Windows.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <string>
#include "parse.h"
#include <string.h>
#include <signal.h>
#include <iomanip>

using namespace std;

HANDLE h_fore_process = NULL;

void GetAllProcess() {
	HANDLE h_process_snap;
	PROCESSENTRY32 pe32; 												 // 1 structure store the information of a thread
	h_process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);	 // Set the size of the structure before using it
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// Get information about the first process, and exit if unsuccessful.
	if (!Process32First(h_process_snap, &pe32))	{
		cout << "Process32First failed\n"; 
		CloseHandle(h_process_snap);	   
		return;
	}
	cout << setw(30) << left << "Process Name" << setw(20) << left << "Parent Process ID" << setw(15) << left << "Process ID" << setw(15) << left << "Number of threads" <<"\n";
	cout << "------------------------------------------------------------------------------------------\n";
	do	{
		cout << setw(30) << left << pe32.szExeFile << setw(20) << left << pe32.th32ParentProcessID << setw(15) << left << pe32.th32ProcessID << setw(15) << left << pe32.cntThreads << "\n";
	} 
	while (Process32Next(h_process_snap, &pe32));
	CloseHandle(h_process_snap);
	return;
}

void GetChildProcess(DWORD pid) {
	PROCESSENTRY32 pe32;
	HANDLE h_process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);	// Take a snapshot of all processes in the system.
	pe32.dwSize = sizeof(PROCESSENTRY32);										// Set the size of the structure before using it.
	// Get information about the first process, and exit if unsuccessful:
	if (!Process32First(h_process_snap, &pe32))	{
		cout << "Process32First fail\n"; 
		CloseHandle(h_process_snap);          
		return;
	}
	cout << setw(50) << left << "Process Name" << setw(20) <<left<<"Process ID"<<setw(20)<<left<<"Parent Process ID" <<"\n";
	cout << "---------------------------------------------------------\n";
	do	{
		if (pe32.th32ParentProcessID == pid)
			cout << setw(50) << left << pe32.szExeFile<< setw(20) <<left<<pe32.th32ProcessID<<setw(20)<<left<<pe32.th32ParentProcessID <<"\n";
	} while (Process32Next(h_process_snap, &pe32));
	CloseHandle(h_process_snap);
	return;
}

void SuspendProcess(DWORD pid) {
	HANDLE h_thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);	// Take a snapshot of all threads in the process.
	THREADENTRY32 th32; 														// 1 structure store the information of a thread
	HANDLE h_thread;
	th32.dwSize = sizeof(THREADENTRY32);										// Set the size of the structure before using it.
	// Get information about the first thread, and exit if it unsuccessful.
	if (!Thread32First(h_thread_snap, &th32))	{
		cout << "Thread32First failed\n"; 
		CloseHandle(h_thread_snap);		 
		return;
	}
	// Get all the threads of process and call the SuspendThread function to stop each one
	do	{
		if (th32.th32OwnerProcessID == pid)	{
			h_thread = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID);
			if (SuspendThread(h_thread) == -1) 			//	DWORD SuspendThread(HANDLE hThread): return -1 if fails,else return previous suspend count of the thread
				return;
		}
	} 	while (Thread32Next(h_thread_snap, &th32));
	CloseHandle(h_thread_snap);
	return;
}
void ResumeProcess(DWORD pid) {
	HANDLE h_thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid); 		// Take a snapshot of all threads in the process.
	THREADENTRY32 th32;
	HANDLE h_thread;
	th32.dwSize = sizeof(THREADENTRY32);											// Set the size of the structure before using it.
	// Get information about the first thread, and exit if it unsuccessful.
	if (!Thread32First(h_thread_snap, &th32)) {
		cout << "Thread32First failed\n";
		CloseHandle(h_thread_snap);
		return;
	}
	// Get all the threads of process and resume each one
	do	{
		if (th32.th32OwnerProcessID == pid)	{
			HANDLE h_thread = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID);
			if (ResumeThread(h_thread) == -1) // DWORD ResumeThread(HANDLE h_thread): decrease suspend's count. if (suspend count == 0), thread is resume.
				return;
		}
	} while (Thread32Next(h_thread_snap, &th32));
	CloseHandle(h_thread_snap);
	return;
}

void KillProcess(DWORD pid) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); // open a process by its pid
	if (hProcess == NULL)	{
		cout << "Failed\n";
		return;
	}
	if (TerminateProcess(hProcess, 1) == 0) // BOOL TerminateProcess(HANDLE hProcess, UINT   uExitCode: return 0 if fails
		cout << "Failed.\n";
	return;
}

void handleSigint(int sigInt) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, sigInt);
	TerminateProcess(hProcess, 1);
	CloseHandle(hProcess);
	return;
}

void CreateNewProcess(char **argv) {
	int wait_time;
	if (argv[2] == NULL) {
			cout << "Required arguments.\n";
			return;
	}	

	if (strcmp(argv[1], "-back"))
		wait_time = INFINITE;
	else
		wait_time = 0;
	char *run_file = CombineLine(argv, 2);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.lpTitle = argv[1];
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process.
	if (!CreateProcess(NULL,			   // No module name (use command line)
					   run_file,		   // Command line
					   NULL,			   // Process handle not inheritable
					   NULL,			   // Thread handle not inheritable
					   FALSE,			   // Set handle inheritance to FALSE
					   CREATE_NEW_CONSOLE, // Create new console
					   NULL,			   // Use parent's environment block
					   NULL,			   // Use parent's starting directory
					   &si,				   // Pointer to STARTUPINFO structure
					   &pi)				   // Pointer to PROCESS_INFORMATION structure
	)
	{
		cout << "Create process failed\n";
		return;
	}

	// get handle of child fore process
	if (strcmp(argv[1], "-fore") == 0)
	{
		h_fore_process = pi.hProcess;
		signal(SIGINT, handleSigint);
	}
	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, wait_time);

	// Close process and thread handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
void GetThreadList(DWORD pid) {
	HANDLE h_thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
	THREADENTRY32 th32;
	th32.dwSize = sizeof(THREADENTRY32);				// Set the size of the structure before using it.
	// Retrieve information about the first thread, and exit if it unsuccessful.
	if (!Thread32First(h_thread_snap, &th32))	{
		cout << "Thread32First Fail. \n"; 
		CloseHandle(h_thread_snap);         
		return;
	}
	cout << setw(40) << left << "Thread ID" << setw(20) << left << "OwnerProcessID" <<"\n";
	cout << "--------------------------------------------------\n";
	do	{
		if (th32.th32OwnerProcessID == pid)
			cout << setw(40) << left << th32.th32ThreadID << setw(20) << left << th32.th32OwnerProcessID <<"\n";
	} while (Thread32Next(h_thread_snap, &th32));
	CloseHandle(h_thread_snap);
	return;
}
void FindProcess(const char *process_name) {
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe32.dwSize = sizeof(PROCESSENTRY32); 
	// Get information about the first process, and exit if unsuccessful
	cout << setw(50) << left << "Process Name" << setw(20) <<left<<"Process ID"<<setw(25)<<left<<"Parent Process ID" <<setw(20)<<left<<"Number of threads\n";
	cout << "---------------------------------------------------------------------------------\n";
	if (!Process32First(hProcessSnap, &pe32)) {
		CloseHandle(hProcessSnap);          // clean the snapshot object
		cout << "Failed to get information of process. \n";
		return;
	}
	do	{
		if (strcmp(process_name, pe32.szExeFile) == 0)
			cout << setw(50) << left << pe32.szExeFile << setw(20) <<left<<pe32.th32ProcessID<<setw(25)<<left<<pe32.th32ParentProcessID<<setw(20)<<left<<pe32.cntThreads<<"\n";
	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	return;
}
