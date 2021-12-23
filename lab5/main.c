#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>


int counter = 0;
pthread_mutex_t mutex;

void* write_t(){
	while(1){
		pthread_mutex_lock(&mutex);
		counter++;
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
}

void* read_t(){
	while(1){
		pthread_mutex_lock(&mutex);
		printf("TID: %ld, count: %d\n", pthread_self(), counter);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

int main(){
	pthread_t writer, readers[10];

	pthread_mutex_init(&mutex, NULL);

	pthread_create(&writer, NULL, write_t, NULL);
	for(int i = 0; i < 10; ++i){
		pthread_create(&readers[i], NULL, read_t, NULL);
	}
	for (int i = 0; i < 10; ++i){
		pthread_join(readers[i], NULL);
	}


	pthread_mutex_destroy(&mutex);
	return 0;
}
