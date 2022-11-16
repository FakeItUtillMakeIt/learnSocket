
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h>

#include "tcp_listen.h"

#define MAXLINE 1024

static void* doit(void*);
void str_echo(int sockfd);

int main(int argc,char** argv){
	int listenfd,*connfd;
	pthread_t tid;
	socklen_t addrlen,len;
	struct sockaddr* cliaddr;

	if(argc==2){
		listenfd=tcp_listen(NULL,argv[1],&addrlen);
	}
	else if(argc==3){
		listenfd=tcp_listen(argv[1],argv[2],&addrlen);
	}
	else{
		std::cout<<"usage:tcpservthread01 [<host>|<service|port>]"<<std::endl;
		return -1;
	}

	cliaddr=(sockaddr*) malloc(addrlen);
	while(true){
		len=addrlen;
		*connfd=accept(listenfd,cliaddr,&len);
		pthread_create(&tid,NULL,&doit,connfd);
	}

	return 0;
}

static void *doit(void *arg){
	int connfd;
	connfd=*((int*)arg);
	pthread_detach(pthread_self());
	str_echo(connfd);
	close(connfd);
	return NULL;
}


void str_echo(int sockfd){
	ssize_t n;
	char buf[MAXLINE] ;

again:
	while((n=read(sockfd,buf,MAXLINE))>0){
		write(sockfd,buf,n);
	}
	if(n<0 && errno==EINTR){
		goto again;
	}
	else{
		if(n<0){
			std::cout<<"str_echo:read error"<<std::endl;
		}
	}
}
