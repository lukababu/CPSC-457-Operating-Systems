#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define numberOfThread 2

void * print(void * tid)
{
	if(tid==0)
		for(int j=0; j<=10; j+=2)
		{
			printf("Thread %d : \n", tid);
			printf("%d\n", j);
		}
	else
		for(int j=1; j<=10; j+=2)
		{
			printf("Thread %d : \n", tid);
			printf("%d\n", j);
		}
	pthread_exit(0);
}

int main()
{
	pthread_t myThreads[numberOfThread];
	int status, i;

	for(i=1; i<=numberOfThread;i++)
	{
		printf("Creating thread #%d\n", i);
		status = pthread_create(&myThreads[i],NULL,print,(void *) i);
	}
}
