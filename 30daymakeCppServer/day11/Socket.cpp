#include "Socket.h"
#include <fcntl.h>
#include "errif.h"

Socket::Socket():sockfd(-1){
	this->sockfd=socket(AF_INET,SOCK_STREAM,0);
	errif(this->sockfd==-1,"socket error");
}

Socket::Socket(int fd){

	this->sockfd=fd;

	errif(this->sockfd==-1,"socket error");

}

Socket::~Socket(){
	if(this->sockfd!=-1){
		close(this->sockfd);
		this->sockfd=-1;
	}
}

void Socket::bind_(InetAddress* inetaddr){
	int ret=bind(this->sockfd,(sockaddr*)&inetaddr->sock_addr,inetaddr->addr_len);

	errif(ret==-1,"bind error");
}

void Socket::listen_(){
	errif(listen(this->sockfd,this->maxfd)==-1,"listen error");
}

int Socket::accept_(InetAddress* inetaddr){
	int connectfd=accept(this->sockfd,(sockaddr*)&inetaddr->sock_addr,&inetaddr->addr_len);
	errif(connectfd==-1,"accept error");
	return connectfd;
}

void Socket::connect_(InetAddress *inetaddr){
	struct sockaddr_in addr=inetaddr->sock_addr;
	errif(connect(sockfd,(struct sockaddr*)&addr,sizeof(addr))==-1,"socket connect error");
}

int Socket::getfd(){
	return this->sockfd;
}

void Socket::setMaxfd(int num){
	this->maxfd=num;
}

void Socket::setnonblocking(){
	fcntl(sockfd,F_SETFL,fcntl(sockfd,F_GETFL)|O_NONBLOCK);
}

