#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serv_addr;
	bzero(&serv_addr,sizeof(serv_addr));

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(8888);

	bind(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr));
	listen(sockfd,10);

	struct sockaddr_in client_addr;
	socklen_t client_addr_len=sizeof(client_addr);
	bzero(&client_addr,sizeof(client_addr));

	int clientfd=accept(sockfd,(sockaddr*)&client_addr,&client_addr_len);
	printf("new client fd %d IP:%s port:%d\n",clientfd,inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

	return 0;	
}
