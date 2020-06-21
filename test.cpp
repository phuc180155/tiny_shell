
#include <iostream>
#include <unistd.h>


using namespace std;
void sleep() {
	for (int i = 0; i<50; i++) {
		cout << i << endl;
		sleep(1);
	}
}

int main() {
	void (*p[]) () = {sleep, sleep};
	(*p[0])();
}
