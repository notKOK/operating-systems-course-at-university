#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

int main(int argc, char** argv) {
	int pipe_des[2];
	pid_t pid;
	int status;
	pipe(pipe_des);
	pid = fork();

	if (pid < 0)
	{
		printf("Error fork();\n");
		return -1;
	}
	else if (pid == 0)
	{	
		
		printf("CHILD:\n");
		close(pipe_des[1]);

		time_t timech;
		pid_t pidch;
		read(pipe_des[0], &timech, sizeof(time_t));
		read(pipe_des[0], &pidch, sizeof(pid_t));

		close(pipe_des[0]);

		sleep(5);

		time_t rawtime;
		time(&rawtime);
		printf("CHILD: Current time: %d h %d min %d sec\n",
			localtime(&rawtime)->tm_hour, localtime(&rawtime)->tm_min,
		       	localtime(&rawtime)->tm_sec); 
		printf("CHILD: Sender time: %d h %d min %d sec \nCHILD: Sender pid %d\n",
			localtime(&timech)->tm_hour, localtime(&timech)->tm_min,
	       		localtime(&timech)->tm_sec, pidch);

		return 0;
	}
	else
	{	
		printf("PARENT:\n");

		time_t sender_rawtime;
		time(&sender_rawtime);

		pid_t send_pid = getpid();

		close(pipe_des[0]);

		write(pipe_des[1], (void*)(&sender_rawtime), sizeof(time_t));
		write(pipe_des[1], (void*)(&send_pid), sizeof(pid_t));
	       	close(pipe_des[1]);
		printf("PARENT: Sended data to child.\n");
		wait(&status);
		return 0;
	}
}
