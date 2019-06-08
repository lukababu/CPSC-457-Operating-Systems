/**********************************************
 * Last Name:   Iremadze
 * First Name:  Luke
 * Student ID:  10163614
 * Course:      CPSC 457
 * Tutorial:    T02
 * Assignment:  1
 * Question:    Q5
 *
 * File name: myWc.cpp
 *********************************************/

#include <unistd.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main (int argc, char * const argv[])
{
  // get the file name from command line
  string filename;
  if (argc != 2) {
    cerr << "Usage: readFile <input file> " << endl;
    return -1;
  }
  else {
    filename = argv[1];
  }

  // open the file for reading
  int fd = open( filename.c_str(), O_RDONLY);
  if( fd < 0) {
    cerr << "Could not open file " << filename << "\n";
    exit(-1);
  }

  // read file character by character and count lines
  int count = 0;
  char buffer[128] = "";
  while( read(fd, & buffer, sizeof(buffer)) > 1 ) {
    for(int i =0; i < 128; i++) {
	if( buffer[i] == '\n') {
		buffer[i] = '\0';
		count ++;
	}
	else if (buffer[i] == '\0') break;
    }
  }

  // close file and report results
  close( fd);
  cout << count << " " << filename << "\n";
  return 0;
}
