/**********************************************
 * Last Name:   Iremadze
 * First Name:  Luke
 * Student ID:  10163614
 * Course:      CPSC 457
 * Tutorial:    T02
 * Assignment:  2
 * Question:    Q2
 *
 * File name: myFind.c
 *********************************************/

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

int main () {
    DIR *dirp;
    struct dirent *dp;
    char *dir = "";
    char firstchar[2];
    char period[2];

    period[0] = '.';
    period[1] = '\0';

    // Make sure the directory opens
    if ((dirp = opendir(".")) == NULL) {
        perror("couldn't open '.'");
        return 0;
    }

    do {
        //printf("./");
        if ((dp = readdir(dirp)) != NULL) {
            dir = dp->d_name;

            firstchar[0] = (char) dir[0];
            firstchar[1] = '\0';

            if (strcmp(firstchar, period) != 0) {
                (void) printf("./%s\n", dir);
            }
            else {
                if (strlen(dir) > 1) {
                    firstchar[0] = (char) dir[1];
                    firstchar[1] = '\0';
                    if (strcmp(firstchar, period) != 0)
                        (void) printf("./%s\n", dir);
                }
            }
                
        }
    } while (dp != NULL);

    (void) closedir(dirp);
    return 0;
}