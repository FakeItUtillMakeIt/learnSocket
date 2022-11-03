#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <time.h>

int main(){
	
	int sockfd,n;
	char recvline[1024];
	
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
		printf("error in socket\n");
		return -1;	
}
	//socket address
	struct	sockaddr_in svrsockaddr;
	svrsockaddr.sin_family=AF_INET;
	svrsockaddr.sin_port=htons(10002);
	svrsockaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	

	if((bind(sockfd,(struct sockaddr*)&svrsockaddr,sizeof(svrsockaddr))<0)){
		printf("error in bind\n");
		return -1;
	}
	if(listen(sockfd,10)<0){
		printf("error in listen\n");
		return -1;
	}
	int connfd;
	struct sockaddr_in cli_addr;
	int len=sizeof(struct sockaddr_in);
	while(true){
		connfd=accept(sockfd,(struct sockaddr*)&cli_addr.sin_addr,(socklen_t*)&len);
		const char* buf=inet_ntop(connfd,(struct sockaddr*)&cli_addr.sin_addr,(char*)NULL,len);
		printf("client address:%s:%d\n",buf,htons(cli_addr.sin_port));
		while(connfd){
			std::cout<<"client input:";
			time_t ticks=time(NULL);
			snprintf(recvline,sizeof(recvline),"%.24s\r\n",ctime(&ticks));			
			send(connfd,recvline,sizeof(recvline),0);
			recv(connfd,recvline,1024,0);
			printf("%s\n",recvline);
		}
	}
	return 0;
}
