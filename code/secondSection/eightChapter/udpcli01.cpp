#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 1024
#define SERV_PORT 10005

void dg_cli(FILE* fp,int sockfd,struct sockaddr* pservaddr,socklen_t servlen){
	int n;
	char sendline[MAX_LEN],recvline[MAX_LEN];
	while(fgets(sendline,MAX_LEN,fp)!=NULL){
		sendto(sockfd,sendline,strlen(sendline),0,pservaddr,servlen);
		n=recvfrom(sockfd,recvline,MAX_LEN,0,NULL,NULL);
		fputs(recvline,stdout);
	}
}


int main(int argc,char** argv){
	if(argc!=2){
		std::cout<<"input error ,you should input ./udpcli01 ipaddr"<<std::endl;
		return -1;
	}
	int sockfd;
	struct sockaddr_in servaddr,cliaddr;

	memset(&servaddr,0,sizeof(servaddr));

	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(SERV_PORT);
	inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
	
	cliaddr.sin_family=AF_INET;
	cliaddr.sin_port=10006;

	sockfd=socket(AF_INET,SOCK_DGRAM,0);
//	bind(sockfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));

	dg_cli(stdin,sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	exit(0);
	return 0;
}

