#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void errif(bool condition,const char *errmsg){
	if(condition){
		perror(errmsg);
		exit(EXIT_FAILURE);
	}
}

int main(int argc,char **argv){
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	errif(sockfd==-1,"sockfd create error");
	struct sockaddr_in serv_addr;
	bzero(&serv_addr,sizeof(serv_addr));

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(8888);

	errif(bind(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr))==-1,"socket bind error");
	errif(listen(sockfd,10)==-1,"socket listen error");

	struct sockaddr_in client_addr;
	socklen_t client_addr_len=sizeof(client_addr);
	bzero(&client_addr,sizeof(client_addr));

	int clientfd=accept(sockfd,(sockaddr*)&client_addr,&client_addr_len);
	printf("new client fd %d IP:%s port:%d\n",clientfd,inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

	char buf[1024];
	while(true){
	
		bzero(buf,1024);
		ssize_t read_bytes=read(clientfd,buf,1024);
		if(read_bytes>0){
			printf("read from client:%s\n",buf);
			write(clientfd,buf,1024);
		}
		else if(read_bytes==0){
			printf("client disconnect\n");
			close(clientfd);
			break;
		}
		else if(read_bytes==-1){
			close(clientfd);
			errif(true,"socket read error\n");
		}

	}

	close(sockfd);

	return 0;	
}
