#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void errif(bool condition,const char *errmsg){
	if(condition==1){
		perror(errmsg);
		exit(EXIT_FAILURE);
	}
}

