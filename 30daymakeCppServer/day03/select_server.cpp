#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>


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

	fd_set readfd;
	fd_set writefd;
	fd_set exceptionfd;

	FD_ZERO(&readfd);
	FD_ZERO(&writefd);
	FD_ZERO(&exceptionfd);

	struct timeval overtime;
	overtime.tv_sec=10;
	overtime.tv_usec=0;

	char buf[1024];
	while(true){
	
		bzero(buf,1024);

		FD_SET(clientfd,&readfd);
		FD_SET(clientfd,&writefd);
		FD_SET(clientfd,&exceptionfd);

		int retval=select(clientfd+1,&readfd,NULL,&exceptionfd,&overtime);
		if(retval<0){
			printf("selction failture\n");
			break;
		}
		if(FD_ISSET(clientfd,&readfd)){
					
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

		if(FD_ISSET(clientfd,&exceptionfd)){
			//exception thing ,reading pipe data use MSG_OOB
			ssize_t read_bytes=recv(clientfd,buf,1024,MSG_OOB);
			if(read_bytes<0){
				break;
			}
			printf("get %d bytes of oob data:%s",read_bytes,buf);
		}

	}

	close(sockfd);

	return 0;	
}
