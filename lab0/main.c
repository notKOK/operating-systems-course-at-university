#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

#define true 1
#define false 0
#define bool int

int main(int argc, char** argv)
{ 
	//printf("N of args %d, string[1]: %s \n\n\n", argc, argv[1]);
	DIR *dir;
	struct dirent *entry;
	char defaultDirway[] = "./";
	char *dirway = &defaultDirway[0];
	int c;
	bool lBit = false;
	if(argc == 2) 
	{
		if((c = getopt(argc,argv, "l") != -1)){
			lBit=true;
		} else {
			dirway = &argv[1][0];
		}
	}; 
	if(argc >= 2) {
		while((c = getopt(argc, argv, ":l:")) != -1){
			switch(c)
			{
				case ':':
					{
						dirway = &argv[1][0];
						printf(": CASE \n\n\n");
					}
				case 'l':
					{
						lBit = true;
						if(optarg != NULL){
							dirway = optarg;
						}
						break;
					}
				default:
					{	
						//printf("VERY HELPFUL MESSAGE %s \n\n", dirway);
						break;
					}

			};
		};
	};
	dir = opendir(dirway);
	if (!dir) {
		perror("diropen");
		exit(1);
	};

	//printf("dirway: %s \n\n\n\n", dirway);
	void defaultls(){		
		if ( (entry = readdir(dir)) != NULL) {
			if(entry->d_name[0] != '.'){
				char str[30];
				strcpy(str, entry->d_name);
				defaultls();
				printf("%s  ", str);
			} 
			else {
				defaultls();
			};
		};
	};

	DIR *dirtot;
	struct dirent *tot;
	int totalcount = 0;
	struct stat totinf;
	dirtot = opendir(dirway);
	while((tot = readdir(dirtot)) != NULL) {
		char fpath[200];
		snprintf(fpath,200, "%s%s", dirway, tot->d_name);
		lstat(fpath, &totinf);
		totalcount += totinf.st_blocks;
	};
	totalcount /= 2;
	closedir(dirtot);


	struct stat file_info;

	void lsl(){
		if ( (entry = readdir(dir)) != NULL ) {
			if(entry->d_name[0] != '.'){
				char full_path[260];
				snprintf(full_path, 260, "%s%s", dirway, entry->d_name);			
				lstat(full_path, &file_info);
				printf((file_info.st_mode & S_IFDIR)?"d":"-");
				printf((file_info.st_mode & S_IRUSR)?"r":"-");
				printf((file_info.st_mode & S_IWUSR)?"w":"-");
				printf((file_info.st_mode & S_IXUSR)?"x":"-");
				printf((file_info.st_mode & S_IRGRP)?"r":"-");
				printf((file_info.st_mode & S_IWGRP)?"w":"-");
				printf((file_info.st_mode & S_IXGRP)?"x":"-");
				printf((file_info.st_mode & S_IROTH)?"r":"-");
				printf((file_info.st_mode & S_IWOTH)?"w":"-");
				printf((file_info.st_mode & S_IXOTH)?"x":"-");
				printf(" %2hu", file_info.st_nlink);

				struct passwd * pwd;
				pwd = getpwuid(file_info.st_uid);
				if(&(pwd->pw_name) == NULL){
					printf(" %20d", file_info.st_uid);
				} else { printf(" %-20.20s", pwd->pw_name); };

				struct group * grp;
				grp = getgrgid(file_info.st_gid);
				printf(" %8s", grp->gr_name);

				printf(" %5d", file_info.st_size);

				char *wanted = ctime(&file_info.st_mtime);
				char *result[5];
				memcpy(result, &wanted[4], 12);
				printf(" %12.12s", result);
				printf(" %s", entry->d_name);
				printf("\n");
				lsl();
			}
			else {
				lsl();
			};		
		};	
	};
	if(lBit){
		printf("total %d\n", totalcount);
		lsl();
	} else {
		defaultls();
	}

	printf("\n");
	closedir(dir);
	return 0;
}
