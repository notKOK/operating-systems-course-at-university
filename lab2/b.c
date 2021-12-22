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
	pid_t pid;
	int status;
	pid = fork();

	if (pid < 0)
	{
		printf("Error fork();\n");
		return -1;
	}
	else if (pid == 0)
	{	
		
		printf("CHILD:\n");
		int fifo_read = open("temp_fifo", O_RDONLY);
		
		if (fifo_read < 0) {
			printf("CHILD: Error read fifo\n");		
			return -1;
		}
		time_t timech;
		pid_t pidch;
		read(fifo_read, &timech, sizeof(time_t));
		read(fifo_read, &pidch, sizeof(pid_t));
		close(fifo_read);

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
		int fifo;
		
		if (access("temp_fifo", F_OK) == -1) {
			fifo = mkfifo("temp_fifo", 0777);
		}
		
		int fifo_write = open("temp_fifo", O_WRONLY);

		if (fifo_write < 0) {
			printf("PARENT Error write fifo\n");
			return -1;
		}
		time_t sender_rawtime;
		time(&sender_rawtime);

		pid_t send_pid = getpid();

		write(fifo_write, (void*)(&sender_rawtime), sizeof(time_t));
		write(fifo_write, (void*)(&send_pid), sizeof(pid_t));
	       	close(fifo_write);
		printf("PARENT: Sended data to child.\n");
		wait(&status);
		return 0;
	}
}
