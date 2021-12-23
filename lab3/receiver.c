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

int main(){
	sleep(1);

	key_t key = ftok(BUFF_MEM, 5);
	if (key == -1) {
		printf("invalid key\n");
		return -1;
	}

	int shm_id = shmget(key, sizeof(tpid), 0666);
	if (shm_id < 0){
		printf("shmid invalid\n");
		return -1;
	}

	struct tpid* shm_ptr = shmat(shm_id, NULL, 0);
	if (shm_ptr < 0){
		printf("shmat erro\n");
		return -1;
	}

	time_t res_time = shm_ptr->time;
	pid_t res_pid = shm_ptr->pid;
	time_t temptime = time(NULL);
	printf("Receiver: time: %spid: %d\nSender: time %spid: %d\n", ctime(&res_time), res_pid, ctime(&temptime), getpid());
	shmdt(shm_ptr);
	return 0;
}
