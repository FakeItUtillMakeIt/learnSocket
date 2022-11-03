#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <sys/select.h>
#include <math.h>

//using select rewrite str_cli
void str_cli(FILE* fd,int sockfd){
	int maxfdp1;
	fd_set readset;
	char sendline[1024],recvline[1024];

	FD_ZERO(&readset);
	while(true){
		FD_SET(fileno(fd),&readset);
		FD_SET(sockfd,&readset);
		maxfdp1=std::max(fileno(fd),sockfd)+1;
		select(maxfdp1,&readset,NULL,NULL,NULL);

		if(FD_ISSET(sockfd,&readset)){//socket is readable
			if(read(sockfd,recvline,1024)<0){
				std::cout<<"str_cli error:server terminated preaturely"<<std::endl;
				return;
			}
			fputs(recvline,stdout);

		}
		if(FD_ISSET(fileno(fd),&readset)){//fd is readable
			if(fgets(sendline,1024,fd)==NULL){
				return;
			}
			write(sockfd,sendline,strlen(sendline));
		}
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
