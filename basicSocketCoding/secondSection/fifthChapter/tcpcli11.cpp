#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <time.h>

void str_cli(FILE* fd,int sockfd){
	char sendline[1024],recvline[1024];
	while(fgets(sendline,1024,fd)!=NULL){
		write(sockfd,sendline,strlen(sendline));
		sleep(1);
		write(sockfd,sendline,strlen(sendline));
		if(read(sockfd,recvline,1024)==0){
			std::cout<<"str_cli:server terminated prematurely"<<std::endl;
		}
		fputs(recvline,stdout);
	}
}

int main(int argc,char** argv){
	int sockfd;
	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	if(argc!=2){
		std::cout<<"usage:tcpcli <IPaddress>"<<std::endl;
		return -1;
	}	
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(10003);
	if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr.s_addr)<0){
		std::cout<<"error in inet_pton"<<std::endl;
		return -1;
	}
	
	if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		std::cout<<"error in connect"<<std::endl;
		return -1;
	}
	std::cout<<"connect success!"<<std::endl;	
	str_cli(stdin,sockfd);
	exit(0);	

	return 0;
}
