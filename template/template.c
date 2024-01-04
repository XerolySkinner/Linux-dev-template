#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char *argv[]){
	int fd, retvalue;
	char *filename=argv[1];
	
	if(argc != 3){
		printf("Error Usage!\r\n");
		return -1;}
	
	fd = open(filename, O_RDWR);
	if(fd < 0){
		printf("Can't open file %s\r\n", filename);
		return -1;}

	if(!strcmp(argv[2],"W")){
		retvalue = write(fd,"Fuckyou", 7);
		if(retvalue<0)printf("write file %s failed!\r\n", filename);}
	
	if(!strcmp(argv[2],"RW")){
		char temp[16];
		retvalue = write(fd,"Fuckyou", 7);
		if(retvalue<0)printf("write file %s failed!\r\n", filename);
		read(fd,temp,7);
		printf(temp);
		}

	retvalue = close(fd);
	if(retvalue < 0){
		printf("Can't close file %s\r\n", filename);
		return -1;}
	}
