
#include <iostream>
#include <ctime>
#include <windows.h>
#include <string>
using namespace std;
int main( )
{
 // current date/time based on current system
 time_t now = time(0);
 // convert now to string form
 char* dt = ctime(&now);
 cout << "The local date and time is: " << dt << endl;
 WIN32_FIND_DATA data;			// du lieu 1 file
 FILETIME timeLastAccess;		
 HANDLE han = FindFirstFile("C:\\Users\\ACER\\Desktop\\New folder (5)\\update shell\\Tiny_Shell-master\\OS-tiny-shell", &data);
 timeLastAccess = data.ftLastAccessTime;
 std::string s = std::to_string(timeLastAccess.dwHighDateTime);
 cout << s <<"\n";
 cout << to_string(1) <<"\n";
 cout << to_string(timeLastAccess.dwLowDateTime) << "\n";
 
}
