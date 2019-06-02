#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
using namespace std;

int main(int argc, char * const argv[]) {
	string filename;
	filename = argv[1];

	// open the file for read
	int fd = open(filename.c_str(), O_RDONLY);

	// read file character by character and count lines
	char c[5000000];
	cout << read(fd, &c, 5000000) << endl;

	return 0;
}
