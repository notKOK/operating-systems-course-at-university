#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h>

void exitfunc()
{
	printf("hey vsause atexit() here  pid=%d\n", getpid());
	return;
};	

int main() { 
	pid_t pid = fork(); 
	atexit(exitfunc);
	switch(pid) { 
		case -1:
			perror("fork");  /* произошла ошибка */
			exit(1); /*выход из родительского процесса*/ 
		case 0: 
			printf(" CHILD: Это процесс-потомок!\n");
			printf(" CHILD: Мой PID -- %d\n", getpid()); 
			printf(" CHILD: PID моего родителя -- %d\n", getppid()); 
			printf(" CHILD: Выход!\n"); 
			exit(0); 
		default: 
			printf("PARENT: Это процесс-родитель!\n");
			printf("PARENT: Мой PID -- %d\n", getpid()); 
			printf("PARENT: PID моего потомка %d\n",pid); 	
			printf("PARENT: Я жду, пока потомок не вызовет exit()...\n");
		       	wait(NULL); 
			printf("PARENT: Код возврата потомка:%d\n", WEXITSTATUS(0)); 
			printf("PARENT: Выход!\n"); 
				} 
	return 0;
};
