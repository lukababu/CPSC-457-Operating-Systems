/**********************************************
 * Last Name:   Iremadze
 * First Name:  Luke
 * Student ID:  10163614
 * Course:      CPSC 457
 * Tutorial:    T02
 * Assignment:  2
 * Question:    Q3
 *
 * File name: scan.c
 *********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FNAME_SIZE 512
#define MAX_FILES 1024

typedef struct file
{
    char name[MAX_FNAME_SIZE];
    int size;
} File;

int comparator(const void *a, const void *b)
{
  struct file *c = (struct file*) a;
  struct file *d = (struct file*) b;
  if(c->size < d->size) return 1;

  return 0;
}

void print_struct(File *struct_ptr, int size)
{
    for(int i = 0; i < size; i++) {
        printf("\t%s : %ld\n", struct_ptr[i].name , struct_ptr[i].size);
    }
}

int main( int argc, char ** argv) {
    char reFiles[strlen(argv[1])];
    char * wildcard = "*";
    int headCount;
    char *head;

  // handle command line arguments
  if( argc > 3 || argc < 1) {
    fprintf(stderr, "Incorrect number of command line variables\n");
    exit(-1);
  }

  head = argv[2];
  headCount = atoi(head);

  // open myFind
  FILE * fp = popen("find . -type f", "r");
  if( fp == NULL) {
    perror("popen failed:");
    exit(-1);
  }

  // read in all filenames
  char buff[MAX_FNAME_SIZE];
  int nFiles = 0;
  char * files[MAX_FILES];
  while(fgets(buff,MAX_FNAME_SIZE,fp)) {
    int len = strlen(buff) - 1;
    files[nFiles] = strndup(buff,len);
    nFiles ++;
  }
  fclose(fp);
  printf("Found %d files:\n", nFiles);

  //Initialize typdef
  struct file fileDB[nFiles];

  // get file sizes for each file and sum them up
  
  long long totalSize = 0;
  struct stat st;
  for(int i = 0 ; i < nFiles ; i ++ ) {
    if( 0 != stat(files[i],&st)) {
      perror("stat failed:");
      exit(-1);
    }
    
    /*
     *Attempting to implement wildcard checkpoint
    printf("The argument is %s\n", argv[1]);
    if(strcmp(argv[1], wildcard) != 0) 
    */

    // Filter based on first argument
    for(int j = strlen(files[i])-1, k = strlen(argv[1])-1; k>-1; j--, k--) {
        reFiles[k] = files[i][j];
    }

    if(strcmp(argv[1], reFiles) == 0) {
        strcpy(fileDB[i].name, files[i]);
        fileDB[i].size = st.st_size;

        totalSize += fileDB[i].size;
    }
  }
  qsort(fileDB, nFiles, sizeof(File), comparator);
  print_struct(fileDB, headCount);
  printf("Total size: %lld bytes.\n", totalSize);

  // clean up
  for(int i = 0; i < nFiles ; i ++ ) {
    free(files[i]);
  }
  

  return 0;
}