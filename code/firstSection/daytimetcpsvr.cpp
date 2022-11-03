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

	if(connfd=accept(sockfd,(struct sockaddr*)NULL,NULL))
	{
		while(true){
	
			//scanf("svr input:%s",&recvline);
			std::cout<<"client input:";
//			std::cin>>recvline;
			time_t ticks=time(NULL);
			snprintf(recvline,sizeof(recvline),"%.24s\r\n",ctime(&ticks));			
			send(connfd,recvline,sizeof(recvline),0);
			recv(connfd,recvline,1024,0);
			printf("%s\n",recvline);
//		send(connfd,recvline,1024,0);		
		}
}
	return 0;
}
