#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <time.h>
#include <fcntl.h>

#define BUFF_MEM "buff"

typedef struct tpid
{
		time_t time;
		pid_t pid;
} tpid;

int main() {

	key_t key = ftok(BUFF_MEM, 5);

	if (key == -1) {
		printf("key error\n");
		return -1;
	}
	
	int shm_id = shmget(key, sizeof(struct tpid), IPC_CREAT | 0666);
	printf("%d shmid", shm_id);
	if (shm_id < 0){
		printf("shm_id invalid\n");
		return -1;
	}
	
	time_t send_time = time(NULL);
	int send_pid = getpid();
	tpid to_send = {send_time, send_pid};

	struct shmid_ds shm_buf;
	shmctl(shm_id, IPC_STAT, &shm_buf); // IPC_SET?
	
	if (shm_buf.shm_nattch >= 1){
		printf("sender already exists\n");
		return -1;
	}
	
	struct tpid* shm_ptr;
	if ((shm_ptr = shmat(shm_id, NULL, 0)) < 0){
		printf("shmat error\n");
		return -1;
	}

	while(1){
		to_send.time = time(NULL);
		to_send.pid = getpid();
		*shm_ptr = to_send;
		sleep(1);;
	}
	shmdt(shm_ptr);
	return 0;
}
