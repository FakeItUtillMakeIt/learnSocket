
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_LEN 1024
#define SERV_PORT 10005
#define LISTEN_COUNT 5

void sig_child(int signo){
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,&stat,WNOHANG))>0){
		std::cout<<"child"<<pid<<"terminated"<<std::endl;
	}
}


void str_echo(int sockfd){
	ssize_t n;
	char buf[MAX_LEN];
again:
	while((n=read(sockfd,buf,MAX_LEN))>0){
		write(sockfd,buf,n);
	}
	if(n<0 && errno==EINTR){
		goto again;
	}
	else if(n<0){
		std::cout<<"read error "<<std::endl;

	}
}

int main(int argc,char** argv){
	
	int listenfd,connfd,udpfd,nready,maxfdp1;
	char msg[MAX_LEN];
	pid_t childpid;
	fd_set rset;
	ssize_t n;
	socklen_t len;
	const int on=1;
	struct sockaddr_in cliaddr,servaddr;

	void sig_child(int);//function of signal dealed

	//create TCP socket
	listenfd=socket(AF_INET,SOCK_STREAM,0);

	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(SERV_PORT);

	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));//socket setting
	bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	listen(listenfd,LISTEN_COUNT);

	//create UDP socket
	udpfd=socket(AF_INET,SOCK_DGRAM,0);
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(SERV_PORT);
	
	bind(udpfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

	//build signal dealing program
	signal(SIGCHLD,sig_child);

	FD_ZERO(&rset);//initialize rset
	maxfdp1=std::max(listenfd,udpfd)+1;

	//deal events
	while(true){
		FD_SET(listenfd,&rset);
		FD_SET(udpfd,&rset);
		if((nready=select(maxfdp1,&rset,NULL,NULL,NULL))<0){
			if(errno==EINTR){
				continue;
			}
			else{
				std::cout<<"select error"<<std::endl;
				return -1;
			}
		}
		if(FD_ISSET(listenfd,&rset)){
			len=sizeof(cliaddr);
			connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&len);

			if((childpid=fork())==0){//child process
				close(listenfd);
				str_echo(connfd);//deal
				exit(0);
			}
			close(connfd);//parent close connfd
		}
		if(FD_ISSET(udpfd,&rset)){
			len=sizeof(cliaddr);
			n=recvfrom(udpfd,msg,MAX_LEN,0,(struct sockaddr*)&cliaddr,&len);
			sendto(udpfd,msg,n,0,(struct sockaddr*)&cliaddr,len);
		}
	}


	return 0;
}
