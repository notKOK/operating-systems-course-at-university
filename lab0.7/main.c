#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#define LEN 10001

void writefunc(FILE *file);

int main(int argc, char** argv)
{
	int num;
	char buff[LEN];
	FILE *archive;
	FILE *file1;
	FILE *file2;
	archive = fopen("archive", "w");
	file1 = fopen("file1", "r");

	if((archive != NULL) && (file1 != NULL))
	{
		num = fread(buff, 1, LEN, file1);
		fwrite(buff, 1, num, archive);		
		fwrite(buff, 1, num, archive);
		
	}
	
	//writefunc(&fi);
	
	return 0;	
}

/* void writefunc(FILE *file)
{
	
	printf("OK\n");	
	return;
}*/
