#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_LEN 1024
#define SERV_PORT 10005

void dg_echo(int sockfd,struct sockaddr* pcliaddr,socklen_t clilen){
	int n;
	socklen_t len;
	char msg[MAX_LEN];
	while(true){
		len=clilen;
		n=recvfrom(sockfd,msg,MAX_LEN,0,pcliaddr,&len);

		sendto(sockfd,msg,n,0,pcliaddr,len);
	}
}


int main(int argc,char** argv){

	int sockfd;
	struct sockaddr_in servaddr,cliaddr;

	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	memset(&servaddr,0,sizeof(servaddr));

	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(SERV_PORT);

	bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

	dg_echo(sockfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
}

