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

int main(int argc, char** argv)
{ 
	DIR *dir;
	struct dirent *entry;
	char *dirway = "./";
	if(argc > 1){
		if((strcmp(argv[1], "-l")) * (argv[1] != NULL)){
			dirway = argv[1];
		};
	};
	dir = opendir(dirway);
	if (!dir) {
        	perror("diropen");
        	exit(1);
	};
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
	
	struct stat file_info;

	void lsl(){
		if ( (entry = readdir(dir)) != NULL) {
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
				printf(" %-20s", pwd->pw_name);
				struct group * grp;
				grp = getgrgid(file_info.st_gid);
				printf(" %8s", grp->gr_name);
				printf(" %5d", file_info.st_size);
				char *wanted = ctime(&file_info.st_mtime);
				char *result[30];
				memcpy(result, &wanted[4], 12);
				printf(" %s", result);
				printf(" %s", entry->d_name);
				printf("\n");
				lsl();
				}
			else {
				lsl();
			};		
		};	
	};
	
	if(argc == 2) {	
		if(!strcmp(argv[1],"-l")){	
			lsl();	
		} else { defaultls(); } 
	} 
	else if(argc > 2) {
		if(!strcmp(argv[2],"-l")){
			lsl();
		} else { defaultls();};
	} else { defaultls();};
	printf("\n");
	closedir(dir);
	return 0;
}
