#include <iostream>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <Windows.h>

using namespace std;

char **SplitLine(char *buf) {
	int position = 0;
	char **tokens = (char **)calloc(1000, sizeof(char *));
	char *token;
	if (tokens == NULL)	{
		cout << " Memory allocation failed. \n";
		exit(EXIT_FAILURE);
	}
	token = strtok(buf, " "); // chia xau thanh cac token boi " ": Lay token dau tien
	while (token != NULL) {
		tokens[position] = token;
		position++;
		token = strtok(NULL, " ");
	}
	tokens[position] = NULL;
	return tokens; 	// tra ve 1 chuoi con tro tro toi 1 mang cac string da duoc chia cua lenh
}
char *Combine(char **argv, int begin) {
	char *path = (char *)calloc(100, sizeof(char));
	char *p = (char *)" ";
	path = strcat(path, argv[begin]);			// Combine argv[] and space char to path:
	int i = begin + 1;
	while (argv[i] != NULL)	{
		path = strcat(path, p);
		path = strcat(path, argv[i]);
		++i;
	}
	return path;
}
