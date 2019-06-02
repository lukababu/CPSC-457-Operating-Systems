#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 2

void * thread_print(void * tid) {
	printf("thread %ld running\n", tid);
	pthread_exit(0);
}

int main()
{
	pthread_t threads[NUMBER_OF_THREADS];
	long status, i;
	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		printf("creating thread %d\n", i);
		status = pthread_create(&threads[i], NULL, thread_print, (void *) i);
		if (status != 0)
		{
			printf("Oops, pthread_create returned error code %d\n", status);
			exit(-1);
		}
	}

	for(i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_join(threads[i], NULL);
	exit(0);
}
