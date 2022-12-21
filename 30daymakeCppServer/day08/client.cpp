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

int main(int argc,char **argv){
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	errif(sockfd==-1,"socket create error");
	struct sockaddr_in serv_addr;
	bzero(&serv_addr,sizeof(serv_addr));

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(8888);

	errif(connect(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr))==-1,"socket connect error");
	char buf[1024];
	while(true){
		bzero(buf,sizeof(char)*1024);
		fgets(buf,sizeof(char)*1024,stdin);
		ssize_t write_bytes=write(sockfd,buf,sizeof(char)*1024);
		if(write_bytes==-1){
			printf("socket disconnect,cant write");
			break;
		}
		bzero(buf,sizeof(char)*1024);
		ssize_t read_bytes=read(sockfd,buf,sizeof(char)*1024);
		if(read_bytes>0){
			printf("message from server:%s\n",buf);
		}
		else if(read_bytes==0){
			printf("server socket disconnect\n");
			break;
		}
		else if(read_bytes==-1){
			close(sockfd);
			errif(true,"socket read error");
		}
	}
	return 0;
}
