#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#include "errif.h"

#define MAX_COUNT 10

int setnonblocking(int fd){
	int old_op=fcntl(fd,F_GETFL);
	int new_op=old_op | O_NONBLOCK;
	fcntl(fd,F_SETFL,new_op);
	return old_op;
}


int main(int argc,char **argv){

	int sockfd,listenfd,connectfd;
	struct sockaddr_in serv_addr,client_addr;
	socklen_t client_addrlen;

	sockfd=socket(AF_INET,SOCK_STREAM,0);

	errif(sockfd==-1,"socket error");

	bzero(&serv_addr,sizeof(serv_addr));
	bzero(&client_addr,sizeof(client_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8888);
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

	errif(bind(sockfd,(struct sockaddr*)(&serv_addr),sizeof(serv_addr))==-1,"bind error");
	listenfd=listen(sockfd,MAX_COUNT);

	int epollfd=epoll_create(MAX_COUNT);
	struct epoll_event ev,events[MAX_COUNT];
	bzero(events,MAX_COUNT);
	ev.events=EPOLLIN;
	ev.data.fd=sockfd;

	setnonblocking(sockfd);
	epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);

	char buf[1024];

	while(true){
		int retval=epoll_wait(epollfd,events,MAX_COUNT,-1);
		errif(retval==-1,"epoll_wait error");
		for(int i=0;i<retval;i++){
			bzero(buf,1024);
			if(events[i].data.fd==sockfd){
				bzero(&client_addr,sizeof(client_addr));
				connectfd=accept(events[i].data.fd,(struct sockaddr*)&client_addr,&client_addrlen);
				printf("new client fd:%d,IP:%s,port:%d\n",events[i].data.fd,inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
				bzero(&ev,sizeof(ev));
				ev.events=EPOLLIN | EPOLLET;
				ev.data.fd=connectfd;
				setnonblocking(connectfd);
				epoll_ctl(epollfd,EPOLL_CTL_ADD,connectfd,&ev);


			}
			else if(events[i].events & EPOLLIN){
				//read data
				bzero(buf,1024);
				while(true){
					ssize_t byte_read=read(events[i].data.fd,buf,1024);
					if(byte_read>0){
						printf("read from client fd:%d ,data:%s",events[i].data.fd,buf);
						write(events[i].data.fd,buf,1024);
					}
					else if(byte_read==-1 && errno==EINTR){//client trapin interupt
						continue;
					}
					else if(byte_read==-1 && ((errno==EAGAIN)||(errno==EWOULDBLOCK))){//none block IO,data has read over
						break;
					}
					else if(byte_read==0){//EOF event,usually client line off
						close(events[i].data.fd);
						break;
					}
				}
			}
		}
	}

	close(sockfd);

	return 0;
}
