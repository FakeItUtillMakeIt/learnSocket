#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>

void setnonblocking(fd){
	fcntl(fd,F_SETFL,fcntl(fd,F_SETFL)|O_NONBLOCK);
}


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
	int listenfd=listen(sockfd,10);
	errif(listenfd==-1,"socket listen error");

	struct sockaddr_in client_addr;
	socklen_t client_addr_len=sizeof(client_addr);
	bzero(&client_addr,sizeof(client_addr));

	int connectfd;

	struct epoll_event ev,events[10];
	int nfds;
	int epollfd=epoll_create(10);
	errif(epollfd==-1,"epoll_create error\n");
	setnonblocking(sockfd);
	ev.events=EPOLLIN;
	ev.data.fd=sockfd;
	errif(epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev)==-1,"epoll_ctl:sockfd");

	char buf[1024];
	while(true){
		nfds=epoll_wait(epollfd,events,10,-1);
		errif(nfds==-1,"epoll_wait error");
		for(int n=0;n<nfds;n++){
			if(events[n].data.fd==sockfd){//sockfd has prepared
								
				connectfd=accept(sockfd,(sockaddr*)&client_addr,&client_addr_len);
				errif(connectfd==-1,"accept error");
				printf("new client fd %d IP:%s port:%d\n",connectfd,inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
				ev.events=EPOLLIN|EPOLLET;
				setnonblocking(connectfd);
				ev.data.fd=connectfd;
				errif(epoll_ctl(epollfd,EPOLL_CTL_ADD,connectfd,&ev)==-1,"epoll_ctl:connectfd");
			}
			else{
				if(events[n].events|EPOLLIN){
					ssize_t read_bytes=read(connectfd,buf,1024);
					if(read_bytes>0){
						printf("read from client:%s\n",buf);
						write(connectfd,buf,1024);
					}
					else if(read_bytes<=0){
						close(connectfd);
						printf("client disconnect\n");
						continue;
					}
				
				}
			}

		}

	}
	close(sockfd);

	return 0;	
}
