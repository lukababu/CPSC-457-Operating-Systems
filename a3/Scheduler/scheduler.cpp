/************************************************************************************
 * Last Name:   Iremadze
 * First Name:  Luke
 * Student ID:  10163614
 * Course:      CPSC 457
 * Tutorial:    T04
 * Assignment:  3
 * Question:    Q7
 * File name:	scheduler.c
 *
 * Compile:	gcc scheduler.c -o scheduler
 * Execute: ./scheduler config.txt rr 1
 * Argument[1]: File name
 * Argument[2]: The scheduling algorithm can be SJF or RR
 * Argument[3]: The quantum time only for RR
 *
*************************************************************************************/

/** Todo
[x] URGENT - add finish time to queue tuple
[x] Remove magic number 100
[x] Waiting times
[-] Sort by incomming time
*/

// Imports
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <tuple>

// Global Constants
#define DEBUG 0
#define MAXPROCESSES 100

// Global Variables
//int maxTime;

// Methods
void displayQueue(std::queue <std::tuple<int, int, int, int, int>> pqueue, int ganntChart[], int gcSize);
void doSJF(std::queue <std::tuple<int, int, int, int, int>> pqueue);
void doRR(std::queue <std::tuple<int, int, int, int, int>> pqueue, int qTime);
int calculateMaxTime(std::queue <std::tuple<int, int, int, int, int>> pqueue);
std::queue <std::tuple<int, int, int, int, int>> findSJ(std::queue <std::tuple<int, int, int, int, int>> pqueue);
std::queue <std::tuple<int, int, int, int, int>> setTimes(std::queue <std::tuple<int, int, int, int, int>> pqueue, int pID, int startTime, int finishTime);

/**
 * Argument[1]:  (String) The configuration file
 * Argument[2]:  (String) Algorithm to use for scheduling
 * Argument[3]:  (int) If RR algorithm then set quantum time, else throw an error
 */
int main(int argc, char *argv[]) {
	int a, b;                           // Used as a buffer holder for inputing arguments from text file
	std::ifstream infile;               // Holder for input file
	std::string algorithm;              // Algorithm being used RR of SJF
	std::tuple <int, int, int, int, int> process; // Holds information about each individual process, arg1 is Pid, arg2 is arrival time, & arg3 burst time
	std::queue <std::tuple<int, int, int, int, int>> pQueue;            // Store incomming processes in a queue
	int qTime;                          // Quantum time in case RR is used

	// Process command line arguments
	if (argc == 3) {
		algorithm = argv[2];
		if ((algorithm.compare("SJF") == 0) || (algorithm.compare("sjf") == 0)) {
			if (DEBUG == 1)
				std::cout << "Running in" << algorithm << "mode" << std::endl;
		}
		else {
			std::cerr << "Unknown scheduling algorithm \"" << algorithm <<
				"\" or inccorect usage (RR must be followed by quantum time)" << std::endl;
			exit(-1);
		}
	}
	else if (argc == 4) {
		algorithm = argv[2];
		if ((algorithm.compare("RR") == 0) || (algorithm.compare("rr") == 0)) {
			qTime = atoi(argv[3]);
			if (qTime <= 0) {
				std::cerr << "Quantum Time must be > 0. Check third argument for possible invalid entry" << std::endl;
				exit(-1);
			}

			if (DEBUG == 1)
				std::cout << "Running in" << algorithm << "mode with quantum time: " << qTime << std::endl;
		}
		else {
			std::cerr << "Unknown scheduling algorithm \"" << algorithm <<
				"\" or inccorect usage (There should be no arguments past SJF)" << std::endl;
			exit(-1);
		}
	}
	else {
		std::cerr << "Incorrect number of command line arguments. Must be 2 or 3" << std::endl;
		exit(-1);
	}

	infile.open(argv[1]);
	if (infile.fail()) {
		std::cerr << "Error opeing a file. Make sure file name is correct and check permissions." << std::endl;
		infile.close();
		exit(1);
	}

	if (DEBUG == 1)
		std::cout << "Inputing file and outputting content:\n" << "Process\t|Arrival\t|\tBurst\n";
	int pID = 0;
	while (infile >> a >> b) {
		process = std::make_tuple(pID, a, b, 0, 0);
		pQueue.push(process);
		std::get<0>(process) = 0;
		std::get<1>(process) = 0;
		std::get<2>(process) = 0;
		++pID;
	}

	if (argc == 4)
		doRR(pQueue, qTime);
	else
		doSJF(pQueue); 

	if (DEBUG == 1) {
		std::cout << "Size of pQueue: " << pQueue.size() << std::endl;
	}

	return 1;
}

/**
 * The function takes in the queue and add all the burst times together
 * plus the arrival time of the first process to calculate maxTime of completion.
 */
int calculateMaxTime(std::queue <std::tuple<int, int, int, int, int>> pqueue) {
	std::queue <std::tuple<int, int, int, int, int>> buffer = pqueue;
	int timeCount = 0;

	timeCount += std::get<1>(buffer.front()); // Get the arrival time of only the first income process
	timeCount += std::get<2>(buffer.front()); // and its burst time
	buffer.pop(); // remove the first process so that we don't recount it

	for (int i = 0; i <= buffer.size(); i++) {
		timeCount += std::get<2>(buffer.front());
	}
	if (DEBUG == 1)
		std::cout << "Max time is: " << timeCount << std::endl;
	return timeCount;
}

std::queue <std::tuple<int, int, int, int, int>> findSJ(std::queue <std::tuple<int, int, int, int, int>> pqueue) {
	std::queue <std::tuple<int, int, int, int, int>> buffer, a, b;
	int minJob;
	// Find the smallest job in our current queue and put it in front
	buffer = pqueue;
	b.push(buffer.front()); // b queue will only hold the shortest job in current queue
	minJob = std::get<2>(b.front());
	buffer.pop();
	for (int i = 0; i < buffer.size(); i++) {
		if (std::get<2>(buffer.front()) < minJob) {
			a.push(b.front());
			b.pop();
			b.push(buffer.front());
			minJob = std::get<2>(b.front());
		}
		else {
			a.push(buffer.front());
		}
		buffer.pop();
	}
	for (int i = 0; i < a.size(); i++) {
		b.push(a.front());
		a.pop();
	}
	return b; // Update current queue
}

std::queue <std::tuple<int, int, int, int, int>> setTimes(std::queue <std::tuple<int, int, int, int, int>> pqueue, int pID, int startTime, int finishTime) {
	std::queue <std::tuple<int, int, int, int, int>> buffer, a, b;
	buffer = pqueue;

	// Find the process with the passed pID
	while(!buffer.empty()) {
		if (std::get<0>(buffer.front()) == pID) {
			std::get<3>(buffer.front()) = startTime;
			std::get<4>(buffer.front()) = finishTime;
			a.push(buffer.front());
			buffer.pop();
		}
		else {
			a.push(buffer.front());
			buffer.pop();
		}
	}

	return a; // Update current queue
}

void doRR(std::queue <std::tuple<int, int, int, int, int>> pqueue, int qTime) {
	std::queue <std::tuple<int, int, int, int, int>> buffer, remaining, currentQueue, a, b;
	int maxTime = calculateMaxTime(pqueue);
	int startTime, finishTime, realEnd, currentTime;
	// Create an array to abstract a Gannt chart and initialize it with value -1
	int* ganntChart = new int[maxTime];
	for (int i = 0; i < maxTime; i++)
		ganntChart[i] = -1;
	// Keeps track of process that have entered or passed our ready queue
	bool passed[MAXPROCESSES] = { }; // mark Pid
	bool premature = false; // trigger for a process finishing before the quantum time has fully elapsed

	// Our base case, initial processes
	buffer = pqueue;
	currentQueue.push(buffer.front()); // Automatically add the first one due to FCFS rule
	passed[std::get<0>(currentQueue.front())] = true;
	buffer.pop();
	remaining = buffer;
	startTime = std::get<1>(currentQueue.front()); // start time is the arrival of the first process
	finishTime = 0;
	currentTime = startTime; 


	while (1) {
		premature = false; // Reset trigger

		// Execute the process for the quantum time
		for (int i = currentTime; i < currentTime + qTime; i++) {
			if (std::get<2>(currentQueue.front()) == 0) {
				pqueue = setTimes (pqueue, std::get<0>(currentQueue.front()), 0, i);
				currentTime = i;
				premature = true;
				break;
			}
			if (!currentQueue.empty()) {
				ganntChart[i] = std::get<0>(currentQueue.front());
				--std::get<2>(currentQueue.front());
			}
		}
		if(!premature) {
			currentTime += qTime;
			if (std::get<2>(currentQueue.front()) == 0)
				pqueue = setTimes(pqueue, std::get<0>(currentQueue.front()), 0, currentTime);
		}

		// Has anything arrived while previous process was running? If so add it to our queue
		buffer = remaining;
		while(!buffer.empty()) {
			if (std::get<1>(buffer.front()) <= currentTime) {
				if (passed[std::get<0>(buffer.front())] != true) {
					currentQueue.push(buffer.front()); // add the jobs that arrived durring the execution of last process
					passed[std::get<0>(buffer.front())] = true;
				}
			}
			buffer.pop(); // remove to itterate through the queue
		}

		// itterate the round
		if (std::get<2>(currentQueue.front()) == 0) {
			currentQueue.pop();
		}
		else {
			b.push(currentQueue.front());
			currentQueue.pop();
			currentQueue.push(b.front());
			b.pop();
		}

		// Check completion
		if (currentQueue.empty())
			break;
	}

	for (int i = maxTime - 1; i >= 0; i--) {
		if (ganntChart[i] > -1) {
			realEnd = i + 1; // the +1 to ittarate over array for other functions
			break;
		}
	}

	displayQueue(pqueue, ganntChart, realEnd);

	//deallocate ganntChart
	delete[] ganntChart;
}


void doSJF(std::queue <std::tuple<int, int, int, int, int>> pqueue) {
	std::queue <std::tuple<int, int, int, int, int>> buffer, currentQueue, a, b;
	int maxTime = calculateMaxTime(pqueue);
	int startTime, finishTime, realEnd;
	// Create an array to abstract a Gannt chart and initialize it with value -1
	int* ganntChart = new int[maxTime];
	for (int i = 0; i < maxTime; i++)
		ganntChart[i] = -1;
	// Keeps track of process that have entered or passed our ready queue
	bool passed[MAXPROCESSES] = { }; // mark Pid

	// Our base case, initial processes
	buffer = pqueue;
	currentQueue.push(buffer.front()); // Automatically add the first one due to FCFS rule
	passed[std::get<0>(currentQueue.front())] = true;
	buffer.pop();
	startTime = std::get<1>(currentQueue.front()); // start time is the arrival of the first process
	finishTime = 0;

	while (!currentQueue.empty()) {
		if (std::get<1>(currentQueue.front()) > finishTime)
			startTime = std::get<1>(currentQueue.front());
		else
			startTime = finishTime;

		// Set the finish time of process
		finishTime = startTime + std::get<2>(currentQueue.front());
		pqueue = setTimes (pqueue, std::get<0>(currentQueue.front()), startTime, finishTime);

		for (int i = startTime; i < finishTime; i++) {
			ganntChart[i] = std::get<0>(currentQueue.front());
		}

		// Has anything arrived while previous process was running? If so add it to our queue
		buffer = pqueue;
		while(!buffer.empty()) {
			if (std::get<1>(buffer.front()) < finishTime) {
				if (passed[std::get<0>(buffer.front())] != true) {
					currentQueue.push(buffer.front()); // add the jobs that arrived durring the execution of last process
					passed[std::get<0>(buffer.front())] = true;
				}
			}
			buffer.pop(); // remove to itterate through the queue
		}

		//done.push(currentQueue);
		currentQueue.pop(); // remove our completed task from our current queue


		if (!currentQueue.empty())
			currentQueue = findSJ(currentQueue); // Update current queue
		else
			break;
	}

	for (int i = maxTime - 1; i >= 0; i--) {
		if (ganntChart[i] > -1) {
			realEnd = i + 1; // the +1 to ittarate over array for other functions
			break;
		}
	}

	displayQueue(pqueue, ganntChart, realEnd);

	//deallocate ganntChart
	//for(int i = 0; i < maxTime; i++)
	delete[] ganntChart;

}

/**
 * Displays a scalable output to the console based on input
 */
void displayQueue(std::queue <std::tuple<int, int, int, int, int>> pqueue, int ganntChart[], int gcSize) {
	std::queue <std::tuple<int, int, int, int, int>> buffer = pqueue;
	float waitTime = 0.0;
	float averageWaitTime = 0.0;

	if (DEBUG == 1) {
		while (!buffer.empty()) {
			std::cout << std::get<0>(buffer.front()) << " " <<
				std::get<1>(buffer.front()) << " " <<
				std::get<2>(buffer.front()) << std::endl;
			buffer.pop();
		}
	}

	buffer = pqueue;
	std::cout << "Time\t";
	while (!buffer.empty()) {
		std::cout << "P" << std::get<0>(buffer.front()) << "\t";
		buffer.pop();
	}
	std::cout << "\n";
	for (int i = 0; i <= pqueue.size(); i++) {
		std::cout << "-------";
	}
	std::cout << std::endl;

	buffer = pqueue;

	// Print Gannt Chart
	for (int time = 0; time < gcSize; time++) {
		std::cout << time << "\t";
		//std::cout << "\t";
		while (!buffer.empty()) {
			//std::cout << "Finish time for P" << std::get<0>(buffer.front()) << " is: " << std::get<4>(buffer.front()) << std::endl;

			if (std::get<0>(buffer.front()) == ganntChart[time]) {
				std::cout << "." << "\t";
				//--std::get<2>(buffer.front());
			}
			else if ((std::get<1>(buffer.front()) <= time) && (std::get<4>(buffer.front()) > time)) { // compares current position on the gannt chart to the arrival time of process
				std::cout << "+" << "\t";
			}
			else {
				std::cout << "\t";
			}
			buffer.pop();
		}
		std::cout << std::endl;
		buffer = pqueue;
	}

	// Print average waiting times
	for (int i = 0; i <= pqueue.size(); i++) {
		std::cout << "-------";
	}
	std::cout << std::endl;
	buffer = pqueue;
	while (!buffer.empty()) {
		// wait is finish - (arrival + burst)
		waitTime = std::get<4>(buffer.front()) - (std::get<1>(buffer.front()) + std::get<2>(buffer.front()));
		averageWaitTime += waitTime;
		std::cout << "P" << std::get<0>(buffer.front()) << "\t" << "waited " << waitTime << ".000 sec." << std::endl;
		buffer.pop();
	}
	averageWaitTime /= pqueue.size();
	printf("average waiting time = %.3f\n", averageWaitTime); 
}