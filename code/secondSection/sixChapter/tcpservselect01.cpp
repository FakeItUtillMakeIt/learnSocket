#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>

#define FD_SIZE 3
//6-21
//using select create server
int main(int argc,char** argv){
	int i,maxi,maxfd,listenfd,connfd,sockfd;
	int nready,client[FD_SIZE];
	ssize_t n;
	fd_set rset,allset;
	char buf[1024];
	socklen_t clientlen;
	struct sockaddr_in cliaddr,servaddr;

	if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0){
		std::cout<<"error in socket"<<std::endl;
		return -1;
	}

	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(10004);

	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		std::cout<<"error in bind"<<std::endl;
		std::cout<<errno<<std::endl;
		return -1;
	}
	
	if(listen(listenfd,5)<0){
		
		std::cout<<"error in listen"<<std::endl;
		return -1;
	}

	maxfd=listenfd;//init maxfd
	maxi=-1;
	for(i=0;i<FD_SIZE;i++){
		client[i]=-1;
	}
	FD_ZERO(&allset);
	FD_SET(listenfd,&allset);

	while(true){
		rset=allset;
		nready=select(maxfd+1,&rset,NULL,NULL,NULL);

		if(FD_ISSET(listenfd,&rset)){//
			clientlen=sizeof(cliaddr);
			if((connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&clientlen))<0){
				std::cout<<"accept error"<<std::endl;
				continue;
			}
			for(i=0;i<FD_SIZE;i++){
				if(client[i]<0){
					client[i]=connfd;//save descriptor
					break;
				}
			}
			if(i==FD_SIZE){
				std::cout<<"too many clients"<<std::endl;
				return -1;
			}
			FD_SET(connfd,&allset);//add new fd to set
			if(connfd>maxfd){
				maxfd=connfd;//for select
			}
			if(i>maxi){
				maxi=i;//max index in client array			
			}
			if(--nready<=0){
				continue;//no more readable fd
			}

		}

		for(i=0;i<maxi;i++){//check all client for data
			if((sockfd=client[i])<0){
				continue;
			}
			if(FD_ISSET(sockfd,&rset)){
				if((n=read(sockfd,buf,1024))<0){
					close(sockfd);
					FD_CLR(sockfd,&allset);
					client[i]=-1;
				}
				else{
					write(sockfd,buf,n);
				}
				if(--nready<=0){//no more readable fd 
					break;
				}
			}			
		}
	}
	return 0;
}
