#include <iostream>
#include <fstream>

using namespace std;

int main() {
	int count = 0;

	fstream myfile("romeo-and-juliet.txt", ios::in);

	char buffer[100000];
	myfile.read(buffer, 100000);

	for(int i=0; i<100000; i++)
		if (buffer[i] =='\n')
			count++;
	cout << count << endl;

	return 0;
}
