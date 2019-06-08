/************************************************************************************
 * Last Name:   Iremadze
 * First Name:  Luke
 * Student ID:  10163614
 * Course:      CPSC 457
 * Tutorial:    T02
 * Assignment:  3
 * Question:    Q5
 * File name:	count.c
 *
 * Compile:	gcc count.c -o count -lm -lpthread
 *
*************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>

#define MAX_NUMBER 10000

pthread_mutex_t process;

// Methods
void driver();
void*thread(void*num);
int isPrimeNum(int64_t i, long threadNum,int numberID);

// Variables
int nThreads;
int inputCount = 0;
int numberID;

// inputs[x][0] - The input number
// inputs[x][1] - boolean value of 1 indicating it is a prime 
// inputs[x][2] - boolean value of 1 indicating the input number was processed
// inputs[x][3] - section start number for thread processing
int64_t inputs[MAX_NUMBER][4];

int main( int argc, char ** argv) {
  int PrimeCount = 0;

  if( argc != 1 && argc != 2) {
    printf("Usage: countPrimes [nThreads]\n");
    exit(-1);
  }
  if( argc == 2) nThreads = atoi( argv[1]);

  /// handle invalid arguments
  if( nThreads < 1 || nThreads > 256) {
    printf("Bad arguments. 1 <= nThreads <= 256!\n");
    exit(-1);
  }

  printf("Counting primes using %d thread%s.\n", nThreads, nThreads == 1 ? "" : "s");

  /// Set up threads and solve
  pthread_mutex_init(&process, NULL);
  driver();
  pthread_mutex_destroy(&process);

  /// count the primes
  for(int i=0; i<inputCount; i++){
    if (inputs[i][1] == 1) PrimeCount++;
  }

  /// report results
  printf("Found %ld primes.\n", PrimeCount);
  return 0;
}

//reads all the numbers, sets things up, creates the threads
void driver() {
  long num;
  long i = 0;

  while(1) {
    if( 1 != scanf("%ld", & num)) break;
    inputs[i][0] = num;
    inputCount++;
    i++;
  }

  /// Primality test, if n is prime
  /// Check header for inputs[x][x] usage 
  for(i = 0; i < inputCount; i++) {
    int64_t max = sqrt(inputs[i][0]);

    if( inputs[i][0] <= 1) {
      inputs[i][1] = 0;
      inputs[i][2] = 1;
    } // small numbers are not primes
    else if( inputs[i][0] <= 3 ){
      inputs[i][1] = 1;
      inputs[i][2] = 1;
    } // 2 and  3 are prime
    else if( inputs[i][0] % 2 == 0 || inputs[i][0] % 3 == 0){
      inputs[i][1] = 0;
      inputs[i][2] = 1;
    } // multiples of 2 and 3 are not primes
    else {
      inputs[i][1] = 0;
      inputs[i][2] = 0;
    }

    // Set-up section start places
    inputs[i][3] = max / nThreads;
    if (nThreads != 1) {
      if (inputs[i][3] % 2 == 0) {    // Can't start at a number divisible by 2
        inputs[i][3]++;
      }
      if (inputs[i][3] % 3 == 0) {    // Can't start at div by 3
        inputs[i][3] += 2;
      }
      else if ((inputs[i][3] + 2) % 3 == 0) { // Multiples of 2, Even
        inputs[i][3] += 4;
      }
    }
  }

  //create the threads
  pthread_t threads[nThreads];
  for(int j=1; j<=nThreads; j++) {
    if(0 != pthread_create(&threads[(j-1)],NULL,thread,(void*)j)){
      printf("Oops, pthread_create failed.\n");
      exit(-1);
    }
  }

  //wait for all tje threads to finish processing
  for(int j=1; j<=nThreads; j++) {
    pthread_join(threads[(j-1)],NULL);
  }
}

void*thread(void*num) {
  int i;
	long threadNum;
  threadNum = (long)num;

  // initialize
	for(i = 0; i < inputCount; i++) {
    int result = isPrimeNum(inputs[i][0], threadNum, i);
  }

}

/**
 * Test for prime number 
 * ================================================================
 * number               Input number to be processed
 * threadNum            The created thread number      
 * numberID             The itteration in the input number data set
 * ================================================================
 * sets the inputs[X][1] as 1 if prime, else 0
 */
int  isPrimeNum(int64_t number, long threadNum, int numberID) {
  int64_t bottom = (threadNum-1)*inputs[numberID][3];

  // Makes sure it starts at > 5
	if (bottom % 2 == 0){
    bottom++;
  }		// even number
	if (bottom % 3 == 0){
    bottom +=2;
  }  // div by 3
	else if ((bottom + 2) % 3 == 0){
    bottom += 4;
  }
	if (bottom < 5){
    bottom = 5;
  }
  
	int64_t top = (threadNum)*inputs[numberID][3] + 1;

  // Process the numbers
  while( bottom <= top+1 && inputs[numberID][2] == 0) {
    if ((number % bottom == 0 || number % (bottom+2) == 0)) { //the number is not a prime
      pthread_mutex_lock(&process);
      // Check to see if it is processed
      if (inputs[numberID][2] == 0) {
        inputs[numberID][1] = 0; // Mark as not prime
        inputs[numberID][2] = 1; // Mark as processed
      }
      pthread_mutex_unlock(&process);
      return 0;
    }

    bottom += 6;
  }

  pthread_mutex_lock(&process);
  
  // Check to see if it is processed and mark the remaing inputs as prime numbers
  if (inputs[numberID][2] == 0) {
    inputs[numberID][1] = 1; // Mark as prime
    if (bottom > sqrt(number)) {
      inputs[numberID][2] = 1; // Mark as processed
    }
  }
  pthread_mutex_unlock(&process);
  return 0;
}